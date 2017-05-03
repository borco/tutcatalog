#include "theme.h"
#include <QString>

namespace tc {
namespace ui {

const QString Theme::MainToolBarStyle { "QToolBar {background: #444; color: white;} QLabel {color: white;}" };
const int Theme::MainToolBarIconSize { 24 };

const QString Theme::DockToolBarStyle { Theme::MainToolBarStyle };
const int Theme::DockToolBarIconSize { 16 };

} // namespace ui
} // namespace tc
