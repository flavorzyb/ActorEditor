#ifndef IMAGEVO_H
#define IMAGEVO_H

#include <QObject>
#include <QString>
#include <QImage>

class Image : public QObject
{
    Q_OBJECT
public:
    Image();
    ~Image();

    bool init(const QString & fullPath, const QString & name, double scaled = 1);
    const QImage & getImage() const;

    int width() const;
    int height() const;
    int minx() const;
    int miny() const;
    int maxx() const;
    int maxy() const;

    int getMinWidth() const;
    int getMinHeight() const;

    int offsetX() const;
    void setOffsetX(int offsetX);

    int offsetY() const;
    void setOffsetY(int offsetY);

    bool rotated() const;
    void setRotated(bool rotated);

    QString getName() const;

    int ox() const;
    int oy() const;

private:
    Image(const Image & img);
    Image & operator = (const Image & img);

    void findCropRect();
    void initOXOY();

private:
    QString m_fullPath;
    QString m_name;
    QImage  m_img;
    int     m_width;
    int     m_height;
    int     m_minx;
    int     m_miny;
    int     m_maxx;
    int     m_maxy;
    int     m_offsetX;
    int     m_offsetY;
    bool    m_rotated;
    int     m_ox;
    int     m_oy;
};

#endif // IMAGEVO_H
