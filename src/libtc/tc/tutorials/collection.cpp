#include "collection.h"
#include "folderinfo.h"
#include "tutorial.h"

#include "tc/ui/pixmap.h"
#include "tc/ui/theme.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace tc {
namespace tutorials {

class CollectionPrivate : public QObject
{
    Q_DECLARE_PUBLIC(Collection)
    Collection* const q_ptr { nullptr };
    QVector<FolderInfo*> m_infos;

    CollectionPrivate(Collection* ptr) : q_ptr(ptr) {}

    void clear() {
        qDeleteAll(m_infos);
        m_infos.clear();
    }

    QString absolutePath(const QString& path) const {
        return path.isEmpty() ? path : QDir(path).absolutePath();
    }

    void setup(const QVector<FolderInfo*>& infos) {
        clear();

        for(auto info: infos) {
            auto i = new FolderInfo(this);
            *i = *info;
            m_infos << i;
        }
    }

    void loadCache(FolderInfo* info) {
        Q_Q(Collection);

        auto db = QSqlDatabase::database();
        db.setDatabaseName(info->cachePath());
        if (!db.open()) {
            qWarning() << "can't open cache file:" << info->cachePath();
            return;
        }

        QSqlQuery query("SELECT"
                        " id,"
                        " title,"
                        " publisher,"
                        " authors,"
                        " has_info,"
                        " has_checksum,"
                        " todo,"
                        " keep,"
                        " complete,"
                        " rating,"
                        " viewed,"
                        " deleted,"
                        " online,"
                        " no_backup,"
                        " duration,"
                        " size,"
                        " path,"
                        " levels,"
                        " created,"
                        " released,"
                        " modified,"
                        " learning_paths,"
                        " tags,"
                        " extra_tags"
                        " FROM tutorials");
        int count { 0 };

        while (query.next()) {
            ++count;
            auto t = new tutorials::Tutorial;
            t->set_folderInfo(info);
            t->set_isCached(true);
            t->set_isReadOnly(true);

            int row = -1;
            t->set_index(query.value(++row).toInt());
            t->set_title(query.value(++row).toString());
            t->set_publisher(query.value(++row).toString());
            t->set_authors(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_hasInfo(query.value(++row).toBool());
            t->set_hasChecksum(query.value(++row).toBool());
            t->set_onToDoList(query.value(++row).toBool());
            t->set_onKeepList(query.value(++row).toBool());
            t->set_isComplete(query.value(++row).toBool());
            t->set_rating(query.value(++row).toInt());
            t->set_isViewed(query.value(++row).toBool());
            t->set_isDeleted(query.value(++row).toBool());
            t->set_isOnline(query.value(++row).toBool());
            t->set_noBackup(query.value(++row).toBool());
            t->set_duration(query.value(++row).toInt());
            t->set_size(query.value(++row).value<qint64>());
            // t->set_size(query.value(++row).toInt());
            t->set_path(absolutePath(query.value(++row).toString()));
            t->set_levels(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_created(query.value(++row).toDateTime());
            t->set_released(query.value(++row).toDateTime());
            t->set_modified(query.value(++row).toDateTime());
            t->set_learningPaths(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_tags(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_extraTags(query.value(++row).toString().split(",", QString::SkipEmptyParts));

            emit q->loaded(t);

            if (t->parent() == nullptr) {
                qWarning() << "deleting unwanted tutorial" << t->title();
                t->deleteLater();
            }
        }

        qDebug() << "  loaded" << count << "tutorials from cache";

        db.close();
    }

    void startLoad() {
        static bool isLoading { false };

        if (isLoading) {
            qWarning() << "collection already loading; ignoring start load request...";
            return;
        }

        isLoading = true;

        qDebug() << "collection: start load";

        for(auto info: m_infos) {
            loadCache(info);
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
