#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tc/project.h"
#include "tc/settings.h"

#include "tc/folders/foldersequence.h"
#include "tc/folders/folder.h"
#include "tc/folders/folderinfo.h"

#include "tc/ui/logwidget.h"
#include "tc/ui/dockwidget.h"
#include "tc/ui/theme.h"
#include "tc/ui/tutorialswidget.h"

#include "tc/tutorials/model.h"
#include "tc/tutorials/tutorial.h"

#include <QCommandLineParser>
#include <QDateTime>
#include <QDebug>

namespace {
const QString MainWindowGroup { "TutCatalog/MainWindow" };
const QString MainWindowGeometryKey { "geometry" };
const QString MainWindowStateKey { "windowState" };
}

MainWindow::MainWindow(const QCommandLineParser &parser, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_tutorials(new tc::tutorials::Model(this))
    , m_folders(new tc::folders::FolderSequence(this))
{
    connect(m_folders, &tc::folders::FolderSequence::loaded, m_tutorials, &tc::tutorials::Model::append);

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

    // docks
    auto logWidget = new LogWidget(this);
    auto logDockWidget = new DockWidget(logWidget, this);
    logDockWidget->setObjectName("logDockWidget");
    addDockWidget(Qt::LeftDockWidgetArea, logDockWidget);
    m_persistents << logWidget;

    auto tutorialsWidget = new TutorialsWidget(this);
    tutorialsWidget->setModel(m_tutorials);
    auto tutorialsDockWidget = new DockWidget(tutorialsWidget, this);
    tutorialsDockWidget->setObjectName("tutorialsDockWidget");
    addDockWidget(Qt::LeftDockWidgetArea, tutorialsDockWidget);
    m_persistents << tutorialsWidget;

    // toolbar
    m_ui->toolBar->setStyleSheet(Theme::MainToolBarStyle);
    m_ui->toolBar->setIconSize(QSize(Theme::MainToolBarIconSize,
                                     Theme::MainToolBarIconSize));

//    m_ui->toolBar->addActions(m_folders->actions());
//    m_ui->toolBar->addSeparator();

    m_ui->toolBar->addAction(tutorialsDockWidget->toggleViewAction());
    m_ui->toolBar->addAction(logDockWidget->toggleViewAction());
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
    loadProject(project);
}

void MainWindow::loadProject(const QString &fileName)
{
    using namespace tc;

    QTime t;
    t.start();

    Project p;
    if (p.readFromFile(fileName)) {
        m_folders->setup(p.folderInfos());
        m_folders->load();
        qDebug() << "loaded" << fileName << "in" << t.elapsed() << "msec";
    } else {
        qDebug() << "loading" << fileName << "failed after" << t.elapsed() << "msec";
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
