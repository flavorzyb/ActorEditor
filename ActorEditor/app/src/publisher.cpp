#include <QtCore>
#include <QRgb>
#include "include/publisher.h"
#include "include/fileutils.h"
#include "include/imageutils.h"
#include "include/MaxRectsBinPack.h"
#include "include/PVRTDecompress.h"
#include "include/PVRTexture.h"
#include "include/PVRTextureUtilities.h"
#include "include/ziputils.h"
#include "include/xmlparser.h"

using namespace rbp;

Publisher::Publisher(const SettingsVO &svo):
    m_settings(svo)
  , m_isFinished(false)
  , m_count(0)
  , m_finishedCount(0)
{
}

Publisher::~Publisher()
{
}

QVector<QString> Publisher::getAllTextureLists()
{
    return FileUtils::getAllActorDir(m_settings.importPath());
}

bool Publisher::publish(const QVector<QString> &fileLists)
{
    m_isFinished = false;
    m_finishedCount = 0;
    m_count = fileLists.size();

    QVector<QString>::const_iterator iterator = fileLists.constBegin();
    for(; iterator != fileLists.constEnd(); iterator++)
    {
        if (false == createTexture(*iterator))
        {
            return false;
        }

        m_finishedCount++;
    }

    m_isFinished = true;
    return true;
}

static void freeImage(QVector<Image *> * imageLists)
{
    if (imageLists != NULL)
    {
        QVector<Image *>::iterator iterator = imageLists->begin();
        Image * pImage = NULL;
        for (; iterator != imageLists->end(); iterator++)
        {
            pImage = *iterator;
            delete pImage;
        }

        imageLists->clear();
    }
}

bool Publisher::createTexture(const QString &path)
{
    int max = 0;
    Image * pImg = NULL;
    QVector<Image *> imageLists;
    QString importFullPath = FileUtils::getAbsoluteFilePath(m_settings.importPath());

    QVector<QString> pngFiles = FileUtils::getAllActorTextureFile(path);

    QVector<QString>::const_iterator iterator = pngFiles.constBegin();

    for(; iterator != pngFiles.constEnd(); iterator++)
    {
        pImg = new Image;
        QString fullPath = *iterator;
        if (pImg->init(fullPath, FileUtils::getActorTextureName(fullPath, importFullPath), m_settings.scale()))
        {
            imageLists.push_back(pImg);
            if (max < pImg->getMinWidth())
            {
                max = pImg->getMinWidth();
            }

            if (max < pImg->getMinHeight())
            {
                max = pImg->getMinHeight();
            }
        }
        else
        {
            delete pImg;
            freeImage(&imageLists);
            return false;
        }
    }

    // 释放imageLists资源
    int size = ImageUtils::getMinPowOf2(max + 4);

    // the size after max rect size
    size = findMaxRectSize(imageLists, size);

    // create big image
    QImage bigImage(size, size, QImage::Format_RGBA8888);

    if (!createNewImage(&bigImage, imageLists, size))
    {
        return false;
    }

    QString actorName = FileUtils::getActorName(path, importFullPath);
    QString pvrFilePath = m_settings.exportPath() + "/" + actorName +"/" + actorName + ".pvr.ccz";

    FileUtils::createParentDirectory(pvrFilePath);
    if (!saveToPVR(bigImage, pvrFilePath))
    {
        qDebug()<<"error:create pvr.ccz file fail"<<pvrFilePath;
        freeImage(&imageLists);
        return false;
    }

    QString plistString = createPlist(imageLists, size, actorName + ".pvr.ccz");
    QString plistPath = m_settings.exportPath() + "/" + actorName +"/" + actorName + ".plist";
    if (!FileUtils::writeFile(plistPath.toStdString().c_str(),
                             "wb+",
                             plistString.toStdString().c_str(),
                             plistString.toStdString().length()))
    {
        qDebug()<<"error:write plist file fail"<<plistPath;
        freeImage(&imageLists);
        return false;
    }

    freeImage(&imageLists);

    // copy xml file
    if (!copyXmlFile(path))
    {
        qDebug()<<"error:copy xml file fail"<<path;
        return false;
    }

    return true;
}

