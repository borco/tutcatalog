#include "tutorialswidget.h"
#include "action.h"
#include "delegates.h"
#include "pixmap.h"
#include "theme.h"

#include "tc/settings.h"

#include "tc/tutorials/model.h"
#include "tc/tutorials/proxymodel.h"
#include "tc/tutorials/tutorial.h"

#include <QDebug>
#include <QHeaderView>
#include <QMenu>
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
    struct ColumnInfo {
        int index { -1 };
        Action* hideAction { nullptr };
    };

    Q_DECLARE_PUBLIC(TutorialsWidget)
    TutorialsWidget* const q_ptr { nullptr };

    QList<Action*> m_dockToolBarAction;
    QList<Action*> m_appToolBarAction;

    QTreeView* m_view { nullptr };
    QMap<QString, ColumnInfo> m_columns;
    QMenu* m_hideColumnsMenu { nullptr };

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

        connect(m_view->selectionModel(), &QItemSelectionModel::selectionChanged, this, onSelectionChanged);

        auto layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_view);

        q->setLayout(layout);

        q->setWindowTitle(tr("Tutorials"));
        q->setWindowIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE53B", Theme::MainToolBarIconSize, Theme::MainToolBarIconColor));

        auto action = new Action;
        action->set_instantPopup(true);
        action->setIcon(Pixmap::fromFont(Theme::AwesomeFont, "\uF141", Theme::DockToolBarIconSize, Theme::DockToolBarIconColor));
        m_hideColumnsMenu = new QMenu;
        action->setMenu(m_hideColumnsMenu);

        m_dockToolBarAction.append(action);
    }

    QSet<tutorials::Tutorial*> selectedTutorials() const {
        QSet<tutorials::Tutorial*> selection;
        foreach (QModelIndex index, m_view->selectionModel()->selectedIndexes()) {
            int row = m_proxyModel->mapToSource(index).row();
            selection << m_model->tutorial(row);
        }
        return selection;
    }

    void onSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/) {
        Q_Q(TutorialsWidget);
        q->update_selection(selectedTutorials());
    }

    void setModel(tutorials::Model* model) {
        Q_Q(TutorialsWidget);

        if (model == m_model)
            return;

        m_model = model;
        m_proxyModel->setSourceModel(m_model);
        setColumnHideMenu();
    }

    void saveSettings(Settings &settings) const {
        settings.setValue(HeaderStateKey, m_view->header()->saveState());
    }

    void loadSettings(const Settings &settings) {
        m_view->header()->restoreState(settings.value(HeaderStateKey).toByteArray());
        restoreColumnHideMenuState();
    }

    void setColumnHideMenu() {
        m_hideColumnsMenu->clear();
        m_columns.clear();

        for (int i = 0; i < m_model->columns().size(); ++i) {
            auto text = m_model->columns()[i];
            auto action = new Action;
            action->setText(text);
            action->setCheckable(true);
            action->setChecked(true);
            connect(action, &QAction::toggled, [=](bool value) { m_view->setColumnHidden(i, !value);} );
            m_columns[text] = { i, action};
        }

        auto keys = m_columns.keys();
        keys.sort();

        for(auto k: keys) {
            m_hideColumnsMenu->addAction(m_columns[k].hideAction);
        }
    }

    void restoreColumnHideMenuState() {
        if (!m_model)
            return;

        if (m_columns.size() != m_view->header()->count()) {
            qWarning() << "failed to restore the state of the tutorials view menus";
            return;
        }

        for (auto c: m_columns) {
            c.hideAction->setChecked(!m_view->header()->isSectionHidden(c.index));
        }
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

QList<Action *> TutorialsWidget::dockToolBarActions() const
{
    Q_D(const TutorialsWidget);
    return d->m_dockToolBarAction;
}

QList<Action *> TutorialsWidget::appToolBarActions() const
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

tutorials::ProxyModel *TutorialsWidget::proxyModel() const
{
    Q_D(const TutorialsWidget);
    return d->m_proxyModel;
}

} // namespace ui
} // namespace tc
