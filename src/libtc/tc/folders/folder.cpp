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
                loadCache(info->cachePath());
            } else {
                qDebug() << "  no cache defined - cache loading skipped";
            }
        } else {
            m_info.clear();
        }
    }

    void loadCache(const QString& cacheFileName) {
        Q_Q(Folder);

        auto db = QSqlDatabase::database();
        db.setDatabaseName(cacheFileName);
        if (!db.open()) {
            qWarning() << "can't open cache file:" << cacheFileName;
            return;
        }

        q->m_tutorials->clear();

        QSqlQuery query("SELECT title, publisher, authors FROM tutorials");
        while (query.next()) {
            auto tutorial = new tutorials::Tutorial(this);
            tutorial->set_title(query.value(0).toString());
            tutorial->set_publisher(query.value(1).toString());
            tutorial->set_authors(query.value(2).toString().split(",", QString::SkipEmptyParts));
            q->m_tutorials->append(tutorial);
        }

        qDebug() << "  loaded" << q->m_tutorials->size() << "tutorials from cache";

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
    , m_tutorials(new QQmlObjectListModel<tutorials::Tutorial>(this))
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
