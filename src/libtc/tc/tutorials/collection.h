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
    explicit Collection(QObject *parent = nullptr);
    ~Collection();

    void setup(const QList<FolderInfo*>& infos);
    void startLoad();

    void update(Tutorial* tutorial);

signals:
    void loaded(const QList<tc::tutorials::Tutorial*>& tutorials);

private:
    Q_DECLARE_PRIVATE(Collection)
    QScopedPointer<CollectionPrivate> const d_ptr;
};

} // namespace tutorials
} // namespace tc
