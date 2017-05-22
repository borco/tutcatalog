#include "model.h"
#include "tutorial.h"

#include "tc/ui/theme.h"

#include <QBrush>
#include <QDebug>
#include <QFont>

namespace tc {
namespace tutorials {

struct ModelPrivate : public QObject
{
    const QStringList Labels {
        tr("Title"),
        tr("Publisher"),
        tr("Authors"),
        tr("Info"),
        tr("Checksum"),
        tr("ToDo"),
        tr("Keep"),
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
        tr("MB/min"),
        tr("Cached"),
    };

    QVector<Tutorial*> items;
    QFont m_awesomeFont { ui::Theme::AwesomeFont };
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
        case Model::HasInfo: return item->hasInfo() ? "\uf27b" : "";
        case Model::HasChecksum: return item->hasChecksum() ? "\uf132" : "";
        case Model::OnToDoList: return item->onToDoList() ? "\uf073" : "";
        case Model::OnKeepList: return item->onKeepList() ? "\uf02e" : "";
        case Model::IsComplete: return item->isComplete() ? "" : "\uf042";
        case Model::Rating: return item->rating();
        case Model::IsViewed: return item->isViewed() ? "\uf06e" : "";
        case Model::IsDeleted: return item->isDeleted() ? "\uf014" : "";
        case Model::IsOnline: return item->isOnline() ? "\uf0e8" : "";
        case Model::Duration: return item->duration();
        case Model::Size: return item->size();
        case Model::Levels: return item->levels();
        case Model::Created: return item->created();
        case Model::Modified: return item->modified();
        case Model::Released: return item->released();
        case Model::NoBackup: return item->noBackup() ? "\uf12a" : "";
        case Model::IsCached: return item->isCached() ? "\uf017" : "";
        case Model::SizeToDuration: {
            static const double MegaByte = 1024 * 1024;
            double ratio = item->size() / (item->duration() * MegaByte);
            return ratio > 0.01 ? QString::number(ratio, 'f', 2) : "";
        }
        default: break;
        }
    }

    case Qt::FontRole: {
        switch (column) {
        case Model::HasInfo:
        case Model::HasChecksum:
        case Model::OnToDoList:
        case Model::OnKeepList:
        case Model::IsComplete:
        case Model::IsOnline:
        case Model::IsViewed:
        case Model::IsDeleted:
        case Model::NoBackup:
        case Model::IsCached:
            return d->m_awesomeFont;
        default:
            break;
        }
    }

    case Qt::TextAlignmentRole: {
        switch (column) {

        case Model::HasInfo:
        case Model::HasChecksum:
        case Model::OnToDoList:
        case Model::OnKeepList:
        case Model::IsComplete:
        case Model::IsOnline:
        case Model::IsViewed:
        case Model::IsDeleted:
        case Model::NoBackup:
        case Model::IsCached:
            return Qt::AlignHCenter;

        case Model::Size:
        case Model::Duration:
        case Model::SizeToDuration:
        case Model::Created:
        case Model::Released:
        case Model::Modified:
            return Qt::AlignRight;

        default:
            break;
        }
    }

    case Qt::ForegroundRole: {
        static const QBrush WarningColor { QColor("#A00") };
        static const QBrush InfoColor { QColor("#996515") };
        switch(column) {
        case Model::Title: {
            if (!item->hasCanonicalPath())
                return WarningColor;
            break;
        }

        case Model::Size: {
            const qint64 GigaByte = 1024 * 1024 * 1024;
            qint64 size = item->size();
            if (size > 10 * GigaByte)
                return WarningColor;
            else if (size > GigaByte)
                return InfoColor;
            break;
        }

        case Model::IsComplete:
            return WarningColor;

        default:
            break;
        }
    }

    default:
        break;
    }

    return QVariant();
}

const QStringList &Model::columns() const
{
    Q_D(const Model);
    return d->Labels;
}

Tutorial *Model::tutorial(int row) const
{
    Q_D(const Model);
    return d->items.value(row);
}

}
}
