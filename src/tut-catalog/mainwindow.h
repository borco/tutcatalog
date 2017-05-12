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
class Collection;
}

namespace tutorials {
class Model;
class ProxyModel;
}

namespace ui {
class TutorialsWidget;
}

} // namespace tc

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QCommandLineParser& parser, QWidget *parent = 0);
    ~MainWindow();

private:
    void closeEvent(QCloseEvent* event) override;

    void setupUi();
    void setupStatusBar();
    void processCommandLineOption(const QCommandLineParser& parser);
    void loadProject(const QString& fileName);
    void loadSettings();
    void saveSettings();

    Ui::MainWindow *m_ui { nullptr };
    QVector<tc::Persistent*> m_persistents;

    tc::tutorials::Model* m_tutorials { nullptr };
    tc::folders::Collection* m_folders { nullptr };
    tc::ui::TutorialsWidget* m_tutorialsWidget { nullptr };
};
