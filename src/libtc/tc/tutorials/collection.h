#pragma once

#include "libtc_global.h"
#include <QObject>
#include <QVector>

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

    void setup(const QVector<FolderInfo*>& infos);
    void startLoad();

signals:
    void loaded(const QVector<tc::tutorials::Tutorial*>& tutorials);

private:
    Q_DECLARE_PRIVATE(Collection)
    QScopedPointer<CollectionPrivate> const d_ptr;
};

} // namespace tutorials
} // namespace tc
