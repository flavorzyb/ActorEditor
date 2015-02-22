#ifndef XMLPARSER_H
#define XMLPARSER_H
#include <QString>
#include <QDomElement>

class XMLParser
{
public:
    XMLParser();
    virtual ~XMLParser();

    bool parser(const QString & xmlFilePath, const QString & prefixPath, double scaled);
    QString getXmlString() const;

private:
    bool parseElement(QDomElement * element, const QString & prefixPath, double scaled);
    void scaledFrame(QDomElement * element, const QString & name, double scaled);

private:
    QString m_xmlString;
};

#endif // XMLPARSER_H
