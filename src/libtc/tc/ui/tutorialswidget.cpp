#include "tutorialswidget.h"
#include "pixmap.h"
#include "theme.h"

#include "tc/tutorials/model.h"
#include "tc/tutorials/proxymodel.h"
#include "tc/tutorials/tutorial.h"

#include <QTreeView>
#include <QVBoxLayout>

namespace tc {
namespace ui {

class TutorialsWidgetPrivate : public QObject
{
    Q_DECLARE_PUBLIC(TutorialsWidget)
    TutorialsWidget* const q_ptr { nullptr };

    QList<QAction*> m_dockToolBarAction;
    QList<QAction*> m_appToolBarAction;

    tutorials::Model* m_model { nullptr };
    tutorials::ProxyModel* m_proxyModel { nullptr };

    explicit TutorialsWidgetPrivate(TutorialsWidget* ptr)
        : q_ptr(ptr)
        , m_proxyModel(new tutorials::ProxyModel(this)) {
        setupUi();
    }

    void setupUi() {
        Q_Q(TutorialsWidget);

        auto view = new QTreeView;
        view->setFrameStyle(QFrame::NoFrame);
        view->setSelectionMode(QAbstractItemView::ExtendedSelection);
        view->setModel(m_proxyModel);
        view->setSelectionBehavior(QAbstractItemView::SelectRows);
        view->setSortingEnabled(true);

        auto layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(view);

        q->setLayout(layout);

        q->setWindowTitle(tr("Tutorials"));
        q->setWindowIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE53B", Theme::MainToolBarIconSize, Theme::MainToolBarIconColor));
    }

    void setModel(tutorials::Model* model) {
        if (model == m_model)
            return;

        m_model = model;
        m_proxyModel->setSourceModel(m_model);
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

void TutorialsWidget::setModel(tutorials::Model *model)
{
    Q_D(TutorialsWidget);
    d->setModel(model);
}

} // namespace ui
} // namespace tc
