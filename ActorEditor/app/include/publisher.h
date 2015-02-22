#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <QObject>
#include <QVector>
#include <QString>

#include "include/settingsvo.h"
#include "include/image.h"

class Publisher : public QObject
{
    Q_OBJECT
public:
    explicit Publisher(const SettingsVO & svo);
    ~Publisher();
    QVector<QString> getAllTextureLists();
    bool publish(const QVector<QString> & fileLists);

    inline bool isFinished() const { return m_isFinished;}
    inline unsigned int getCount() const { return m_count;}
    inline unsigned int getFinishedCount() const { return m_finishedCount;}

private:
    bool createTexture(const QString & path);
    int findMaxRectSize(const QVector<Image *> & imageLists, int size);
    bool createNewImage(QImage * img, const QVector<Image *> & imageLists, int size);
    QString createPlist(const QVector<Image *> & imageLists, int size, const QString & textureName);
    bool copyXmlFile(const QString & path);
    bool saveToPVR(const QImage & image, const QString & filePath);

private:
    SettingsVO  m_settings;
    bool        m_isFinished;
    unsigned int m_count;
    unsigned int m_finishedCount;
};

#endif // PUBLISHER_H
