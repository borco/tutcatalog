#pragma once

#include "libtc_global.h"

#include <QWidget>

namespace tc {
namespace ui {

class LIBTCSHARED_EXPORT DockableWidget : public QWidget
{
public:
    explicit DockableWidget(QWidget* parent = nullptr) : QWidget(parent) {}

};

} // namespace ui
} // namespace tc
