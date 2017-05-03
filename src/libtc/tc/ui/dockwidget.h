#pragma once

#include "libtc_global.h"

#include <QDockWidget>

namespace tc {
namespace ui {

class DockableWidget;

class DockWidgetPrivate;
class LIBTCSHARED_EXPORT DockWidget : public QDockWidget
{
public:
    explicit DockWidget(DockableWidget* content, QWidget* parent = nullptr);
    ~DockWidget();

    DockableWidget* content() const;

private:
    Q_DECLARE_PRIVATE(DockWidget)
    QScopedPointer<DockWidgetPrivate> const d_ptr;
};

} // namespace ui
} // namespace tc
