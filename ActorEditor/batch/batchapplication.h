#ifndef BATCHAPPLICATION_H
#define BATCHAPPLICATION_H

#include <QCoreApplication>
#include <QVector>
#include "include/framevo.h"

class BatchApplication : public QCoreApplication
{
public:
    explicit BatchApplication(int argc, char *argv[]);
    virtual ~BatchApplication();
    int run();

private:
    bool runActor();
    bool runEfx();
    bool resizeImage(const QString & fullPath, const FrameVO & fvo);
    bool runResizeImage(const QString & importPath, const QString & exportPath);
    bool checkAllXmlFile(const QString & fullPath);
    QVector<QString> getAllErrorXmlFile(const QString & fullPath);

    void fixedAllXmlFile(const QVector<QString> & xmlFileLists);
};

#endif // BATCHAPPLICATION_H
