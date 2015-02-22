#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QTableView>
#include <QStandardItemModel>
#include <QMap>
#include <QStandardItem>
#include <QProgressBar>
#include <QVector>

#include "include/settingsvo.h"
#include "include/publisherthread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void initUI();
    void initImportSettingsGroup();
    void initExportSettingsGroup();
    void initExport();
    void initAction();
    void loadImpportTextureData(const QString & path);
    void updateExportButtonStatus();
    void loadTextureDataTable(const QString & filterString);
    QVector<QString> getAllSelectedFileLists();

private slots:
    void onSpinBoxValueChanged(double value);
    void onSliderValueChanged(int value);
    void onOpenImportPath();
    void onOpenExportPath();
    void onSelectAll();
    void onCancelAll();
    void onExport();
    void onUpdatePublishStatus(bool isFinished, unsigned int finishedCount, unsigned int count);
    void onFilterTextChanged(QString text);

private:
    QWidget                     * m_wMainWidget;
    QWidget                     * m_wExportWidget;

    QLineEdit                   * m_leFilter;
    QLineEdit                   * m_leImportPath;
    QLineEdit                   * m_leExportPath;

    QPushButton                 * m_pbImportPath;
    QPushButton                 * m_pbExportPath;

    QPushButton                 * m_pbSelectedAll;
    QPushButton                 * m_pbCancelAll;

    QGroupBox                   * m_gbImport;
    QGroupBox                   * m_gbExport;

    QDoubleSpinBox              * m_dsExport;
    QSlider                     * m_sExport;

    QPushButton                 * m_pbExport;
    QPushButton                 * m_pbCancel;

    QTableView                  * m_tvImport;

    SettingsVO                    m_settings;
    QStandardItemModel          * m_simModel;
    QMap<QStandardItem *, QString> m_itemMap;
    QProgressBar                * m_pbFinishedBar;
    PublisherThread               m_publisherThread;
    QVector<QString>              m_textureFileLists;
};

#endif // MAINWINDOW_H
