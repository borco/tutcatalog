#pragma once

#include <QStyledItemDelegate>
#include <QSvgRenderer>

namespace tc {
namespace ui {

class BoolDelegate : public QStyledItemDelegate
{
public:
    explicit BoolDelegate(const QString& svgTrue, const QString& svgFalse, QObject* parent = 0);
    QString displayText(const QVariant &value, const QLocale &locale) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    void paint(QPainter *painter, const QRect &rect, const QPalette &palette, QSvgRenderer& svg) const;

    mutable QSvgRenderer m_svgTrue;
    mutable QSvgRenderer m_svgFalse;
};

class FileSizeDelegate : public QStyledItemDelegate
{
public:
    explicit FileSizeDelegate(QObject* parent = 0);
    QString displayText(const QVariant &value, const QLocale &) const;
};

class FileSizeToDurationDelegate : public QStyledItemDelegate
{
public:
    FileSizeToDurationDelegate(QObject* parent = 0);
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
