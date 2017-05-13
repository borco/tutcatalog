#include "collection.h"
#include "folder.h"
#include "folderinfo.h"
#include "tutorial.h"

#include "tc/ui/pixmap.h"
#include "tc/ui/theme.h"

#include <QAction>
#include <QDebug>

namespace tc {
namespace tutorials {

class CollectionPrivate : public QObject
{
    Q_DECLARE_PUBLIC(Collection)
    Collection* const q_ptr { nullptr };
    QVector<Folder*> m_folders;
    QList<QAction*> m_actions;
    QAction* m_loadAction { nullptr };
    bool m_isLoading;

    CollectionPrivate(Collection* ptr) : q_ptr(ptr) {
        setupActions();
    }

    void clear() {
        qDeleteAll(m_folders);
        m_folders.clear();
    }

    void setupActions() {
        using namespace ui;

        auto action = new QAction;
        action->setIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE5D5", Theme::MainToolBarIconSize, Theme::MainToolBarIconColor));
        action->setToolTip(tr("Sync tutorial folders"));
        action->setCheckable(true);
        connect(action, &QAction::toggled, this, &CollectionPrivate::onLoadActionToggled);
        m_loadAction = action;
        m_actions.append(m_loadAction);
    }

    void setup(const QVector<FolderInfo*>& infos) {
        Q_Q(Collection);

        clear();

        for(auto info: infos) {
            auto item = new Folder(this);
            m_folders.append(item);
            connect(item, &Folder::loaded, q, &Collection::loaded);
            item->setup(info);
        }
    }

    void load() {
        m_loadAction->setChecked(true);
    }

    void onLoadActionToggled(bool toggled) {
        if (toggled) {
            startLoad();
        } else {
            cancelLoad();
        }
    }

    void startLoad() {
        if (m_isLoading) {
            qWarning() << "collection already loading; ignoring start load request...";
            return;
        }

        m_isLoading = true;

        qDebug() << "collection: start load";

        for(auto folder: m_folders) {
            folder->load();
        }
    }

    void cancelLoad() {
        if (!m_isLoading) {
            qWarning() << "collection isn't loading; ignoring cancel load request...";
            return;
        }

        m_isLoading = false;

        qDebug() << "collection: cancel load";
    }
};

Collection::Collection(QObject *parent)
    : QObject(parent)
    , d_ptr(new CollectionPrivate(this))
{
}

Collection::~Collection()
{
}

void Collection::setup(const QVector<FolderInfo *> &infos)
{
    Q_D(Collection);
    d->setup(infos);
}

void Collection::startLoad()
{
    Q_D(Collection);
    d->load();
}

QList<QAction *> Collection::actions() const
{
    Q_D(const Collection);
    return d->m_actions;
}

} // namespace folders
} // namespace tc
