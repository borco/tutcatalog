#include "theme.h"
#include <QString>

namespace tc {
namespace ui {

const QString Theme::AwesomeFont { ":fonts/FontAwesome.otf" };
const QString Theme::LineaBasicFont { ":fonts/linea-basic-10.ttf" };
const QString Theme::LineaEcomerceFont { "fonts/linea-ecommerce-10.ttf" };
const QString Theme::MaterialFont { ":fonts/MaterialIcons-Regular.ttf" };
const QString Theme::DefaultFont { "" };

const QString Theme::MainToolBarStyle {
    "QToolBar {background: #444; color: #fff;}"
    "QLabel {color: #fff;}"
};

const QString Theme::DockToolBarStyle { Theme::MainToolBarStyle };

const QString Theme::StatusBarStyle {
    "QStatusBar { background: #444; color: #fff; }"
    "QLabel { color: #fff; }"
};

const int Theme::MainToolBarIconColor = 0xffffff;
const int Theme::MainToolBarIconDisabledColor = 0x888888;
const int Theme::MainToolBarIconSize { 24 };

const int Theme::DockToolBarIconColor = 0xeeeeee;
const int Theme::DockToolBarIconDisabledColor = 0x888888;
const int Theme::DockToolBarIconSize { 16 };

} // namespace ui
} // namespace tc
