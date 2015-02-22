#ifndef FRAMEVO_H
#define FRAMEVO_H
#include <QRect>
#include <QSize>
#include <QPoint>
#include <QString>

class FrameVO
{
public:
    FrameVO();
    FrameVO(const QSize & size);
    FrameVO(int width, int height);
    FrameVO(const FrameVO & fvo);
    const FrameVO & operator= (const FrameVO & fvo);

    QString name() const;
    void setName(const QString &fileName);

    QRect rect() const;
    void setRect(const QRect &rect);
    void setRect(int x, int y, int width, int height);

    QPoint offset() const;
    void setOffset(const QPoint &offset);
    void setOffset(int x, int y);

    bool rotated() const;
    void setRotated(bool rotated);

    QRect sourceColorRect() const;
    void setSourceColorRect(const QRect &sourceColorRect);
    void setSourceColorRect(int x, int y, int width, int height);

    QSize sourceSize() const;
    void setSourceSize(const QSize &sourceSize);
    void setSourceSize(int width, int height);

private:
    QString m_name;
    QRect m_rect;
    QPoint m_offset;
    bool m_rotated;
    QRect m_sourceColorRect;
    QSize m_sourceSize;
};

#endif // FRAMEVO_H