int Publisher::findMaxRectSize(const QVector<Image *> &imageLists, int size)
{
    MaxRectsBinPack binPack(size, size);
    QVector<Image *>::const_iterator iterator = imageLists.constBegin();
    Image *pImage = NULL;

    for(; iterator != imageLists.constEnd(); iterator++)
    {
        pImage = (*iterator);
        Rect bpRect = binPack.Insert(pImage->getMinWidth() + 4, pImage->getMinHeight() + 4, MaxRectsBinPack::RectBestShortSideFit);

        if ((bpRect.width == 0) || (bpRect.height == 0))
        {
            return findMaxRectSize(imageLists, size * 2);
        }

        pImage->setOffsetX(bpRect.x + 2);
        pImage->setOffsetY(bpRect.y + 2);
        if (bpRect.width == (pImage->getMinHeight() + 4))
        {
            pImage->setRotated(true);
        }
        else
        {
            pImage->setRotated(false);
        }
    }

    return size;
}

bool Publisher::createNewImage(QImage * result, const QVector<Image *> &imageLists, int size)
{
    if (NULL == result)
    {
        return false;
    }

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            result->setPixel(x, y, 0);
        }
    }

    QVector<Image *>::const_iterator iterator = imageLists.constBegin();
    Image *pImage = NULL;
    for(; iterator != imageLists.constEnd(); iterator++)
    {
        pImage = (*iterator);

        int w = pImage->getMinWidth();
        int h = pImage->getMinHeight();

        int dx = pImage->minx();
        int dy = pImage->miny();
        int rx = 0;
        int ry = 0;
        QRgb value = 0;
        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                value = pImage->getImage().pixel(x + dx, y + dy);

                if (pImage->rotated())
                {
                    rx = pImage->offsetX() + h - y -1;
                    ry = pImage->offsetY() + x;
                }
                else
                {
                    rx = pImage->offsetX() + x;
                    ry = pImage->offsetY() + y;
                }

                if ((rx >= size) || (ry >= size))
                {
                    return false;
                }

                result->setPixel(rx, ry, value);
            }
        }
    }

    return true;
}

static QString getSizeString(int w, int h)
{
    char str[256] = {0};
    sprintf(str, "<string>{%d,%d}</string>\n", w, h);

    return str;
}

static QString getRectString(int x, int y, int w, int h)
{
    char str[1024] = {0};
    sprintf(str, "<string>{{%d,%d},{%d,%d}}</string>\n", x, y, w, h);
    return str;
}

static QString createFrameString(const Image * pImage)
{
    QString result = "            <key>" + pImage->getName() + "</key>\n";
    int w = pImage->getMinWidth();
    int h = pImage->getMinHeight();
    int sw = pImage->width();
    int sh = pImage->height();

    result += "            <dict>\n";
    result += "                <key>frame</key>\n";
    result += "                " + getRectString(pImage->offsetX(), pImage->offsetY(), w, h);
    result += "                <key>offset</key>\n";
    result += "                " + getSizeString(pImage->ox(), pImage->oy());
    result += "                <key>rotated</key>\n";
    if (pImage->rotated())
    {
        result += "                <true/>\n";
    }
    else
    {
        result += "                <false/>\n";
    }

    result += "                <key>sourceColorRect</key>\n";
    result += "                " + getRectString(0, 0, sw, sh);
    result += "                <key>sourceSize</key>\n";
    result += "                " + getSizeString(sw, sh);
    result += "            </dict>\n";

    return result;
}

