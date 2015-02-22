#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QVector>
#include <QString>

class FileUtils
{
public:
    static QVector<QString> getAllActorDir(const QString & path);
    static QString getActorName(const QString & fullPath, const QString & importPath);
    static QString getAbsoluteFilePath(const QString & path);
    static QString getActorTextureName(const QString & fullPath, const QString & importPath);
    static QVector<QString> getAllActorTextureFile(const QString & path);
    static bool writeFile(const char * fileName, const char * mode, const char * content, unsigned long size);
    static bool writeFile(const char * fileName, const char * mode, const unsigned char * content, unsigned long size);
    static unsigned char* getFileData(const char* pszFileName, const char* pszMode, unsigned long * pSize);
    static bool hasParentDirectory(const QString & path);
    static bool createParentDirectory(const QString & path);
    static QVector<QString> getAllActorPlistFile(const QString & path);
    static bool exists(const QString & path);
    static bool unlink(const QString & path);
    static bool copyFile(const QString &sourceFileName, const QString &destFileName);
    static QVector<QString> getAllXMLFile(const QString & path);
};

#endif // FILEUTILS_H
