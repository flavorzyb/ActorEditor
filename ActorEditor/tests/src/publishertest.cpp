#include "include/fileutils.h"
#include "include/publishertest.h"

PublisherTest::PublisherTest()
{
}

void PublisherTest::init()
{
    m_importPath = FileUtils::getAbsoluteFilePath("images/Actor");
    m_exportPath = FileUtils::getAbsoluteFilePath("output/Actor");

    SettingsVO svo;
    svo.setImportPath(m_importPath);
    svo.setExportPath(m_exportPath);
    svo.setScale(0.45f);

    m_publisher = new Publisher(svo);
}

void PublisherTest::cleanup()
{
    delete m_publisher;
}

void PublisherTest::testGetAllTextureListsReturnLists()
{
    QVector<QString> fileLists = m_publisher->getAllTextureLists();
    QCOMPARE(fileLists.size(), 8);
}

void PublisherTest::testPublishReturnBoolean()
{
    QVector<QString> fileLists = m_publisher->getAllTextureLists();
    QCOMPARE(m_publisher->publish(fileLists), true);
}
