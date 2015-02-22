#include "include/publisherthread.h"

class _PubThread : public QThread
{
public:
    explicit _PubThread(Publisher * pub, const QVector<QString> & fileLists);

private:
    void run();

private:
    Publisher * m_pub;
    QVector<QString> m_fileLists;
};

_PubThread::_PubThread(Publisher *pub, const QVector<QString> &fileLists):
    m_pub(pub)
  , m_fileLists(fileLists)
{
}

void _PubThread::run()
{
    if (m_pub != NULL)
    {
        m_pub->publish(m_fileLists);
    }
}

PublisherThread::PublisherThread():
    m_publisher(NULL)
  , m_publishFileLists()
{
}

PublisherThread::~PublisherThread()
{
    if (m_publisher != NULL)
    {
        delete m_publisher;
    }
}

void PublisherThread::setPublisher(Publisher *pub)
{
    if (m_publisher != NULL)
    {
        delete m_publisher;
    }

    m_publisher = pub;
}

void PublisherThread::setPublishFileLists(const QVector<QString> &fileLists)
{
    m_publishFileLists = fileLists;
}

void PublisherThread::run()
{
    if (NULL == m_publisher)
    {
        return ;
    }

    _PubThread pub(m_publisher, m_publishFileLists);
    pub.start();

    while(pub.isFinished() == false)
    {
        update(m_publisher->isFinished(), m_publisher->getFinishedCount(), m_publisher->getCount());
        QThread::usleep(500);
    }

    update(m_publisher->isFinished(), m_publisher->getFinishedCount(), m_publisher->getCount());
}
