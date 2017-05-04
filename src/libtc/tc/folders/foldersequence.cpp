#include "foldersequence.h"
#include "folder.h"
#include "folderinfo.h"

#include "tc/ui/pixmap.h"
#include "tc/ui/theme.h"

#include <QAction>
#include <QDebug>

namespace tc {
namespace folders {

class FolderSequencePrivate : public QObject
{
    Q_DECLARE_PUBLIC(FolderSequence)
    FolderSequence* const q_ptr { nullptr };
    QVector<Folder*> m_folders;
    QList<QAction*> m_actions;
    QAction* m_loadAction { nullptr };
    bool m_isLoading;

    FolderSequencePrivate(FolderSequence* ptr) : q_ptr(ptr) {
        setupActions();
    }

    void clear() {
        qDeleteAll(m_folders);
        m_folders.clear();
    }

    void setupActions() {
        using namespace ui;

        auto action = new QAction;
        action->setIcon(Pixmap::fromFont(Theme::MaterialFont, "\uE5D5", Theme::MainToolBarIconColor, Theme::MainToolBarIconSize));
        action->setToolTip(tr("Sync tutorial folders"));
        action->setCheckable(true);
        connect(action, &QAction::toggled, this, &FolderSequencePrivate::onLoadActionToggled);
        m_loadAction = action;
        m_actions.append(m_loadAction);
    }

    void setup(const QVector<FolderInfo*>& infos) {
        clear();

        for(auto info: infos) {
            auto item = new Folder(this);
            item->setup(info);
            m_folders.append(item);
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

FolderSequence::FolderSequence(QObject *parent)
    : QObject(parent)
    , d_ptr(new FolderSequencePrivate(this))
{
}

FolderSequence::~FolderSequence()
{
}

void FolderSequence::setup(const QVector<FolderInfo *> &infos)
{
    Q_D(FolderSequence);
    d->setup(infos);
}

void FolderSequence::load()
{
    Q_D(FolderSequence);
    d->load();
}

QList<QAction *> FolderSequence::actions() const
{
    Q_D(const FolderSequence);
    return d->m_actions;
}

} // namespace folders
} // namespace tc
