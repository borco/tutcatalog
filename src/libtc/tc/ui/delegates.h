#pragma once

#include <QStyledItemDelegate>
#include <QSvgRenderer>

namespace tc {
namespace ui {

class FileSizeDelegate : public QStyledItemDelegate
{
public:
    explicit FileSizeDelegate(QObject* parent = 0);
    QString displayText(const QVariant &value, const QLocale &) const;
};

class DurationDelegate : public QStyledItemDelegate
{
public:
    explicit DurationDelegate(QObject* parent = 0);
    QString displayText(const QVariant &value, const QLocale &) const;
};

class RatingDelegate : public QStyledItemDelegate
{
public:
    explicit RatingDelegate(QObject* parent = 0);
    QString displayText(const QVariant &value, const QLocale &) const;
};

} // namespace ui
} // namespace tc
