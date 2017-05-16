#pragma once

#include "libtc_global.h"
#include <QObject>
#include <QDateTime>

class QAction;

namespace tc {
namespace tutorials {

class FolderInfo;
class Tutorial;

class CollectionPrivate;
class LIBTCSHARED_EXPORT Collection : public QObject
{
    Q_OBJECT

public:
    struct CachedFile {
        QString name;
        QByteArray data;
        QString checksum; // crc32
        QDateTime modified;
    };

    typedef QMap<QString, CachedFile> CachedFiles;

    explicit Collection(QObject *parent = nullptr);
    ~Collection();

    void setup(const QVector<FolderInfo*>& infos);
    void startLoad();

    CachedFiles cachedInfos(const Tutorial* tutorial) const;
    CachedFiles cachedImages(const Tutorial* tutorial) const;
    CachedFiles cachedFiles(const Tutorial* tutorial) const;

signals:
    void loaded(tutorials::Tutorial* tutorial);

private:
    Q_DECLARE_PRIVATE(Collection)
    QScopedPointer<CollectionPrivate> const d_ptr;
};

} // namespace tutorials
} // namespace tc
