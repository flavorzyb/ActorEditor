#include <QGridLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QStandardItem>
#include <QHeaderView>
#include <QDir>
#include <QMessageBox>
#include <QtCore>
#include <QtAlgorithms>

#include "include/mainwindow.h"
#include "include/utils.h"
#include "include/fileutils.h"

MainWindow::MainWindow():
    QMainWindow(NULL)
  , m_publisherThread()
{
    initImportSettingsGroup();
    initExportSettingsGroup();
    initExport();

    initUI();
    initAction();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initUI()
{
    setMinimumWidth(820);
    setMinimumHeight(700);

    setWindowTitle(tr("Actor动画导出工具"));

    m_wMainWidget = new QWidget;
    setCentralWidget(m_wMainWidget);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_gbImport);
    mainLayout->addWidget(m_gbExport);
    mainLayout->addWidget(m_wExportWidget);

    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(30);

    m_wMainWidget->setLayout(mainLayout);

    updateExportButtonStatus();

    // 居中显示
    if (!isFullScreen())
    {
        Utils::center(this);
    }
}

void MainWindow::initImportSettingsGroup()
{
    m_gbImport = new QGroupBox;
    m_gbImport->setTitle(tr("导入设置"));

    QGridLayout * layout = new QGridLayout;

    QLabel * label = new QLabel;
    label->setText("导入目录");
    label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    m_leImportPath = new QLineEdit;
    m_leImportPath->setText(tr("请设置导入目录"));
    m_leImportPath->setReadOnly(true);

    m_pbImportPath = new QPushButton;
    m_pbImportPath->setText("选择");
    m_pbImportPath->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QLabel * lbFilter = new QLabel;
    lbFilter->setText("过滤选择");

    m_leFilter  = new QLineEdit;
    m_leFilter->setEnabled(false);

    m_tvImport = new QTableView;
    m_tvImport->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_tvImport->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tvImport->resizeColumnsToContents();

    m_simModel = new QStandardItemModel;
    m_simModel->setHorizontalHeaderItem(0, new QStandardItem("选择"));
    m_simModel->setHorizontalHeaderItem(1, new QStandardItem("角色"));
    m_simModel->setHorizontalHeaderItem(2, new QStandardItem("路径"));

    m_tvImport->setModel(m_simModel);
    m_tvImport->setColumnWidth(0, 60);
    m_tvImport->setColumnWidth(1, 140);
    m_tvImport->setColumnWidth(2, 550);

    m_pbSelectedAll = new QPushButton;
    m_pbSelectedAll->setText("全选");
    m_pbSelectedAll->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_pbCancelAll = new QPushButton;
    m_pbCancelAll->setText("反选");
    m_pbCancelAll->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QWidget * selectContenter = new QWidget;
    QHBoxLayout * selectLayout  = new QHBoxLayout;
    selectLayout->addWidget(m_pbSelectedAll);
    selectLayout->addWidget(m_pbCancelAll);
    selectContenter->setLayout(selectLayout);

    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 7);
    layout->setColumnStretch(2, 2);

    layout->addWidget(label, 0, 0);
    layout->addWidget(m_leImportPath, 0, 1);
    layout->addWidget(m_pbImportPath, 0, 2);

    layout->addWidget(lbFilter, 1, 0);
    layout->addWidget(m_leFilter, 1, 1);

    layout->addWidget(m_tvImport, 2, 0, 1, 3);

    layout->addWidget(selectContenter, 3, 0, 1, 3);

    m_gbImport->setLayout(layout);
    m_gbImport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MainWindow::initExportSettingsGroup()
{
    m_gbExport = new QGroupBox;
    m_gbExport->setTitle(tr("导出设置"));

    QGridLayout * layout = new QGridLayout;

    QLabel * lbProgress = new QLabel;
    lbProgress->setText("导出进度");
    lbProgress->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    m_pbFinishedBar = new QProgressBar;
    m_pbFinishedBar->setMinimum(0);
    m_pbFinishedBar->setMaximum(100);
    m_pbFinishedBar->setValue(0);

    QLabel * lbExport = new QLabel;
    lbExport->setText("导出目录");
    lbExport->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    m_leExportPath = new QLineEdit;
    m_leExportPath->setText(tr("请设置导出目录"));
    m_leExportPath->setReadOnly(true);

    m_pbExportPath = new QPushButton;
    m_pbExportPath->setText("选择");
    m_pbExportPath->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QLabel *lbScale = new QLabel;
    lbScale->setText(tr("缩放"));
    lbScale->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);

    m_sExport = new QSlider;
    m_sExport->setOrientation(Qt::Horizontal);
    m_sExport->setMinimum(1);
    m_sExport->setMaximum(100);
    m_sExport->setValue(50);
    m_sExport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    m_dsExport = new QDoubleSpinBox;
    m_dsExport->setMinimum(0.01);
    m_dsExport->setMaximum(1.0);
    m_dsExport->setSingleStep(0.01);
    m_dsExport->setValue(0.5);
    m_dsExport->setAlignment(Qt::AlignRight);

    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 7);
    layout->setColumnStretch(2, 2);

    layout->addWidget(lbProgress, 0, 0);
    layout->addWidget(m_pbFinishedBar, 0, 1);

    layout->addWidget(lbExport, 1, 0);
    layout->addWidget(m_leExportPath, 1, 1);
    layout->addWidget(m_pbExportPath, 1, 2);


    layout->addWidget(lbScale, 2, 0);
    layout->addWidget(m_sExport, 2, 1);
    layout->addWidget(m_dsExport, 2, 2);

    m_settings.setScale(0.5);
    m_gbExport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_gbExport->setLayout(layout);
}

