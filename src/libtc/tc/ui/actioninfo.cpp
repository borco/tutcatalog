#include "actioninfo.h"

#include <QAction>
#include <QFile>
#include <QPainter>
#include <QString>
#include <QSvgRenderer>

namespace tc {
namespace ui {

const int ActionInfo::DefaultColor { 0xffffff };
const int ActionInfo::DefaultDisabledColor { 0x888888 };
const int ActionInfo::DefaultIconSize { 32 };

namespace {

// NOTE: this is a hack to get the white icon work
//     * it assumes that all we need is to replace "fill:#ffffff" with other color
QPixmap makeIcon(const QString& iconName,
                 int color = ActionInfo::DefaultColor,
                 int disabledColor = ActionInfo::DefaultColor,
                 int iconSize = ActionInfo::DefaultIconSize)
{
    QFile file(iconName);
    if (!file.open(QIODevice::ReadOnly))
        return QPixmap();

    QByteArray data = file.readAll();
    if (color != disabledColor) {
        data = data.replace(QString("fill=\"#%1\"").arg((QString("%1").arg(color, 6, 16, QChar('0')).toUpper())).toLatin1(),
                            QString("fill=\"#%1\"").arg(disabledColor, 6, 16, QChar('0')).toLatin1());
    }

    QImage image (QSize(iconSize, iconSize), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QSvgRenderer r(data);
    QPainter p(&image);
    r.render(&p);
    return QPixmap::fromImage(image);
}

} // namespace

ActionInfo::ActionInfo(const QString &icon,
                       const QString &tip,
                       const QList<QKeySequence> &shortcuts,
                       bool checkable,
                       int color,
                       int disabledColor)
    : m_icon(icon)
    , m_tip(tip)
    , m_shortcuts(shortcuts)
    , m_checkable(checkable)
    , m_color(color)
    , m_disabledColor(disabledColor)
{

}

QAction* ActionInfo::newAction(QObject *parent) const
{
    auto action = new QAction(parent);

    if (m_disabledColor == m_color) {
        action->setIcon(QIcon(m_icon));
    } else {
        QIcon icon;
        icon.addPixmap(makeIcon(m_icon), QIcon::Active);
        icon.addPixmap(makeIcon(m_icon, m_color, m_disabledColor), QIcon::Disabled);
        action->setIcon(icon);
    }

    action->setToolTip(m_shortcuts.isEmpty() ? m_tip : m_tip.arg(QKeySequence::listToString(m_shortcuts)));
    action->setShortcuts(m_shortcuts);
    action->setCheckable(m_checkable);

    return action;
}

} // namespace ui
} // namespace tc