QString Publisher::createPlist(const QVector<Image *> &imageLists, int size, const QString & textureName)
{
    QString result =    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
            result +=   "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";
            result +=   "<plist version=\"1.0\">\n";
            result +=   "   <dict>\n";
            result +=   "       <key>frames</key>\n";
            result +=   "       <dict>\n";

            QVector<Image *>::const_iterator iterator = imageLists.constBegin();
            for(; iterator != imageLists.constEnd(); iterator++)
            {
                result += createFrameString(*iterator);
            }

            result +=   "       </dict>\n";
            result +=   "       <key>metadata</key>\n";
            result +=   "       <dict>\n";
            result +=   "           <key>format</key>\n";
            result +=   "           <integer>2</integer>\n";
            result +=   "           <key>realTextureFileName</key>\n";
            result +=   "           <string>" + textureName + "</string>\n";
            result +=   "           <key>size</key>\n";
            result +=   "           " + getSizeString(size, size);
            result +=   "           <key>smartupdate</key>\n";
            result +=   "           <string>ActorEditor</string>\n";
            result +=   "           <key>textureFileName</key>\n";
            result +=   "           <string>" + textureName + "</string>\n";
            result +=   "       </dict>\n";
            result +=   "   </dict>\n";
            result +=   "</plist>\n";

            return result;
}

bool Publisher::copyXmlFile(const QString &path)
{
    QString importFullPath = FileUtils::getAbsoluteFilePath(m_settings.importPath());
    QString actorName = FileUtils::getActorName(path, importFullPath);

    QString srcXmlPath = m_settings.importPath() + "/" + actorName +"/" + actorName + ".xml";

    XMLParser parser;
    if (!parser.parser(srcXmlPath, actorName, m_settings.scale()))
    {
        qDebug()<<"error: parse xml fail("<<srcXmlPath<<")";
        return false;
    }

    QString destXmlPath = m_settings.exportPath() + "/" + actorName +"/" + actorName + ".xml";

    QString xmlString = parser.getXmlString();

    return FileUtils::writeFile(destXmlPath.toStdString().c_str(), "wb", xmlString.toStdString().c_str(), xmlString.toStdString().length());
}

bool Publisher::saveToPVR(const QImage &image, const QString &filePath)
{
    QImage result = image.convertToFormat(QImage::Format_ARGB4444_Premultiplied);
    int w = result.width();
    int h = result.height();
    int len = w * h * 4;
    if (len < 4)
    {
        return false;
    }

    unsigned char * pData = new unsigned char [len];
    memset(pData, 0, len * sizeof(unsigned char));

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            QRgb v = result.pixel(x, y);
            pData[4 * (y * w + x)    ] = qRed(v);
            pData[4 * (y * w + x) + 1] = qGreen(v);
            pData[4 * (y * w + x) + 2] = qBlue(v);
            pData[4 * (y * w + x) + 3] = qAlpha(v);
        }
    }

    pvrtexture::CPVRTextureHeader header(pvrtexture::PVRStandard8PixelType.PixelTypeID, w, h);
    pvrtexture::CPVRTexture pvrTexture(header, pData);
    delete [] pData;

    QString tmpFile = filePath.left(filePath.length() - 4);
    if (!pvrTexture.saveFile(tmpFile.toStdString().c_str()))
    {
        FileUtils::unlink(tmpFile);
        qDebug()<<"error: pvr Transcode or save file fail"<<filePath;
        return false;
    }

    unsigned long fileSize = 0;
    unsigned char * pvrData = FileUtils::getFileData(tmpFile.toStdString().c_str(), "rb", &fileSize);

    FileUtils::unlink(tmpFile);

    if ((pvrData == NULL) || (fileSize == 0))
    {
        qDebug()<<"error: read pvr tmp file fail"<<tmpFile;
        return false;
    }


    if (!ZipUtils::ccDeflateCCZFile(filePath.toStdString().c_str(), pvrData, fileSize))
    {
        qDebug()<<"error: ccz pvr tmp file fail"<<filePath;
        delete []pvrData;
        return false;
    }

    delete []pvrData;

    return true;
}
