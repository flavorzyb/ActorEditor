#include <QDomDocument>
#include <QFile>
#include <iostream>
#include <QtCore>
#include <QImage>
#include <QTextStream>

#include "batchapplication.h"
#include "include/fileutils.h"
#include "include/plistparser.h"
#include "include/publisher.h"
#include "include/xmlparser.h"

const static QString IMPORT_ACTOR_PATH = "/Users/admin/program/c/c-actor/raw/Actor";
const static QString PUBLISH_ACTOR_PATH = "/Users/admin/program/c/c-actor/published/ios/Actor";

const static QString IMPORT_EFX_PATH = "/Users/admin/program/c/c-actor/raw/efx";
const static QString PUBLISH_EFX_PATH = "/Users/admin/program/c/c-actor/published/ios/efx";

const static QString EXPORT_ACTOR_PATH = "/Users/admin/program/c/c-actor/published/ios/Actor";
const static QString EXPORT_EFX_PATH = "/Users/admin/program/c/c-actor/published/ios/efx";

BatchApplication::BatchApplication(int argc, char *argv[]):QCoreApplication(argc, argv)
{
}

BatchApplication::~BatchApplication()
{
}

int BatchApplication::run()
{
//    if (!checkAllXmlFile(IMPORT_ACTOR_PATH))
//    {
//        qDebug()<<"error: some xml fail is error.";
//        return -1;
//    }

//    if (!checkAllXmlFile(IMPORT_EFX_PATH))
//    {
//        qDebug()<<"error: some xml fail is error.";
//        return -1;
//    }

//    if (!checkAllXmlFile(EXPORT_ACTOR_PATH))
//    {
//        qDebug()<<"error: some xml fail is error.";
//        return -1;
//    }

    if (!checkAllXmlFile(EXPORT_EFX_PATH))
    {
        qDebug()<<"error: some xml fail is error.";
        return -1;
    }

    if (!runActor())
    {
        qDebug()<<"error: runActor fail.";
        return -1;
    }

    if (!runEfx())
    {
        qDebug()<<"error: runEfx fail.";
        return -1;
    }

    printf("completed!\n");
    flush();

    return 0;
}

bool BatchApplication::runResizeImage(const QString & importPath, const QString & exportPath)
{
    QString importFullPath = FileUtils::getAbsoluteFilePath(importPath);
    QString exportFullPath = FileUtils::getAbsoluteFilePath(exportPath);

    QVector<QString> textureFileLists = FileUtils::getAllActorTextureFile(importFullPath);
    QVector<QString> publishPlistFileLists = FileUtils::getAllActorPlistFile(exportFullPath);
    QVector<QString>::iterator it = publishPlistFileLists.begin();
    PlistParser parser;
    QMap<QString, FrameVO> framesMap;
    for(; it != publishPlistFileLists.end(); it++)
    {
        if (!parser.load(*it))
        {
            qDebug()<<"parse error file:"<< *it;
            return false;
        }

        QMap<QString, FrameVO> frames = parser.getFramesMap();
        QMap<QString, FrameVO>::const_iterator framesIterator = frames.constBegin();
        for(; framesIterator != frames.constEnd(); framesIterator++)
        {
            if (framesMap.contains(framesIterator.key()))
            {
                qDebug()<<"error: has found texture:"<<framesIterator.key();
                return false;
            }

            framesMap.insert(framesIterator.key(), framesIterator.value());
        }
    }

    QVector<QString>::const_iterator iterator = textureFileLists.constBegin();
    for(; iterator != textureFileLists.constEnd(); iterator++)
    {
        QString actorTextureName = FileUtils::getActorTextureName(*iterator, importFullPath);
        if (!framesMap.contains(actorTextureName))
        {
            qDebug()<<"error: has found in frames map: "<<actorTextureName;
            return false;
        }

        if (!resizeImage(*iterator, framesMap.value(actorTextureName)))
        {
            qDebug()<<"error: resizeImage fail "<<actorTextureName;
            return false;
        }

        printf("resizeImage %s ... ... succ!\n", actorTextureName.toStdString().c_str());
        flush();
    }

    return true;
}

