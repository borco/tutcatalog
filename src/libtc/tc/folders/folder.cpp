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

        QSqlQuery query("SELECT title, publisher, authors FROM tutorials");
        int count { 0 };
        while (query.next()) {
            ++count;
            auto tutorial = new tutorials::Tutorial;
            tutorial->set_folder(info->name());
            tutorial->set_isCached(true);
            tutorial->set_isReadOnly(true);

            tutorial->set_title(query.value(0).toString());
            tutorial->set_publisher(query.value(1).toString());
            tutorial->set_authors(query.value(2).toString().split(",", QString::SkipEmptyParts));

            emit q->loaded(tutorial);

            if (tutorial->parent() == nullptr) {
                qWarning() << "deleting unwanted tutorial" << tutorial->title();
                tutorial->deleteLater();
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
