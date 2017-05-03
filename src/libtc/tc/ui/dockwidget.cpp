#include "dockwidget.h"
#include "dockablewidget.h"
#include "theme.h"

#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

namespace tc {
namespace ui {

class DockWidgetPrivate: public QObject
{
    Q_DECLARE_PUBLIC(DockWidget)
    DockWidget* const q_ptr { nullptr };
    DockableWidget* const m_content { nullptr };

    explicit DockWidgetPrivate(DockableWidget* content, DockWidget* ptr)
        : q_ptr(ptr)
        , m_content(content)
    {
        Q_Q(DockWidget);

        QString title = m_content->windowTitle();
        auto tb = new QToolBar;
        tb->addWidget(new QLabel(title));
        tb->setStyleSheet(Theme::DockToolBarStyle);
        tb->setIconSize(QSize(Theme::DockToolBarIconSize,
                              Theme::DockToolBarIconSize));
        tb->addSeparator();
        // TODO: add actions on the dock toolbar
//        tb->addActions(m_widget->dockToolbarActions());

        auto empty = new QWidget();
        empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
        tb->addWidget(empty);

        // TODO: add close dock button
        auto action = new QAction;
//        Constants::CloseLogWindow.setActionProperties(action);
        tb->addAction(action);
        connect(action, &QAction::triggered, q, &QDockWidget::close);

        q->setTitleBarWidget(tb);
        q->setWindowTitle(title);

        // TODO: add close action
//        action = q->toggleViewAction();
//        tc::Constants::ToggleDuplicatesWindow.setActionProperties(action);

        auto container = new QWidget;
        auto layout = new QVBoxLayout(container);
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_content);
        q->setWidget(container);

        auto toggleAction = q->toggleViewAction();
        toggleAction->setText("xxx");
        toggleAction->setIcon(m_content->windowIcon());
    }
};

DockWidget::DockWidget(DockableWidget* content, QWidget *parent)
    : QDockWidget(parent)
    , d_ptr(new DockWidgetPrivate(content, this))
{
}

DockWidget::~DockWidget()
{
}

DockableWidget *DockWidget::content() const
{
    Q_D(const DockWidget);
    return d->m_content;
}

} // namespace ui
} // namespace tc
