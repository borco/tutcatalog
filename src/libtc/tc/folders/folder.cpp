#include "folder.h"
#include "folderinfo.h"

#include "tc/tutorials/tutorial.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace tc {
namespace folders {

class FolderPrivate : public QObject
{
    Q_DECLARE_PUBLIC(Folder)
    Folder* const q_ptr { nullptr };
    FolderInfo m_info;
    bool m_isRefreshing;

    FolderPrivate(Folder* ptr) : q_ptr(ptr) {}

    void setup(const FolderInfo* info) {
        if (info) {
            m_info = *info;
            qDebug() << "- tutorial folder - setup:" << m_info.name();
            if (!info->cachePath().isEmpty()) {
                loadCache(info);
            } else {
                qDebug() << "  no cache defined - cache loading skipped";
            }
        } else {
            m_info.clear();
        }
    }

    void loadCache(const FolderInfo* info) {
        Q_Q(Folder);

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
                        " todo,"
                        " complete,"
                        " rating,"
                        " viewed,"
                        " deleted,"
                        " online,"
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
            t->set_cache(info->cachePath());
            t->set_group(info->name());
            t->set_isCached(true);
            t->set_isReadOnly(true);

            int row = -1;
            t->set_tableId(query.value(++row).toInt());
            t->set_title(query.value(++row).toString());
            t->set_publisher(query.value(++row).toString());
            t->set_authors(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_hasInfo(query.value(++row).toBool());
            t->set_onToDoList(query.value(++row).toBool());
            t->set_isComplete(query.value(++row).toBool());
            t->set_rating(query.value(++row).toInt());
            t->set_isViewed(query.value(++row).toBool());
            t->set_isDeleted(query.value(++row).toBool());
            t->set_isOnline(query.value(++row).toBool());
            t->set_duration(query.value(++row).toString());
            t->set_size(query.value(++row).toInt());
            t->set_path(query.value(++row).toString());
            t->set_levels(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_created(query.value(++row).toDateTime());
            t->set_released(query.value(++row).toDateTime());
            t->set_modified(query.value(++row).toDateTime());
            t->set_learningPaths(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_tags(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_extraTags(query.value(++row).toString().split(",", QString::SkipEmptyParts));

            auto skipBackupPath = info->skipBackupPath();
            t->set_skipBackup(!skipBackupPath.isEmpty() && t->path().startsWith(skipBackupPath));

            emit q->loaded(t);

            if (t->parent() == nullptr) {
                qWarning() << "deleting unwanted tutorial" << t->title();
                t->deleteLater();
            }
        }

        qDebug() << "  loaded" << count << "tutorials from cache";

        db.close();
    }

    void load() {
        qDebug() << "- tutorial folder - load:" << m_info.name();
    }

    void startRefresh() {
        // TODO: startRefresh
    }

    void cancelRefresh() {
        // TODO: cancelRefresh
    }
};

Folder::Folder(QObject *parent)
    : QObject(parent)
    , d_ptr(new FolderPrivate(this))
{
}

Folder::~Folder()
{
}

void Folder::setup(const FolderInfo *info)
{
    Q_D(Folder);
    d->setup(info);
}

void Folder::load()
{
    Q_D(Folder);
    d->load();
}

const FolderInfo *Folder::info() const
{
    Q_D(const Folder);
    return &d->m_info;
}

void Folder::startRefresh()
{
    Q_D(Folder);
    d->startRefresh();
}

void Folder::cancelRefresh()
{
    Q_D(Folder);
    d->cancelRefresh();
}

bool Folder::isRefreshing() const
{
    Q_D(const Folder);
    return d->m_isRefreshing;
}

} // namespace folders
} // namespace tc
