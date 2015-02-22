#ifndef SETTINGSVO_H
#define SETTINGSVO_H
#include <QString>

class SettingsVO
{
public:
    SettingsVO();
    ~SettingsVO();

    QString exportPath() const;
    void setExportPath(const QString &exportPath);

    QString importPath() const;
    void setImportPath(const QString &importPath);

    double scale() const;
    void setScale(double scale);

private:
    QString m_importPath;
    QString m_exportPath;
    double  m_scale;
};

#endif // SETTINGSVO_H