void MainWindow::initExport()
{
    m_wExportWidget = new QWidget;

    QGridLayout * layout = new QGridLayout;

    m_pbExport = new QPushButton;
    m_pbExport->setText(tr("发布"));
    m_pbExport->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_pbCancel = new QPushButton;
    m_pbCancel->setText(tr("关闭"));
    m_pbCancel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout->setColumnStretch(0, 5);
    layout->setColumnStretch(1, 5);

    layout->addWidget(m_pbExport, 0, 0);
    layout->addWidget(m_pbCancel, 0, 1);

    m_wExportWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_wExportWidget->setLayout(layout);
}

void MainWindow::initAction()
{
    connect(m_sExport, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));

    connect(m_dsExport, SIGNAL(valueChanged(double)), this, SLOT(onSpinBoxValueChanged(double)));

    connect(m_pbImportPath, SIGNAL(clicked()), this, SLOT(onOpenImportPath()));

    connect(m_pbExportPath, SIGNAL(clicked()), this, SLOT(onOpenExportPath()));

    connect(m_pbCancel, SIGNAL(clicked()), this, SLOT(close()));

    connect(m_pbSelectedAll, SIGNAL(clicked()), this, SLOT(onSelectAll()));
    connect(m_pbCancelAll, SIGNAL(clicked()), this, SLOT(onCancelAll()));

    connect(m_pbExport, SIGNAL(clicked()), this, SLOT(onExport()));
    connect(&m_publisherThread, SIGNAL(update(bool,uint,uint)), this, SLOT(onUpdatePublishStatus(bool,uint,uint)), Qt::QueuedConnection);

    connect(m_leFilter, SIGNAL(textChanged(QString)), this, SLOT(onFilterTextChanged(QString)));
}

void MainWindow::loadImpportTextureData(const QString &path)
{
    QString fullPath = FileUtils::getAbsoluteFilePath(path);
    m_textureFileLists = FileUtils::getAllActorDir(fullPath);
    qSort(m_textureFileLists);

    m_itemMap.clear();
    loadTextureDataTable("");

    disconnect(m_leFilter, SIGNAL(textChanged(QString)), this, 0);
    m_leFilter->clear();
    m_leFilter->setEnabled(m_textureFileLists.size() > 0);
    connect(m_leFilter, SIGNAL(textChanged(QString)), this, SLOT(onFilterTextChanged(QString)));
}

void MainWindow::updateExportButtonStatus()
{
    if ((m_settings.exportPath().length() > 0) && (m_settings.importPath().length() > 0) && (m_simModel->rowCount() > 0))
    {
        m_pbExport->setEnabled(true);
    }
    else
    {
        m_pbExport->setEnabled(false);
    }
}

static QMap<QString, bool> convertToMap(const QVector<QString> & fileLists)
{
    QMap<QString, bool> result;
    QVector<QString>::const_iterator iterator = fileLists.constBegin();
    for (; iterator != fileLists.constEnd(); iterator++)
    {
        result.insert(*iterator, false);
    }

    return result;
}

