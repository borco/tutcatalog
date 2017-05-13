#include "folder.h"
#include "folderinfo.h"
#include "tutorial.h"

#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace tc {
namespace tutorials {

class FolderPrivate : public QObject
{
    Q_DECLARE_PUBLIC(Folder)
    Folder* const q_ptr { nullptr };
    FolderInfo m_info;

    explicit FolderPrivate(Folder* ptr) : q_ptr(ptr) {}

    void setup(const FolderInfo* info) {
        if (info) {
            m_info = *info;
            qDebug() << "- tutorial folder - setup:" << m_info.name();
            if (!m_info.cachePath().isEmpty()) {
                loadCache();
            } else {
                qDebug() << "  no cache defined - cache loading skipped";
            }
        } else {
            m_info.clear();
        }
    }

    QString absolutePath(const QString& path) const {
        return path.isEmpty() ? path : QDir(path).absolutePath();
    }

    void loadCache() {
        Q_Q(Folder);

        auto db = QSqlDatabase::database();
        db.setDatabaseName(m_info.cachePath());
        if (!db.open()) {
            qWarning() << "can't open cache file:" << m_info.cachePath();
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
            t->set_folder(q);
            t->set_isCached(true);
            t->set_isReadOnly(true);

            int row = -1;
            t->set_tableId(query.value(++row).toInt());
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

    void load() {
        qDebug() << "- tutorial folder - load:" << m_info.name();
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

bool Folder::noBackup(const tutorials::Tutorial *tutorial) const
{
    Q_D(const Folder);
    auto noBackupPath = d->m_info.noBackupPath();
    auto currentPath = tutorial->path();
    return !noBackupPath.isEmpty() && currentPath.startsWith(noBackupPath);
}

} // namespace tutorials
} // namespace tc
