#ifndef XMLPARSERTEST_H
#define XMLPARSERTEST_H

#include <QObject>
#include <QTest>
#include <QString>

class XMLParserTest : public QObject
{
    Q_OBJECT
public:
    XMLParserTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void testParserReturnBoolean();
private:
    QString m_xmlFilePath;
};

#endif // XMLPARSERTEST_H
