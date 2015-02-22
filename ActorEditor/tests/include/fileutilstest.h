#ifndef FILEUTILSTEST_H
#define FILEUTILSTEST_H

#include <QObject>
#include <QString>
#include <QTest>

class FileUtilsTest : public QObject
{
    Q_OBJECT
public:
    FileUtilsTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void testGetAllActorDirReturnPathLists();
    void testGetActorNameReturnName();
    void testGetActorTextureNameReturnName();
private:
    QString m_sActorPath;
};

#endif // FILEUTILSTEST_H
