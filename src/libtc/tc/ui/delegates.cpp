#include "delegates.h"

#include "tc/tutorials/tutorial.h"

#include <QPainter>

namespace tc {
namespace ui {

BoolDelegate::BoolDelegate(const QString &svgTrue, const QString &svgFalse, QObject *parent)
    : QStyledItemDelegate(parent)
{
    if (!svgTrue.isEmpty()) m_svgTrue.load(svgTrue);
    if (!svgFalse.isEmpty()) m_svgFalse.load(svgFalse);
}

QString BoolDelegate::displayText(const QVariant &value, const QLocale &) const
{
    return (m_svgTrue.isValid() || m_svgFalse.isValid()) ? QString() : value.toString();
}

void BoolDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
    if (index.data().canConvert<bool>()) {
        bool value = index.data().toBool();
        if (value) {
            if (m_svgTrue.isValid())
                paint(painter, option.rect, option.palette, m_svgTrue);
        } else {
            if (m_svgFalse.isValid())
                paint(painter, option.rect, option.palette, m_svgFalse);
        }
    }
}

void BoolDelegate::paint(QPainter *painter, const QRect &rect, const QPalette &palette, QSvgRenderer &svg) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);
    painter->setBrush(palette.foreground());
    int size = qMin(rect.width(), rect.height());
    int x = rect.x() + (rect.width() - size) / 2;
    int y = rect.y() + (rect.height() - size) / 2;
    svg.render(painter, QRect(x, y, size, size).adjusted(2, 2, -2, -2));

    painter->restore();
}

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
