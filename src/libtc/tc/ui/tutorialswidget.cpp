#include "tutorialswidget.h"
#include "delegates.h"
#include "pixmap.h"
#include "theme.h"

#include "tc/settings.h"

#include "tc/tutorials/model.h"
#include "tc/tutorials/proxymodel.h"
#include "tc/tutorials/tutorial.h"

#include <QHeaderView>
#include <QTreeView>
#include <QVBoxLayout>


namespace {
const QString WidgetGroup { "TutorialsWidget/" };
const QString HeaderStateKey { WidgetGroup + "headerState" };
}

namespace tc {
namespace ui {

class TutorialsWidgetPrivate : public QObject
{
    Q_DECLARE_PUBLIC(TutorialsWidget)
    TutorialsWidget* const q_ptr { nullptr };

    QList<QAction*> m_dockToolBarAction;
    QList<QAction*> m_appToolBarAction;

    QTreeView* m_view;

    tutorials::Model* m_model { nullptr };
    tutorials::ProxyModel* m_proxyModel { nullptr };

    explicit TutorialsWidgetPrivate(TutorialsWidget* ptr)
        : q_ptr(ptr)
        , m_proxyModel(new tutorials::ProxyModel(this)) {
        setupUi();
    }

    void setupUi() {
        Q_Q(TutorialsWidget);

        m_view = new QTreeView;
        m_view->setFrameStyle(QFrame::NoFrame);
        m_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
        m_view->setModel(m_proxyModel);
        m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_view->setSortingEnabled(true);

        m_view->setItemDelegateForColumn(tutorials::Model::Rating, new RatingDelegate(this));
        m_view->setItemDelegateForColumn(tutorials::Model::Size, new FileSizeDelegate(this));
        m_view->setItemDelegateForColumn(tutorials::Model::Duration, new DurationDelegate(this));
        m_view->setItemDelegateForColumn(tutorials::Model::FileSizeToDuration, new FileSizeDelegate(this));

        auto layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_view);

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

    void saveSettings(Settings &settings) const {
        settings.setValue(HeaderStateKey, m_view->header()->saveState());
    }

    void loadSettings(const Settings &settings) {
        m_view->header()->restoreState(settings.value(HeaderStateKey).toByteArray());
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
    Q_D(const TutorialsWidget);
    d->saveSettings(settings);
}

void TutorialsWidget::loadSettings(const Settings &settings)
{
    Q_D(TutorialsWidget);
    d->loadSettings(settings);
}

void TutorialsWidget::setModel(tutorials::Model *model)
{
    Q_D(TutorialsWidget);
    d->setModel(model);
}

} // namespace ui
} // namespace tc
