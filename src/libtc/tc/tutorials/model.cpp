#include "model.h"
#include "tutorial.h"

#include <QDebug>

namespace tc {
namespace tutorials {

struct ModelPrivate : public QObject
{
    const QVector<QString> Labels {
        tr("Title"),
        tr("Publisher"),
        tr("Authors"),
        tr("Info"),
        tr("Checksum"),
        tr("ToDo"),
        tr("Complete"),
        tr("Rating"),
        tr("Viewed"),
        tr("Deleted"),
        tr("Online"),
        tr("Duration"),
        tr("Size"),
        tr("Levels"),
        tr("Created"),
        tr("Modified"),
        tr("Released"),
        tr("No Backup"),
        tr("Size / Duration"),
    };

    QVector<Tutorial*> items;
};

Model::Model(QObject* parent)
    : QAbstractListModel(parent)
    , d_ptr(new ModelPrivate)
{
}

Model::~Model()
{
}

void Model::clear()
{
    Q_D(Model);
    beginResetModel();
    qDeleteAll(d->items);
    d->items.clear();
    endResetModel();
}

void Model::append(Tutorial *tutorial)
{
    Q_D(Model);
    beginInsertRows(QModelIndex(), d->items.size(), d->items.size());
    tutorial->setParent(this);
    d->items.append(tutorial);
    endInsertRows();
}

void Model::remove(Tutorial *tutorial)
{
    Q_D(Model);
    int row = d->items.indexOf(tutorial);
    if (row < 0) {
        qWarning() << "ignored request to remove unknown tutorial";
        return;
    }
    beginRemoveRows(QModelIndex(), row, row);
    d->items.removeOne(tutorial);
    endRemoveRows();
}

int Model::columnCount(const QModelIndex &parent) const
{
    Q_D(const Model);
    return parent.isValid() ? 0 : d->Labels.size();
}

int Model::rowCount(const QModelIndex &parent) const
{
    Q_D(const Model);
    return parent.isValid() ? 0 : d->items.size();
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_D(const Model);
    if (orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole) {
        return d->Labels.value(section);
    }
    return QVariant();
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    Q_D(const Model);

    if (!index.isValid())
        return QVariant();

    int row = index.row();
    if (row < 0 || row >= d->items.size())
        return QVariant();

    const Tutorial* item = d->items.at(row);
    if (!item)
        return QVariant();

    int column = index.column();
    switch(role) {
    case Qt::DisplayRole: {
        switch (column) {
        case Model::Title: return item->title();
        case Model::Authors: return item->authors().join(", ");
        case Model::Publisher: return item->publisher();
        case Model::HasInfo: return item->hasInfo() ? "Y" : "";
        case Model::HasChecksum: return item->hasChecksum() ? "Y" : "";
        case Model::OnToDoList: return item->onToDoList() ? "Y" : "";
        case Model::IsComplete: return item->isComplete() ? "" : "N";
        case Model::Rating: return item->rating();
        case Model::Viewed: return item->isViewed() ? "Y" : "";
        case Model::Deleted: return item->isDeleted() ? "Y" : "";
        case Model::Online: return item->isOnline() ? "Y" : "";
        case Model::Duration: return item->duration();
        case Model::Size: return item->size();
        case Model::Levels: return item->levels();
        case Model::Created: return item->created();
        case Model::Modified: return item->modified();
        case Model::Released: return item->released();
        case Model::SkipBackup: return item->skipBackup() ? "Y" : "";
        case Model::FileSizeToDuration: return Tutorial::fileSizeToDurationAsString(item->size(), item->duration());
        default: return QVariant();
        }
    }

    default:
        return QVariant();
    }
}

}
}
