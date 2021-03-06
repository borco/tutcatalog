#include "collection.h"

#include "cachedfile.h"
#include "folderinfo.h"
#include "tutorial.h"

#include "tc/ui/pixmap.h"
#include "tc/ui/theme.h"

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextCursor>
#include <QThread>
#include <QTimer>

namespace tc {
namespace tutorials {

typedef QList<FolderInfo*> Infos;
typedef QList<Tutorial*> Tutorials;

class LoaderThread : public QThread
{
    Q_OBJECT

public:
    explicit LoaderThread(Collection* collection, Infos& infos)
        : m_infos(infos)
        , m_collection(collection) {}

signals:
    void warning(QString message);
    void info(QString message);
    void debug(QString message);

private:
    void run() override {
        QTime t;
        t.start();

        for(auto info: m_infos) {
            auto db = QSqlDatabase::database();
            db.setDatabaseName(info->cachePath());

            if (!db.open()) {
                emit warning(QString("can't open cache file: \"%1\"").arg(info->cachePath()));
                continue;
            }

            loadCache(info);
            db.close();
        }

        emit info(QString("finished loading data in %1 sec").arg(t.elapsed() / 1000., 0, 'f', 2));
    }

    QString absolutePath(const QString& path) const {
        return path.isEmpty() ? path : QDir(path).absolutePath();
    }

    void loadCache(FolderInfo* info) {
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
                        " extra_tags,"
                        " url"
                        " FROM tutorials");
        int count { 0 };

        Tutorials items;

        while (query.next()) {
            ++count;
            auto t = new tutorials::Tutorial;
            t->set_collection(m_collection);
            t->set_folderInfo(info);
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
            t->set_released(query.value(++row).toString());
            t->set_modified(query.value(++row).toDateTime());
            t->set_learningPaths(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_tags(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_extraTags(query.value(++row).toString().split(",", QString::SkipEmptyParts));
            t->set_url(query.value(++row).toString());

            t->moveToThread(m_collection->thread());
            items << t;

        }

        emit m_collection->loaded(items);
        emit debug(QString("  loaded %1 tutorials from cache: \"%2\"").arg(count).arg(info->cachePath()));
    }

    Infos& m_infos;
    Collection* m_collection;
};

class CollectionPrivate : public QObject
{
    Q_DECLARE_PUBLIC(Collection)
    Collection* const q_ptr { nullptr };
    Infos m_infos;

    CollectionPrivate(Collection* ptr) : q_ptr(ptr) {}

    void clear() {
        qDeleteAll(m_infos);
        m_infos.clear();
    }

    void setup(const Infos& infos) {
        clear();

        for(auto info: infos) {
            auto i = new FolderInfo(this);
            *i = *info;
            m_infos << i;
        }
    }

    QSqlDatabase openDb(const QString& path) const {
        auto db = QSqlDatabase::database();
        db.setDatabaseName(path);

        if (!db.open()) {
            qWarning() << "can't open cache file:" << path;
            return QSqlDatabase();
        }

        return db;
    }

    void startLoad() {
        Q_Q(Collection);
        static LoaderThread* loader { nullptr };

        if (loader) {
            qWarning() << "collection already loading; ignoring start load request...";
            return;
        }

        loader = new LoaderThread(q, m_infos);
        connect(loader, &LoaderThread::finished, [=](){
            loader->deleteLater();
            loader = nullptr;
        });
        connect(loader, &LoaderThread::debug, this, [](QString message){ qDebug().noquote() << message; });
        connect(loader, &LoaderThread::info, this, [](QString message){ qInfo().noquote() << message; });
        connect(loader, &LoaderThread::warning, this, [](QString message){ qWarning().noquote() << message; });

        QMetaObject::invokeMethod(loader, "start", Qt::QueuedConnection);
    }

    CachedFiles cachedFiles(const Tutorial* tutorial, const QString& table) {
        CachedFiles files;

        auto db = openDb(tutorial->folderInfo()->cachePath());
        if (!db.isOpen())
            return files;

        int index = tutorial->index();
        Q_ASSERT(index >= 0);

        QSqlQuery query;
        query.prepare(QString("SELECT name, data, checksum, modified FROM %1 WHERE tutorial_id=:tutorial_id").arg(table));
        query.bindValue(":tutorial_id", index);
        if (!query.exec()) {
            qWarning() << "failed to retrieve cached files from:" << table;
            return files;
        }

        while (query.next()) {
            QString name = query.value(0).toString();
            QByteArray data = query.value(1).toByteArray();
            QString checksum = query.value(2).toString();
            QDateTime modified = query.value(3).toDateTime();
            files[name] = { name, data, checksum, modified };
        }

        db.close();

        return files;
    }

    CachedFile cachedInfo(const Tutorial* tutorial) {
        CachedFiles infos = cachedFiles(tutorial, "infos");
        if (infos.size() == 0) {
            qWarning() << "no info found for tutorial:" << tutorial->title();
            return CachedFile();
        } else if (infos.size() > 1) {
            qWarning() << "more than one info found for tutorial:" << tutorial->title();
            return CachedFile();
        } else {
            return infos.first();
        }
    }

    void update(Tutorial* t) {
        if (t->hasInfo()) {
            if (t->info().data.isEmpty()) {
                qDebug() << "reading info from cache for tutorial:" << t->title();
                t->set_info(cachedInfo(t));
                t->set_images(cachedFiles(t, "images"));
            }
        } else {
            t->set_info(CachedFile());
            t->set_images(CachedFiles());
        }
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

void Collection::setup(const QList<FolderInfo *> &infos)
{
    Q_D(Collection);
    d->setup(infos);
}

void Collection::startLoad()
{
    Q_D(Collection);
    d->startLoad();
}

void Collection::update(Tutorial *tutorial)
{
    Q_D(Collection);
    d->update(tutorial);
}

} // namespace folders
} // namespace tc

#include "collection.moc"
