#pragma once

#include "libtc_global.h"
#include <QObject>

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
    typedef QMap<QString, QByteArray> CachedFiles;

    explicit Collection(QObject *parent = nullptr);
    ~Collection();

    void setup(const QVector<FolderInfo*>& infos);
    void startLoad();

    CachedFiles cachedFiles(const Tutorial* tutorial) const;

signals:
    void loaded(tutorials::Tutorial* tutorial);

private:
    Q_DECLARE_PRIVATE(Collection)
    QScopedPointer<CollectionPrivate> const d_ptr;
};

} // namespace tutorials
} // namespace tc
