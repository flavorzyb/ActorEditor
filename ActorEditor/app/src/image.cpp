#include "include/image.h"

Image::Image():
    m_fullPath("")
  , m_name("")
  , m_minx(0)
  , m_miny(0)
  , m_maxx(0)
  , m_maxy(0)
  , m_offsetX(0)
  , m_offsetY(0)
  , m_rotated(false)
  , m_ox(0)
  , m_oy(0)
{
}

Image::~Image()
{
}

bool Image::init(const QString &fullPath, const QString &name, double scaled)
{
    m_name = name;

    if (!m_img.load(fullPath, "PNG"))
    {
        return false;
    }

    if (scaled > 1.0f)
    {
        return false;
    }

    int w = scaled * m_img.width();
    int h = scaled * m_img.height();

    m_img = m_img.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_width = m_img.width();
    m_height = m_img.height();

    findCropRect();

    initOXOY();

    return true;
}

const QImage & Image::getImage() const
{
    return m_img;
}
int Image::width() const
{
    return m_width;
}

int Image::height() const
{
    return m_height;
}

int Image::minx() const
{
    return m_minx;
}

int Image::miny() const
{
    return m_miny;
}

int Image::maxx() const
{
    return m_maxx;
}

int Image::maxy() const
{
    return m_maxy;
}

int Image::getMinWidth() const
{
    return (m_maxx - m_minx + 1);
}

int Image::getMinHeight() const
{
    return (m_maxy - m_miny + 1);
}

void Image::findCropRect()
{
    m_minx = 999999;
    m_miny = 999999;

    m_maxx = 0;
    m_maxy = 0;

    for (int x = 0; x < m_width; ++x)
    {
        for (int y = 0; y < m_height; ++y)
        {
            if (qAlpha(m_img.pixel(x, y)) != 0)
            {
                if (m_minx > x)
                {
                    m_minx = x;
                }

                if (m_miny > y)
                {
                    m_miny = y;
                }

                if (m_maxx < x)
                {
                    m_maxx = x;
                }

                if (m_maxy < y)
                {
                    m_maxy = y;
                }
            }
        }
    }

    //fixed value when no blank
    if (m_minx >= m_maxx)
    {
        m_minx = 0;
        m_maxx = m_width - 1;
    }

    if (m_miny >= m_maxy)
    {
        m_miny = 0;
        m_maxy = m_height - 1;
    }
}

void Image::initOXOY()
{
    int w = m_maxx - m_minx + 1;
    int h = m_maxy - m_miny + 1;

    m_ox = w / 2 - m_width / 2 + m_minx;
    m_oy = m_height / 2 - h / 2 - m_miny;
}
bool Image::rotated() const
{
    return m_rotated;
}

void Image::setRotated(bool rotated)
{
    m_rotated = rotated;
}

QString Image::getName() const
{
    return m_name;
}

int Image::ox() const
{
    return m_ox;
}

int Image::oy() const
{
    return m_oy;
}

int Image::offsetY() const
{
    return m_offsetY;
}

void Image::setOffsetY(int offsetY)
{
    m_offsetY = offsetY;
}

int Image::offsetX() const
{
    return m_offsetX;
}

void Image::setOffsetX(int offsetX)
{
    m_offsetX = offsetX;
}

