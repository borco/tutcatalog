#pragma once

#include "libtc_global.h"
#include "QQmlVarPropertyHelpers.h"

#include <QAction>

namespace tc {
namespace ui {

class LIBTCSHARED_EXPORT Action : public QAction
{
    Q_OBJECT

    QML_WRITABLE_VAR_PROPERTY(bool, instantPopup)

public:
    explicit Action(QObject *parent = nullptr)
        : QAction(parent), m_instantPopup(false) {}
};

} // namespace ui
} // namespace tc
