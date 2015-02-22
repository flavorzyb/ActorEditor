#ifndef PUBLISHERTEST_H
#define PUBLISHERTEST_H

#include <QObject>
#include <QString>
#include <QTest>

#include "include/publisher.h"

class PublisherTest : public QObject
{
    Q_OBJECT
public:
    PublisherTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void testGetAllTextureListsReturnLists();
    void testPublishReturnBoolean();
private:
    QString m_importPath;
    QString m_exportPath;

    Publisher * m_publisher;
};

#endif // PUBLISHERTEST_H
