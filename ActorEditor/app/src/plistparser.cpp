#include <QFile>
#include <QtCore>
#include <QDomDocument>
#include <QRegExp>
#include <QStringList>

#include "include/plistparser.h"

PlistParser::PlistParser():
    m_framesMap()
{
}

PlistParser::~PlistParser()
{
}

static bool loadToDocument(const QString &plistPath, QDomDocument * doc)
{
    QFile file(plistPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    if (!doc->setContent(&file))
    {
        file.close();
        return false;
    }

    file.close();

    return true;
}

bool PlistParser::load(const QString &plistPath)
{
    QDomDocument doc;
    m_framesMap.clear();
    if (!loadToDocument(plistPath, &doc))
    {
        qDebug()<<"error: loadToDocument(\""<<plistPath<<"\") fail.";
        return false;
    }

    QDomElement element = doc.documentElement();

    if (!element.hasChildNodes())
    {
        qDebug()<<"error: error plist(" << plistPath <<") file.";
        return false;
    }

    //<dict></dict>
    QDomNode node = element.firstChild();

    if (!node.hasChildNodes())
    {
        qDebug()<<"error: error plist(" << plistPath <<") file.";
        return false;
    }

    if (node.childNodes().count() != 4)
    {
        qDebug()<<"error: error plist(" << plistPath <<") file.";
        return false;
    }

    return parseFrames(node);
}

QMap<QString, FrameVO> PlistParser::getFramesMap() const
{
    return m_framesMap;
}

bool PlistParser::parseFrames(const QDomNode &node)
{
    if (!node.hasChildNodes())
    {
        qDebug()<<"error: error plist file.";
        return false;
    }

    //<key>frames</key>
    QDomElement framesElement = node.firstChild().toElement();
    if (framesElement.text() != "frames")
    {
        qDebug()<<"error: error plist file need frames dict.";
        return false;
    }

    QDomNode framelist = node.childNodes().at(1);

    QDomNodeList framesNodeList = framelist.childNodes();

    int count = framesNodeList.count();

    if (!((count % 2 == 0) && (count > 0)))
    {
        qDebug()<<"error: error plist file frame.";
        return false;
    }

    for(int i = 0; i < count / 2; i++)
    {
        FrameVO fvo;
        if (!parseFrameVO(framesNodeList.at(i*2), framesNodeList.at(i*2+1), &fvo))
        {
            qDebug()<<"error: error parseFrameVO.";
            return false;
        }

        m_framesMap.insert(fvo.name(), fvo);
    }

    return true;
}

bool PlistParser::parseFrameVO(const QDomNode &nameNode, const QDomNode &frameNode, FrameVO *out)
{
    if (NULL == out)
    {
        return false;
    }

    out->setName(nameNode.toElement().text());

    QDomNodeList framesNodeList = frameNode.childNodes();
    int count = framesNodeList.count();

    if (count != 10)
    {
        qDebug()<<"error: error parseFrameVO count != 10.";
        return false;
    }

    QPoint size;
    QRect rect;
    for(int i = 0; i < count / 2; i++)
    {
        QDomNode node = framesNodeList.at(i * 2);
        QDomElement value = framesNodeList.at(i * 2 + 1).toElement();
        if (node.toElement().text() == "frame")
        {
            if (!parseRectString(value, &rect))
            {
                return false;
            }

            out->setRect(rect);
        }
        else if (node.toElement().text() == "offset")
        {
            if (!parseSizeString(value, &size))
            {
                return false;
            }

            out->setOffset(size);
        }
        else if (node.toElement().text() == "rotated")
        {
            QString rotatedStr = value.tagName().toLower();
            if (rotatedStr == "true")
            {
                out->setRotated(true);
            }
            else if (rotatedStr == "false")
            {
                out->setRotated(false);
            }
            else
            {
#ifdef SKIP_ROTATED_ERROR
                if (value.text().trimmed()=="1")
                {
                    out->setRotated(true);
                }
                else if (value.text().trimmed()=="0")
                {
                    out->setRotated(false);
                }
                else
                {
                    qDebug() << "error: error setRotated " <<value.text();
                    return false;
                }
#else
                qDebug()<<"error: error setRotated" << value.tagName();
                return false;
#endif
            }
        }
        else if (node.toElement().text() == "sourceColorRect")
        {
            if (!parseRectString(value, &rect))
            {
                return false;
            }

            out->setSourceColorRect(rect);
        }
        else if (node.toElement().text() == "sourceSize")
        {
            if (!parseSizeString(value, &size))
            {
                return false;
            }

            QSize sourceSize;
            sourceSize.setWidth(size.x());
            sourceSize.setHeight(size.y());
            out->setSourceSize(sourceSize);
        }
        else
        {
            qDebug()<<"error: error parseFrameVO nothing to parse";
            return false;
        }
    }

    return true;
}

static QStringList parseAllInteger(const QString & text)
{
    QRegExp re("(\\d+)");
    int pos = 0;
    QStringList result;
    while ((pos = re.indexIn(text, pos)) != -1)
    {
        result.push_back(re.cap(1));
        pos += re.matchedLength();
    }

    return result;
}

bool PlistParser::parseSizeString(const QDomElement &element, QPoint *out)
{
    QStringList list = parseAllInteger(element.text());

    if (list.size() != 2)
    {
        qDebug()<<"error: error parseSizeString list size = " << list.size();
        return false;
    }

    QPoint result(list.at(0).toInt(), list.at(1).toInt());
    *out = result;

    return true;
}

bool PlistParser::parseRectString(const QDomElement &element, QRect *out)
{
    QStringList list = parseAllInteger(element.text());

    if (list.size() != 4)
    {
        qDebug()<<"error: error parseRectString list size = " << list.size();
        return false;
    }

    QRect result(list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt(), list.at(3).toInt());
    *out = result;

    return true;
}
