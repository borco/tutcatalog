#pragma once

#include "libtc_global.h"

#include <QAbstractListModel>

namespace tc {
namespace tutorials {

class Tutorial;

class ModelPrivate;
class LIBTCSHARED_EXPORT Model : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit Model(QObject* parent = 0);
    ~Model();

    void clear();
    void append(Tutorial* tutorial);
    void remove(Tutorial* tutorial);

    int columnCount(const QModelIndex &parent) const override;
    int rowCount(const QModelIndex &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    QScopedPointer<ModelPrivate> const d_ptr;
    Q_DECLARE_PRIVATE(Model)
};

} // namespace tutorials
} // namespace tc