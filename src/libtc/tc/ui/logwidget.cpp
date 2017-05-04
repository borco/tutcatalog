#include "logwidget.h"
#include "pixmap.h"
#include "theme.h"

#include <QAction>
#include <QTextEdit>
#include <QVBoxLayout>

namespace tc {
namespace ui {

class LogWidgetPrivate: public QObject
{
    Q_DECLARE_PUBLIC(LogWidget)
    LogWidget* const q_ptr { nullptr };
    QTextEdit* m_edit { nullptr };
    QList<QAction*> m_dockToolBarActions;
    QList<QAction*> m_appToolBarActions;

    explicit LogWidgetPrivate(LogWidget* ptr) : q_ptr(ptr) {
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
        action->setToolTip(tr("Clear all messages"));
        action->setIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE872", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        connect(action, &QAction::triggered, m_edit, &QTextEdit::clear);
        m_dockToolBarActions.append(action);

        bool defaultWrapText = true;
        action = new QAction;
        action->setToolTip(tr("Wrap long lines"));
        action->setCheckable(true);
        action->setIcon(Pixmap::fromFont(Theme::DefaultFont, "\u2BA8", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        action->setChecked(defaultWrapText);
        wrapText(defaultWrapText);
        connect(action, &QAction::toggled, this, &LogWidgetPrivate::wrapText);
        m_dockToolBarActions.append(action);
    }

    void wrapText(bool value) {
        m_edit->setWordWrapMode(value ? QTextOption::WordWrap : QTextOption::NoWrap);
    }
};

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

} // namespace ui
} // namespace tc
