#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <iostream>

#include "include/fileutils.h"

QVector<QString> FileUtils::getAllActorDir(const QString &path)
{
    QVector<QString> result;

    QDir dir(path);

    QFileInfoList fileList = dir.entryInfoList();

    QFileInfoList::const_iterator iterator = fileList.constBegin();

    for(; iterator != fileList.constEnd(); iterator++)
    {
        QString filename = iterator->fileName();

        if (!filename.startsWith("."))
        {
            if (iterator->isDir())
            {
                if (filename == "texture")
                {
                    result.push_back(iterator->absoluteFilePath());
                }
                else
                {
                    result += getAllActorDir(iterator->absoluteFilePath());
                }
            }
        }
    }

    return result;
}

QString FileUtils::getActorName(const QString &fullPath, const QString &importPath)
{
    if ((fullPath.length() > importPath.length()) &&
            (fullPath.startsWith(importPath)))
    {
        QString subPath = fullPath.right(fullPath.length() - importPath.length() -1);
        int index = subPath.indexOf(QDir::separator());
        if (index != -1)
        {
            return subPath.left(index);
        }
    }

    return "";
}

QString FileUtils::getAbsoluteFilePath(const QString &path)
{
    QFileInfo fileInfo(path);
    return fileInfo.absoluteFilePath();
}

QString FileUtils::getActorTextureName(const QString &fullPath, const QString &importPath)
{
    if ((fullPath.length() > importPath.length()) &&
            (fullPath.startsWith(importPath)))
    {
        QString subPath = fullPath.right(fullPath.length() - importPath.length() -1);
        QString searchStr = "texture";
        searchStr += QDir::separator();

        return subPath.replace(searchStr, "");
    }

    return "";
}

QVector<QString> FileUtils::getAllActorTextureFile(const QString &path)
{
    QVector<QString> result;
    QDir dir(path);
    QFileInfoList fileInfoLists = dir.entryInfoList();
    QFileInfoList::const_iterator iterator = fileInfoLists.constBegin();

    for(; iterator != fileInfoLists.constEnd(); iterator++)
    {
        QString filename = iterator->fileName();

        if (!filename.startsWith("."))
        {
            if (iterator->isDir())
            {
                result += getAllActorTextureFile(iterator->absoluteFilePath());
            }
            else if (iterator->isFile())
            {
                QString ext = iterator->completeSuffix();
                QString extLower = ext.toLower();
                if (extLower.endsWith("png"))
                {
                    result.push_back(iterator->absoluteFilePath());
                }
            }
            else
            {
                //
            }
        }
    }

    return result;
}
#define WRITE_FILE_DATA(filename, mode, contentType, content, size) \
    if (!(fileName && mode && content)) \
    { \
        return false; \
    } \
    \
    FILE * fp = fopen(fileName, mode); \
    if (fp) \
    { \
        fwrite(content, sizeof(contentType), size, fp); \
        fclose(fp); \
    \
        return true; \
    } \

bool FileUtils::writeFile(const char *fileName, const char *mode, const unsigned char *content, unsigned long size)
{
    WRITE_FILE_DATA(filename, mode, unsigned char, content, size)

            return false;
}

unsigned char *FileUtils::getFileData(const char *pszFileName, const char *pszMode, unsigned long *pSize)
{
    unsigned char * pBuffer = NULL;
    *pSize = 0;

    if (pszFileName == NULL || pSize == NULL || pszMode == NULL)
    {
        return NULL;
    }

    // read the file from hardware
    FILE *fp = fopen(pszFileName, pszMode);

    if (!fp)
    {
        return NULL;
    }

    fseek(fp,0,SEEK_END);
    *pSize = ftell(fp);
    fseek(fp,0,SEEK_SET);
    pBuffer = new unsigned char[*pSize];
    *pSize = fread(pBuffer,sizeof(unsigned char), *pSize,fp);
    fclose(fp);

    return pBuffer;
}

bool FileUtils::writeFile(const char *fileName, const char *mode, const char *content, unsigned long size)
{
    WRITE_FILE_DATA(filename, mode, char, content, size)

    return false;
}

static QString getParentPath(const QString & path)
{
    QFileInfo fileInfo(path);

    return fileInfo.absolutePath();
}


bool FileUtils::hasParentDirectory(const QString & path)
{
    QString parentPath = getParentPath(path);

    if (parentPath.length() > 0)
    {
        QDir parentDir(parentPath);
        return parentDir.exists();
    }

    return false;
}

bool FileUtils::createParentDirectory(const QString & path)
{
    QString parentPath = getParentPath(path);
    QDir parentDir;
    return parentDir.mkpath(parentPath);
}

QVector<QString> FileUtils::getAllActorPlistFile(const QString &path)
{
    QVector<QString> result;
    QDir dir(path);
    QFileInfoList fileInfoLists = dir.entryInfoList();
    QFileInfoList::const_iterator iterator = fileInfoLists.constBegin();

    for(; iterator != fileInfoLists.constEnd(); iterator++)
    {
        QString filename = iterator->fileName();

        if (!filename.startsWith("."))
        {
            if (iterator->isDir())
            {
                result += getAllActorPlistFile(iterator->absoluteFilePath());
            }
            else if (iterator->isFile())
            {
                QString ext = iterator->completeSuffix();
                QString extLower = ext.toLower();
                if (extLower.endsWith("plist"))
                {
                    result.push_back(iterator->absoluteFilePath());
                }
            }
            else
            {
                //
            }
        }
    }

    return result;
}

bool FileUtils::exists(const QString &path)
{
    QFile file(path);
    return file.exists();
}

bool FileUtils::unlink(const QString &path)
{
    QFile file(path);
    if (file.exists())
    {
        return file.remove();
    }

    return true;
}

bool FileUtils::copyFile(const QString &sourceFileName, const QString &destFileName)
{
    FileUtils::createParentDirectory(destFileName);
    unlink(destFileName);
    return QFile::copy(sourceFileName, destFileName);
}

QVector<QString> FileUtils::getAllXMLFile(const QString &path)
{
    QVector<QString> result;
    QDir dir(path);
    QFileInfoList fileInfoLists = dir.entryInfoList();
    QFileInfoList::const_iterator iterator = fileInfoLists.constBegin();

    for(; iterator != fileInfoLists.constEnd(); iterator++)
    {
        QString filename = iterator->fileName();

        if (!filename.startsWith("."))
        {
            if (iterator->isDir())
            {
                result += getAllXMLFile(iterator->absoluteFilePath());
            }
            else if (iterator->isFile())
            {
                QString ext = iterator->completeSuffix();
                QString extLower = ext.toLower();
                if (extLower.endsWith("xml"))
                {
                    result.push_back(iterator->absoluteFilePath());
                }
            }
            else
            {
                //
            }
        }
    }

    return result;
}
