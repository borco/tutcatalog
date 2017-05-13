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

    CollectionPrivate(Collection* ptr) : q_ptr(ptr) {}

    void clear() {
        qDeleteAll(m_folders);
        m_folders.clear();
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

    void startLoad() {
        static bool isLoading { false };

        if (isLoading) {
            qWarning() << "collection already loading; ignoring start load request...";
            return;
        }

        isLoading = true;

        qDebug() << "collection: start load";

        for(auto folder: m_folders) {
            folder->load();
        }

        isLoading = false;
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
    d->startLoad();
}

} // namespace folders
} // namespace tc
