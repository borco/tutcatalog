#pragma once

#include "libtc_global.h"

#include <QSortFilterProxyModel>

namespace tc {
namespace tutorials {

class ProxyModelPrivate;
class LIBTCSHARED_EXPORT ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ProxyModel(QObject* parent = 0);
    ~ProxyModel();

protected:
    bool lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const;

private:
    QScopedPointer<ProxyModelPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(ProxyModel)
};

} // namespace tutorials
} // namespace tc
