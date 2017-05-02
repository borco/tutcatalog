#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tc/project.h"
#include "tc/folders/foldersequence.h"
#include "tc/folders/folder.h"
#include "tc/folders/folderinfo.h"
#include "tc/ui/constants.h"
#include "tc/ui/logdockwidget.h"

#include <QDateTime>
#include <QDebug>

MainWindow::MainWindow(const QString& fileName, QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_folders(new tc::folders::FolderSequence(this))
{
    setupUi();
    setupFolders(fileName);
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

    // docks
    m_logDock = new LogDockWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, m_logDock);

    // toolbar
    m_ui->toolBar->setStyleSheet(Constants::MainToolBarStyle);
    m_ui->toolBar->setIconSize(QSize(Constants::MainToolBarIconSize,
                                     Constants::MainToolBarIconSize));

    m_ui->toolBar->addActions(m_folders->actions());

    m_ui->toolBar->addSeparator();
    m_ui->toolBar->addAction(m_logDock->toggleViewAction());
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
