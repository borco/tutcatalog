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

#include <QCommandLineParser>
#include <QDateTime>
#include <QDebug>

namespace {
const QString MainWindowGroup { "TutCatalogWindow" };
const QString MainWindowGeometryKey { MainWindowGroup + "/geometry" };
const QString MainWindowStateKey { MainWindowGroup + "/windowState" };
}

MainWindow::MainWindow(const QCommandLineParser &parser, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_folders(new tc::folders::FolderSequence(this))
{
    setupUi();
    processCommandLineOption(parser);
}

MainWindow::~MainWindow()
{
    delete m_folders;
    delete m_ui;
}

void MainWindow::setupUi()
{
    using namespace tc::ui;

    m_ui->setupUi(this);

    m_ui->centralwidget->hide();

    // docks
    m_logDockWidget = new DockWidget(new LogWidget(this), this);
    m_logDockWidget->setObjectName("logDockWidget");
    addDockWidget(Qt::RightDockWidgetArea, m_logDockWidget);

    // toolbar
    m_ui->toolBar->setStyleSheet(Theme::MainToolBarStyle);
    m_ui->toolBar->setIconSize(QSize(Theme::MainToolBarIconSize,
                                     Theme::MainToolBarIconSize));

    m_ui->toolBar->addActions(m_folders->actions());

    m_ui->toolBar->addSeparator();
    m_ui->toolBar->addAction(m_logDockWidget->toggleViewAction());
}

void MainWindow::setupFolders(const QString &fileName)
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

void MainWindow::processCommandLineOption(const QCommandLineParser &parser)
{
    const QStringList args = parser.positionalArguments();
    QString project = args.size() == 1 ? args[0] : QString();
    QString customIni = parser.value("ini");

    if (!customIni.isEmpty()) {
        tc::Settings::setFileName(customIni);
    }

    loadSettings();
    setupFolders(project);
}

void MainWindow::loadSettings()
{
    tc::Settings settings;
    qDebug() << "loading settings from:" << settings.fileName();
    restoreGeometry(settings.value(MainWindowGeometryKey).toByteArray());
    restoreState(settings.value(MainWindowStateKey).toByteArray());
}

void MainWindow::saveSettings()
{
    tc::Settings settings;
    qDebug() << "saving settings to:" << settings.fileName();
    settings.setValue(MainWindowGeometryKey, saveGeometry());
    settings.setValue(MainWindowStateKey, saveState());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    QMainWindow::closeEvent(event);
}
