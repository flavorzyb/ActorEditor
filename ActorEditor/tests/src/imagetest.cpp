#include "include/imagetest.h"
#include "include/fileutils.h"

ImageTest::ImageTest()
{

}

void ImageTest::init()
{
    m_imagePath = FileUtils::getAbsoluteFilePath("images/Actor/ailisi_1/texture/ctoufa03.png");
    m_image = new Image();
}

void ImageTest::cleanup()
{
    delete m_image;
}

void ImageTest::testInitReturnBoolean()
{
    QCOMPARE(m_image->init(m_imagePath+"adafdsdf.png", "ailisi_1/ctoufa03.png", 0.45f), false);

    QCOMPARE(m_image->init(m_imagePath, "ailisi_1/ctoufa03.png", 0.45f), true);

    QCOMPARE(m_image->width(), 69);
    QCOMPARE(m_image->height(), 61);

    QCOMPARE(m_image->minx(), 0);
    QCOMPARE(m_image->miny(), 0);
    QCOMPARE(m_image->maxx(), 68);
    QCOMPARE(m_image->maxy(), 60);
}
