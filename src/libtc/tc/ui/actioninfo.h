#pragma once

#include "libtc_global.h"
#include <QObject>

class QAction;
class QKeySequence;
class QToolButton;

namespace tc {
namespace ui {

class LIBTCSHARED_EXPORT ActionInfo
{
public:
    static const int DefaultColor;
    static const int DefaultDisabledColor;
    static const int DefaultIconSize;

    explicit ActionInfo(const QString& icon = QString(),
                        const QString& tip = QString(),
                        const QList<QKeySequence> &shortcuts = QList<QKeySequence>(),
                        bool checkable = false,
                        int color = DefaultColor,
                        int disabledColor = DefaultColor
            );

    QAction* newAction(QObject* parent = nullptr) const;

private:
    QString m_icon;
    QString m_tip;
    QList<QKeySequence> m_shortcuts;
    bool m_checkable { false };
    int m_color { DefaultColor };
    int m_disabledColor { DefaultColor };
};

} // namespace ui
} // namespace tc
