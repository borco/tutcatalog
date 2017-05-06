#include "logwidget.h"
#include "pixmap.h"
#include "theme.h"

#include "tc/settings.h"

#include <QAction>
#include <QTextEdit>
#include <QVBoxLayout>

#include <iostream>

namespace {
const QString LogWidgetGroup { "LogWidget/" };
const QString ShowDebugMessagesKey { LogWidgetGroup + "showDebugMessages" };
const QString WrapLongLinesKey { LogWidgetGroup + "wrapLongLines" };
}

namespace tc {
namespace ui {

class LogWidgetPrivate: public QObject
{
public:
    ~LogWidgetPrivate() {
        qInstallMessageHandler(0);
    }

private:
    Q_DECLARE_PUBLIC(LogWidget)
    LogWidget* const q_ptr { nullptr };
    QTextEdit* m_edit { nullptr };
    QList<QAction*> m_dockToolBarActions;
    QList<QAction*> m_appToolBarActions;
    QAction* m_showDebugMessagesAction { nullptr };
    QAction* m_wrapLongLinesAction { nullptr };
    bool m_showDebugMessages { true };

    static LogWidgetPrivate* m_instance;

    explicit LogWidgetPrivate(LogWidget* ptr) : q_ptr(ptr) {
        setupUi();
        setupMessageHandler();
    }

    void setupMessageHandler() {
        m_instance = this;
        qInstallMessageHandler(&LogWidgetPrivate::staticMessageHandler);
    }

    static void staticMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        if (m_instance) {
            m_instance->messageHandler(type, context, msg);
        }
    }

    void messageHandler(QtMsgType type, const QMessageLogContext &/*context*/, const QString &msg) {
        std::cout << msg.toStdString() << std::endl;

        QString localMsg;

        switch (type) {
        case QtDebugMsg:
            if (!m_showDebugMessages)
                return;
            localMsg = QString("<span style=\"color: #555;\">%1</span>").arg(msg);
            break;
        case QtInfoMsg:
            localMsg = msg;
            break;
        case QtWarningMsg:
            localMsg = QString("<span style=\"color: #000; background: #ff0;\">&nbsp;WARNING&nbsp;</span>&nbsp;%1").arg(msg);
            break;
        case QtCriticalMsg:
            localMsg = QString("<span style=\"color: #fff; background: #f00;\">&nbsp;CRITICAL&nbsp;</span>&nbsp;%1").arg(msg);
            break;
        case QtFatalMsg:
            abort();
        }
        m_edit->append(localMsg);
    }

    void setupUi() {
        Q_Q(LogWidget);
        q->setWindowTitle(tr("Messages"));
        q->setWindowIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE8B0", Theme::MainToolBarIconSize, Theme::MainToolBarIconColor));

        m_edit = new QTextEdit;
        m_edit->setReadOnly(true);
        m_edit->setFrameStyle(QFrame::NoFrame);

        auto layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_edit);

        q->setLayout(layout);

        auto action = new QAction;
        action->setToolTip(tr("Show debug messages"));
        action->setCheckable(true);
        action->setIcon(Pixmap::fromFont(Theme::AwesomeFont, "\uf188", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        action->setChecked(m_showDebugMessages);
        connect(action, &QAction::toggled, [=](bool value) { m_showDebugMessages = value; });
        m_showDebugMessagesAction = action;
        m_dockToolBarActions.append(action);

        bool defaultWrapText = true;
        action = new QAction;
        action->setToolTip(tr("Wrap long lines"));
        action->setCheckable(true);
        action->setIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE25B", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        action->setChecked(defaultWrapText);
        wrapText(defaultWrapText);
        connect(action, &QAction::toggled, this, &LogWidgetPrivate::wrapText);
        m_wrapLongLinesAction = action;
        m_dockToolBarActions.append(action);

        action = new QAction;
        action->setSeparator(true);
        m_dockToolBarActions.append(action);

        action = new QAction;
        action->setToolTip(tr("Clear all messages"));
        action->setIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE872", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        connect(action, &QAction::triggered, m_edit, &QTextEdit::clear);
        m_dockToolBarActions.append(action);
    }

    void wrapText(bool value) {
        m_edit->setWordWrapMode(value ? QTextOption::WordWrap : QTextOption::NoWrap);
    }

    void saveSettings(Settings &settings) const {
        settings.setValue(ShowDebugMessagesKey, m_showDebugMessagesAction->isChecked());
        settings.setValue(WrapLongLinesKey, m_wrapLongLinesAction->isChecked());
    }

    void loadSettings(const Settings &settings) {
        m_showDebugMessagesAction->setChecked(settings.value(ShowDebugMessagesKey, true).toBool());
        m_wrapLongLinesAction->setChecked(settings.value(WrapLongLinesKey, false).toBool());
    }
};

LogWidgetPrivate* LogWidgetPrivate::m_instance { nullptr };

LogWidget::LogWidget(QWidget *parent)
    : DockableWidget(parent)
    , d_ptr(new LogWidgetPrivate(this))
{
}

LogWidget::~LogWidget()
{
}

QList<QAction *> LogWidget::dockToolBarActions() const
{
    Q_D(const LogWidget);
    return d->m_dockToolBarActions;
}

QList<QAction *> LogWidget::appToolBarActions() const
{
    Q_D(const LogWidget);
    return d->m_appToolBarActions;
}

void LogWidget::saveSettings(Settings &settings) const
{
    Q_D(const LogWidget);
    d->saveSettings(settings);
}

void LogWidget::loadSettings(const Settings &settings)
{
    Q_D(LogWidget);
    d->loadSettings(settings);
}

} // namespace ui
} // namespace tc
