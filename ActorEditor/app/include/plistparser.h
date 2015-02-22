#ifndef PLISTPARSER_H
#define PLISTPARSER_H
#include <QString>
#include <QDomElement>
#include <QDomNode>
#include <QMap>

#include "include/framevo.h"

class PlistParser
{
public:
    PlistParser();
    virtual ~PlistParser();

    bool load(const QString & plistPath);
    QMap<QString, FrameVO> getFramesMap() const;
private:
    bool parseFrames(const QDomNode & node);
    bool parseFrameVO(const QDomNode & nameNode, const QDomNode & frameNode, FrameVO * out);
    bool parseSizeString(const QDomElement & element, QPoint * out);
    bool parseRectString(const QDomElement & element, QRect * out);

private:
    QMap<QString, FrameVO> m_framesMap;
};

#endif // PLISTPARSER_H
