#pragma once

#include "libtc_global.h"

namespace tc {
namespace ui {

struct LIBTCSHARED_EXPORT Theme {
    static const QString AwesomeFont;
    static const QString LineaBasicFont;
    static const QString LineaEcomerceFont;
    static const QString MaterialFont;
    static const QString DefaultFont;

    static const QString MainToolBarStyle;
    static const QString DockToolBarStyle;
    static const QString StatusBarStyle;

    static const int MainToolBarIconColor;
    static const int MainToolBarIconDisabledColor;
    static const int MainToolBarIconSize;

    static const int DockToolBarIconColor;
    static const int DockToolBarIconDisabledColor;
    static const int DockToolBarIconSize;
};
} // namespace ui
} // namespace tc
