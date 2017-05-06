#include "proxymodel.h"

namespace tc {
namespace tutorials {

struct ProxyModelPrivate
{
};

ProxyModel::ProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , d_ptr(new ProxyModelPrivate)
{
}

ProxyModel::~ProxyModel()
{
}

bool ProxyModel::lessThan(const QModelIndex &sourceLeft, const QModelIndex &sourceRight) const
{
    // TODO: implement sorting
    return QSortFilterProxyModel::lessThan(sourceLeft, sourceRight);
}

}
}