bool BatchApplication::checkAllXmlFile(const QString & fullPath)
{
    QVector<QString> result = getAllErrorXmlFile(fullPath);

    if (result.size() > 0)
    {
        fixedAllXmlFile(result);
        result = getAllErrorXmlFile(fullPath);
    }

    if (result.size() == 0)
    {
        return true;
    }

    qDebug()<<"error xml file count:"<<result.size();

    QVector<QString>::const_iterator it = result.constBegin();
    for(; it != result.constEnd(); it++)
    {
        qDebug()<<"error xml file:"<<*it;
    }

    return false;
}

QVector<QString> BatchApplication::getAllErrorXmlFile(const QString &fullPath)
{
    QVector<QString> result;
    QVector<QString> xmlFileList = FileUtils::getAllXMLFile(fullPath);
    QVector<QString>::const_iterator it = xmlFileList.constBegin();
    XMLParser xmlParse;

    for(; it != xmlFileList.constEnd(); it++)
    {
        if (!xmlParse.parser(*it, "test", 1))
        {
            result.push_back(*it);
        }
    }

    return result;
}

static QString getFileContent(const QString & filePath)
{
    QFile file(filePath);
    QString result;
    if (file.open(QFile::ReadOnly))
    {
        QTextStream ts(&file);
        result = ts.readAll();
        file.close();
    }

    return result;
}

const static QString STR_SKELETON_BEGAN = "<skeleton ";
const static QString STR_SKELETON_END = "</skeleton>";
void BatchApplication::fixedAllXmlFile(const QVector<QString> &xmlFileLists)
{
    QVector<QString>::const_iterator it = xmlFileLists.constBegin();
    for(; it != xmlFileLists.constEnd(); it++)
    {
        QString xmlString = getFileContent(*it);
        int index = xmlString.indexOf(STR_SKELETON_BEGAN);
        if (index != -1)
        {
            xmlString = xmlString.right(xmlString.length() - index + 1);
        }

        index = xmlString.indexOf(STR_SKELETON_END);
        if (index > 0)
        {
            QString result = xmlString.left(index) + STR_SKELETON_END;
            FileUtils::writeFile(it->toStdString().c_str(), "wb+", result.toStdString().c_str(), result.toStdString().length());
        }
    }
}

bool BatchApplication::runActor()
{
    if (!runResizeImage(IMPORT_ACTOR_PATH, PUBLISH_ACTOR_PATH))
    {
        qDebug()<<"error: runResizeImage Actor";
        return false;
    }

    SettingsVO svo;
    svo.setImportPath(IMPORT_ACTOR_PATH);
    svo.setExportPath(EXPORT_ACTOR_PATH);
    svo.setScale(1);

    Publisher publish(svo);
    QVector<QString> fileLists = publish.getAllTextureLists();

    return publish.publish(fileLists);
}

bool BatchApplication::runEfx()
{
    if (!runResizeImage(IMPORT_EFX_PATH, PUBLISH_EFX_PATH))
    {
        qDebug()<<"error: runResizeImage efx";
        return false;
    }

    SettingsVO svo;
    svo.setImportPath(IMPORT_EFX_PATH);
    svo.setExportPath(EXPORT_EFX_PATH);
    svo.setScale(1);

    Publisher publish(svo);
    QVector<QString> fileLists = publish.getAllTextureLists();

    return publish.publish(fileLists);
}

bool BatchApplication::resizeImage(const QString &fullPath, const FrameVO &fvo)
{
    QImage image;
    if (!image.load(fullPath, "PNG"))
    {
        qDebug()<<"error: load image file faild "<<fullPath;
        return false;
    }

//    double wr = image.width() * 1.0f / fvo.sourceSize().width();
//    double hr = image.height() * 1.0f / fvo.sourceSize().height();

//    qDebug()<<"image w:"<<image.width()
//            <<", h:"<<image.height()
//            <<", fvo w:"<<fvo.sourceSize().width()
//            <<", h:"<<fvo.sourceSize().height()
//            <<", w rate:"<<wr
//            <<", h rate:"<<hr;

    if ((fvo.sourceSize().width() != image.width()) || (fvo.sourceSize().height() != image.height()))
    {
        image = image.scaled(fvo.sourceSize().width(), fvo.sourceSize().height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        image.save(fullPath, "PNG", 100);
    }

    return true;
}
