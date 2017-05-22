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
    return tutorials::Tutorial::durationAsString(value.value<int>());
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
