#include "include/settingsvo.h"

SettingsVO::SettingsVO():
    m_importPath("")
  , m_exportPath("")
  , m_scale(0)
{
}

SettingsVO::~SettingsVO()
{
}

QString SettingsVO::exportPath() const
{
    return m_exportPath;
}

void SettingsVO::setExportPath(const QString &exportPath)
{
    m_exportPath = exportPath;
}

QString SettingsVO::importPath() const
{
    return m_importPath;
}

void SettingsVO::setImportPath(const QString &importPath)
{
    m_importPath = importPath;
}
double SettingsVO::scale() const
{
    return m_scale;
}

void SettingsVO::setScale(double scale)
{
    m_scale = scale;
}
