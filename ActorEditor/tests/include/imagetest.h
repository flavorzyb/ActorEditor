#ifndef IMAGETEST_H
#define IMAGETEST_H

#include <QObject>
#include <QString>
#include <QTest>

#include "include/image.h"

class ImageTest : public QObject
{
    Q_OBJECT
public:
    ImageTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void testInitReturnBoolean();

private:
    QString m_imagePath;
    Image * m_image;
};

#endif // IMAGETEST_H
