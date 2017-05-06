#include "tutorialswidget.h"
#include "pixmap.h"
#include "theme.h"

namespace tc {
namespace ui {

class TutorialsWidgetPrivate : public QObject
{
    Q_DECLARE_PUBLIC(TutorialsWidget)
    TutorialsWidget* const q_ptr { nullptr };

    QList<QAction*> m_dockToolBarAction;
    QList<QAction*> m_appToolBarAction;

    explicit TutorialsWidgetPrivate(TutorialsWidget* ptr) : q_ptr(ptr) {
        setupUi();
    }

    void setupUi() {
        Q_Q(TutorialsWidget);
        q->setWindowTitle(tr("Tutorials"));
        q->setWindowIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE53B", Theme::MainToolBarIconSize, Theme::MainToolBarIconColor));
    }
};

TutorialsWidget::TutorialsWidget(QWidget *parent)
    : DockableWidget(parent)
    , d_ptr(new TutorialsWidgetPrivate(this))
{
}

TutorialsWidget::~TutorialsWidget()
{
}

QList<QAction *> TutorialsWidget::dockToolBarActions() const
{
    Q_D(const TutorialsWidget);
    return d->m_dockToolBarAction;
}

QList<QAction *> TutorialsWidget::appToolBarActions() const
{
    Q_D(const TutorialsWidget);
    return d->m_appToolBarAction;
}

void TutorialsWidget::saveSettings(Settings &settings) const
{
}

void TutorialsWidget::loadSettings(const Settings &settings)
{
}

} // namespace ui
} // namespace tc
