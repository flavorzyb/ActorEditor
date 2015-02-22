#ifndef IMAGEUTILS_H
#define IMAGEUTILS_H

#include <QObject>

class ImageUtils : public QObject
{
    Q_OBJECT
public:
    static int getMinPowOf2(int value);
};

#endif // IMAGEUTILS_H