void MainWindow::loadTextureDataTable(const QString &filterString)
{
    QString fullPath = FileUtils::getAbsoluteFilePath(m_settings.importPath());
    QVector<QString> selectedFileLists = getAllSelectedFileLists();

    QMap<QString, bool> selectedFileMap = convertToMap(selectedFileLists);

    QVector<QString>::const_iterator iterator = m_textureFileLists.constBegin();
    int index = 0;
    m_itemMap.clear();
    m_simModel->removeRows(0, m_simModel->rowCount());
    // 添加未选择的在前面
    for (; iterator != m_textureFileLists.constEnd(); iterator++)
    {
        QString actorName = FileUtils::getActorName(*iterator, fullPath);
        bool isAdd = true;

        if (filterString.length() > 0)
        {
            isAdd = (actorName.indexOf(filterString) != -1);
        }

        if (isAdd && (selectedFileMap.contains(*iterator) == false))
        {
            QStandardItem * si = new QStandardItem("");
            si->setCheckable(true);
            si->setCheckState(Qt::Unchecked);
            m_itemMap.insert(si, *iterator);

            m_simModel->setItem(index, 0, si);
            m_simModel->setItem(index, 1, new QStandardItem(actorName));
            m_simModel->setItem(index, 2, new QStandardItem(*iterator));
            index++;
        }
    }

    // 将已选择的放后面
    iterator = selectedFileLists.constBegin();
    for (; iterator != selectedFileLists.constEnd(); iterator++)
    {
        QString actorName = FileUtils::getActorName(*iterator, fullPath);
        QStandardItem * si = new QStandardItem("");
        si->setCheckable(true);
        si->setCheckState(Qt::Checked);
        m_itemMap.insert(si, *iterator);

        m_simModel->setItem(index, 0, si);
        m_simModel->setItem(index, 1, new QStandardItem(actorName));
        m_simModel->setItem(index, 2, new QStandardItem(*iterator));
        index++;
    }
}

QVector<QString> MainWindow::getAllSelectedFileLists()
{
    QVector<QString> result;
    QMap<QStandardItem *, QString>::const_iterator iterator = m_itemMap.constBegin();
    for(; iterator != m_itemMap.constEnd(); iterator++)
    {
        if (iterator.key()->checkState() == Qt::Checked)
        {
            result.push_back(iterator.value());
        }
    }

    qSort(result);
    return result;
}

void MainWindow::onSpinBoxValueChanged(double value)
{
    disconnect(m_sExport, SIGNAL(valueChanged(int)), this, 0);
    m_sExport->setValue(value * 100);
    connect(m_sExport, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));

    m_settings.setScale(value);
}

void MainWindow::onSliderValueChanged(int value)
{
    disconnect(m_dsExport, SIGNAL(valueChanged(double)), this, 0);
    m_dsExport->setValue(value * 0.01f);
    connect(m_dsExport, SIGNAL(valueChanged(double)), this, SLOT(onSpinBoxValueChanged(double)));

    m_settings.setScale(value * 0.01f);
}

void MainWindow::onOpenImportPath()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("选择导入目录"));
    m_settings.setImportPath(dirPath);
    if (dirPath.length() > 0)
    {
        m_leImportPath->setText(dirPath);
        loadImpportTextureData(dirPath);
    }

    updateExportButtonStatus();
}

void MainWindow::onOpenExportPath()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("选择导出目录"));
    if (dirPath.length() > 0)
    {
        m_leExportPath->setText(dirPath);
    }

    m_settings.setExportPath(dirPath);
    updateExportButtonStatus();
}

void MainWindow::onSelectAll()
{
    QMap<QStandardItem *, QString>::const_iterator iterator = m_itemMap.constBegin();
    for(; iterator != m_itemMap.constEnd(); iterator++)
    {
        iterator.key()->setCheckState(Qt::Checked);
    }
}

void MainWindow::onCancelAll()
{
    QMap<QStandardItem *, QString>::const_iterator iterator = m_itemMap.constBegin();
    for(; iterator != m_itemMap.constEnd(); iterator++)
    {
        iterator.key()->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::onExport()
{
    QVector<QString> fileLists = getAllSelectedFileLists();

    Publisher * pub = new Publisher(m_settings);
    m_publisherThread.setPublisher(pub);
    m_publisherThread.setPublishFileLists(fileLists);
    m_publisherThread.start();

    m_pbExport->setEnabled(false);
}

void MainWindow::onUpdatePublishStatus(bool isFinished, unsigned int finishedCount, unsigned int count)
{
    m_pbFinishedBar->setValue(finishedCount);
    m_pbFinishedBar->setMaximum(count);

    if (m_publisherThread.isFinished())
    {
        if ((false == isFinished) && (finishedCount < count))
        {
            QMessageBox mb(this);
            mb.setWindowTitle("错误");
            mb.setText(tr("请先正确设置导入目录或导出目录"));
            mb.exec();
        }
        else
        {
            QMessageBox mb(this);
            mb.setWindowTitle("成功");
            char msg[256] = {0};
            sprintf(msg, "成功导出 %d 个动画", count);
            mb.setText(msg);
            mb.exec();
        }
        updateExportButtonStatus();
    }
}

void MainWindow::onFilterTextChanged(QString text)
{
    QString filterStr = text.trimmed();
    loadTextureDataTable(filterStr);
}
