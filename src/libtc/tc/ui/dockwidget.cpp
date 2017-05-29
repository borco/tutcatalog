#include "dockwidget.h"
#include "dockablewidget.h"
#include "action.h"
#include "pixmap.h"
#include "theme.h"

#include <QLabel>
#include <QToolBar>
#include <QToolButton>
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

        auto title = m_content->dockToolBarTitle();

        // define the dock toolbar
        auto tb = new QToolBar;
        tb->addWidget(new QLabel(title));
        tb->setStyleSheet(Theme::DockToolBarStyle);
        tb->setIconSize(QSize(Theme::DockToolBarIconSize,
                              Theme::DockToolBarIconSize));
        tb->addSeparator();
        for (auto a: m_content->dockToolBarActions()) {
            if (!a->instantPopup()) {
                tb->addAction(a);
            } else {
                auto button = new QToolButton;
                button->setDefaultAction(a);
                button->setPopupMode(QToolButton::InstantPopup);
                tb->addWidget(button);
            }
        }

        auto empty = new QWidget;
        empty->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
        tb->addWidget(empty);

        auto action = new QAction(this);
        action->setIcon(Pixmap::fromFont(Theme::AwesomeFont, "\uf08d", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        connect(action, &QAction::toggled, [=](bool value) { q->setFloating(!value); });
        connect(q, &DockWidget::topLevelChanged, [=](bool value) {
            const QSignalBlocker blocker(action);
            Q_UNUSED(blocker);
            action->setChecked(!value);
        });
        action->setCheckable(true);
        action->setChecked(true);
        tb->addAction(action);

        action = new QAction(this);
        action->setIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE5CD", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        connect(action, &QAction::triggered, q, &QDockWidget::close);
        tb->addAction(action);

        q->setTitleBarWidget(tb);
        q->setWindowTitle(title);
        q->setAllowedAreas(Qt::AllDockWidgetAreas);

        // embed the content
        auto container = new QWidget;
        auto layout = new QVBoxLayout(container);
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_content);
        q->setWidget(container);

        // define the toggle action
        auto toggleAction = q->toggleViewAction();
        toggleAction->setToolTip(m_content->appToolBarToggleViewToolTip());
        toggleAction->setIcon(m_content->appToolBarToggleViewIcon());
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
