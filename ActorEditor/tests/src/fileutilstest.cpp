#include "include/fileutilstest.h"
#include "include/fileutils.h"

FileUtilsTest::FileUtilsTest()
{
}

void FileUtilsTest::init()
{
    m_sActorPath = FileUtils::getAbsoluteFilePath("images/Actor");
}

void FileUtilsTest::cleanup()
{
}

void FileUtilsTest::testGetAllActorDirReturnPathLists()
{
    QVector<QString> pathLists = FileUtils::getAllActorDir(m_sActorPath);
    QVERIFY(pathLists.size() > 0);
}

void FileUtilsTest::testGetActorNameReturnName()
{
    QVector<QString> pathLists = FileUtils::getAllActorDir(m_sActorPath);

    QVector<QString>::const_iterator iterator = pathLists.constBegin();
    for (; iterator != pathLists.constEnd(); iterator++)
    {
        QVERIFY(FileUtils::getActorName(*iterator, m_sActorPath).length() > 0);
        //printf("name: %s\n", FileUtils::getActorName(*iterator, m_sActorPath).toStdString().c_str());
    }
}

void FileUtilsTest::testGetActorTextureNameReturnName()
{
    QString name = FileUtils::getActorTextureName(FileUtils::getAbsoluteFilePath("images/Actor/ailisi_1/texture/ctoufa03.png"),
                                                  FileUtils::getAbsoluteFilePath("images/Actor"));

    QCOMPARE(name, tr("ailisi_1/ctoufa03.png"));
}

