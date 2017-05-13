#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"

#include "tc/project.h"
#include "tc/settings.h"

#include "tc/ui/labeledvalue.h"
#include "tc/ui/logwidget.h"
#include "tc/ui/dockwidget.h"
#include "tc/ui/theme.h"
#include "tc/ui/tutorialswidget.h"

#include "tc/tutorials/collection.h"
#include "tc/tutorials/folderinfo.h"
#include "tc/tutorials/model.h"
#include "tc/tutorials/proxymodel.h"
#include "tc/tutorials/tutorial.h"

#include <QCommandLineParser>
#include <QDateTime>
#include <QDebug>
#include <QDir>

namespace {
const QString MainWindowGroup { "TutCatalog/MainWindow" };
const QString MainWindowGeometryKey { "geometry" };
const QString MainWindowStateKey { "windowState" };
}

MainWindow::MainWindow(const QCommandLineParser &parser, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_tutorials(new tc::tutorials::Model(this))
    , m_folders(new tc::tutorials::Collection(this))
{
    connect(m_folders, &tc::tutorials::Collection::loaded, m_tutorials, &tc::tutorials::Model::append);

    setupUi();
    processCommandLineOption(parser);
}

MainWindow::~MainWindow()
{
    delete m_tutorials;
    delete m_folders;
    delete m_ui;
}

void MainWindow::setupUi()
{
    using namespace tc::ui;

    m_ui->setupUi(this);

    setWindowTitle(MY_APPLICATION);

    // docks
    auto logWidget = new LogWidget(this);
    auto logDockWidget = new DockWidget(logWidget, this);
    logDockWidget->setObjectName("logDockWidget");
    addDockWidget(Qt::LeftDockWidgetArea, logDockWidget);
    m_persistents << logWidget;

    m_tutorialsWidget = new TutorialsWidget(this);
    m_tutorialsWidget->setModel(m_tutorials);
    auto tutorialsDockWidget = new DockWidget(m_tutorialsWidget, this);
    tutorialsDockWidget->setObjectName("tutorialsDockWidget");
    addDockWidget(Qt::LeftDockWidgetArea, tutorialsDockWidget);
    m_persistents << m_tutorialsWidget;

    // toolbar
    m_ui->toolBar->setStyleSheet(Theme::MainToolBarStyle);
    m_ui->toolBar->setIconSize(QSize(Theme::MainToolBarIconSize,
                                     Theme::MainToolBarIconSize));

    m_ui->toolBar->addAction(tutorialsDockWidget->toggleViewAction());
    m_ui->toolBar->addAction(logDockWidget->toggleViewAction());

    setupStatusBar();
}

void MainWindow::setupStatusBar()
{
    auto tutorialsCountLabel = new tc::ui::LabeledValue;
    tutorialsCountLabel->set_label(tr("Tutorials:"));
    tutorialsCountLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    tutorialsCountLabel->setMinimumWidth(140);

    auto proxyModel = m_tutorialsWidget->proxyModel();

    auto updateTutorialsCount = [=]() {
        int totalCount = m_tutorials->rowCount(QModelIndex());
        int shownCount = proxyModel->rowCount(QModelIndex());
        tutorialsCountLabel->set_value(totalCount == shownCount ? QString::number(totalCount) : tr("%1 (of %2)").arg(shownCount).arg(totalCount));
    };

    connect(proxyModel, &QAbstractListModel::rowsInserted, updateTutorialsCount);
    connect(proxyModel, &QAbstractListModel::rowsRemoved, updateTutorialsCount);
    connect(proxyModel, &QAbstractListModel::modelReset, updateTutorialsCount);
    connect(m_tutorials, &QAbstractListModel::rowsInserted, updateTutorialsCount);
    connect(m_tutorials, &QAbstractListModel::rowsRemoved, updateTutorialsCount);
    connect(m_tutorials, &QAbstractListModel::modelReset, updateTutorialsCount);

    m_ui->statusBar->addWidget(tutorialsCountLabel);

    m_ui->statusBar->setStyleSheet(tc::ui::Theme::StatusBarStyle);
}

void MainWindow::processCommandLineOption(const QCommandLineParser &parser)
{
    const QStringList args = parser.positionalArguments();
    QString project = args.size() == 1 ? args[0] : QString();
    QString customIni = parser.value("ini");

    if (!customIni.isEmpty()) {
        tc::Settings::setFileName(customIni);
    }

    loadSettings();

    if (!project.isEmpty()) {
        loadProject(project);
    } else {
        qWarning() << "no project to load";
    }
}

void MainWindow::loadProject(const QString &fileName)
{
    using namespace tc;

    Project p;
    if (p.readFromFile(fileName)) {
        setWindowTitle(QString("%2 - %1").arg(MY_APPLICATION, QDir::toNativeSeparators(fileName)));
        m_folders->setup(p.folderInfos());
        m_folders->startLoad();
    } else {
        qDebug() << "loading" << fileName << "failed";
    }
}

void MainWindow::loadSettings()
{
    tc::Settings settings;
    qDebug() << "loading settings from:" << settings.fileName();
    settings.beginGroup(MainWindowGroup);

    restoreGeometry(settings.value(MainWindowGeometryKey).toByteArray());
    restoreState(settings.value(MainWindowStateKey).toByteArray());

    for(auto p: m_persistents)
        p->loadSettings(settings);

    settings.endGroup();
}

void MainWindow::saveSettings()
{
    tc::Settings settings;
    qDebug() << "saving settings to:" << settings.fileName();
    settings.beginGroup(MainWindowGroup);

    settings.setValue(MainWindowGeometryKey, saveGeometry());
    settings.setValue(MainWindowStateKey, saveState());

    for(auto p: m_persistents)
        p->saveSettings(settings);

    settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}
