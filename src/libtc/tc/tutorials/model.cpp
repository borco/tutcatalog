#include "model.h"
#include "tutorial.h"

#include <QDebug>

namespace {

struct Header {
    enum Id {
        Title,
        Publisher,
        Authors,
        Info,
        ToDo,
        Complete,
        Rating,
        Viewed,
        Deleted,
        Online,
        Duration,
        Size,
        Levels,
        Created,
        Modified,
        Released,
        SkipBackup,
    };

    Id id;
    QString title;
};

}

namespace tc {
namespace tutorials {

struct ModelPrivate : public QObject
{
    const QVector<Header> Headers {
        { Header::Title, tr("Title") },
        { Header::Publisher, tr("Publisher") },
        { Header::Authors, tr("Authors") },
        { Header::Info, tr("Info") },
        { Header::ToDo, tr("ToDo") },
        { Header::Complete, tr("Complete") },
        { Header::Rating, tr("Rating") },
        { Header::Viewed, tr("Viewed") },
        { Header::Deleted, tr("Deleted") },
        { Header::Online, tr("Online") },
        { Header::Duration, tr("Duration") },
        { Header::Size, tr("Size") },
        { Header::Levels, tr("Levels") },
        { Header::Created, tr("Created") },
        { Header::Modified, tr("Modified") },
        { Header::Released, tr("Released") },
        { Header::SkipBackup, tr("No Backup") },
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
    return parent.isValid() ? 0 : d->Headers.size();
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
        return d->Headers.value(section).title;
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

    switch(role) {
    case Qt::DisplayRole: {
        switch (index.column()) {
        case Header::Title: return item->title();
        case Header::Authors: return item->authors().join(", ");
        case Header::Publisher: return item->publisher();
        case Header::Info: return item->hasInfo() ? "Y" : "";
        case Header::ToDo: return item->onToDoList() ? "Y" : "";
        case Header::Complete: return item->isComplete() ? "" : "N";
        case Header::Rating: return item->rating();
        case Header::Viewed: return item->isViewed() ? "Y" : "";
        case Header::Deleted: return item->isDeleted() ? "Y" : "";
        case Header::Online: return item->isOnline() ? "Y" : "";
        case Header::Duration: return item->duration();
        case Header::Size: return item->size();
        case Header::Levels: return item->levels();
        case Header::Created: return item->created();
        case Header::Modified: return item->modified();
        case Header::Released: return item->released();
        case Header::SkipBackup: return item->skipBackup() ? "Y" : "";
        default: return QVariant();
        }
    }

    default:
        return QVariant();
    }
}

}
}
