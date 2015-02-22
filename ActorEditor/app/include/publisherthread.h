#ifndef PUBLISHERTHREAD_H
#define PUBLISHERTHREAD_H

#include <QThread>
#include <QVector>
#include <QString>

#include "include/publisher.h"

class PublisherThread : public QThread
{
    Q_OBJECT
public:
    PublisherThread();
    ~PublisherThread();
    void setPublisher(Publisher * pub);
    void setPublishFileLists(const QVector<QString> & fileLists);
private:
    PublisherThread(const PublisherThread & pt);
    PublisherThread & operator = (const PublisherThread & pt);

    void run();

signals:
    void update(bool isFinished, unsigned int finishedCount, unsigned int count);

private:
    Publisher           * m_publisher;
    QVector<QString>    m_publishFileLists;
};

#endif // PUBLISHERTHREAD_H
