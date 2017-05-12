#pragma once

#include "libtc_global.h"
#include "QQmlVarPropertyHelpers.h"

#include <QWidget>

class QLabel;

namespace tc {
namespace ui {

class LIBTCSHARED_EXPORT LabeledValue : public QWidget
{
    Q_OBJECT

    QML_WRITABLE_VAR_PROPERTY(QString, label)
    QML_WRITABLE_VAR_PROPERTY(QString, value)

public:
    explicit LabeledValue(QWidget* parent = nullptr);
};

} // namespace ui
} // namespace tc
