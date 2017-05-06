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

namespace tutorials {
class Model;
class ProxyModel;
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
    void processCommandLineOption(const QCommandLineParser& parser);
    void loadProject(const QString& fileName);
    void loadSettings();
    void saveSettings();
    void closeEvent(QCloseEvent* event) override;

    Ui::MainWindow *m_ui { nullptr };
    QVector<tc::Persistent*> m_persistents;

    tc::tutorials::Model* m_tutorials { nullptr };
    tc::folders::FolderSequence* m_folders { nullptr };
};
