#include "logwidget.h"
#include "pixmap.h"
#include "theme.h"

#include <QAction>

namespace tc {
namespace ui {

class LogWidgetPrivate: public QObject
{
    Q_DECLARE_PUBLIC(LogWidget)
    LogWidget* const q_ptr;
    QList<QAction*> m_dockToolBarActions;
    QList<QAction*> m_appToolBarActions;

    explicit LogWidgetPrivate(LogWidget* ptr) : q_ptr(ptr) {
        Q_Q(LogWidget);
        q->setWindowTitle(tr("Messages"));
        q->setWindowIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE8B0", Theme::MainToolBarIconSize, Theme::MainToolBarIconColor));

        auto action = new QAction;
        action->setToolTip(tr("Clear all messages"));
        action->setIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE872", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        m_dockToolBarActions.append(action);

        action = new QAction;
        action->setToolTip(tr("Wrap long lines"));
        action->setIcon(Pixmap::fromFont(Theme::DefaultFont, "\u2BA8", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        m_dockToolBarActions.append(action);
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
