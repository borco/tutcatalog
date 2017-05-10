#include "delegates.h"

#include "tc/tutorials/tutorial.h"

#include <QPainter>

namespace tc {
namespace ui {

FileSizeDelegate::FileSizeDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QString FileSizeDelegate::displayText(const QVariant &value, const QLocale &) const
{
    return tutorials::Tutorial::fileSizeAsString(value.value<qint64>());
}

DurationDelegate::DurationDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QString DurationDelegate::displayText(const QVariant &value, const QLocale &) const
{
    int v = value.value<int>();
    int minutes = v % 60;
    int hours = v / 60;
    return v > 0 ? ( hours > 0 ? QString("%1h %2m").arg(hours).arg(minutes)
                               : QString("%1m").arg(minutes))
                 : "";
}

RatingDelegate::RatingDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QString RatingDelegate::displayText(const QVariant &value, const QLocale &) const
{
    return tutorials::Tutorial::ratingAsString(value.toInt());
}

} // namespace ui
} // namespace tc
