#ifndef PLISTPARSERTEST_H
#define PLISTPARSERTEST_H

#include <QObject>
#include <QString>
#include <QTest>

class PlistParserTest : public QObject
{
    Q_OBJECT
public:
    PlistParserTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void testLoadReturnBoolean();

private:
    QString m_plistPath;
};

#endif // PLISTPARSERTEST_H
