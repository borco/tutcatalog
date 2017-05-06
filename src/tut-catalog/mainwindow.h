#pragma once

#include <QMainWindow>
#include <QQmlVarPropertyHelpers.h>

class QCommandLineParser;

namespace Ui {
class MainWindow;
}

namespace tc {

class Persistent;

namespace folders {
class FolderSequence;
}

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QCommandLineParser& parser, QWidget *parent = 0);
    ~MainWindow();

private:
    void setupUi();
    void setupFolders(const QString& fileName);
    void processCommandLineOption(const QCommandLineParser& parser);
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent* event) override;

    Ui::MainWindow *m_ui { nullptr };
    tc::folders::FolderSequence* m_folders { nullptr };
    QVector<tc::Persistent*> m_persistents;
};
