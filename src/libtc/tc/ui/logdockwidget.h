#pragma once

#include "libtc_global.h"

#include <QDockWidget>

namespace tc {
namespace ui {

class LogDockWidgetPrivate;
class LIBTCSHARED_EXPORT LogDockWidget : public QDockWidget
{
public:
    explicit LogDockWidget(QWidget* parent = nullptr);
    ~LogDockWidget();

private:
    Q_DECLARE_PRIVATE(LogDockWidget)
    QScopedPointer<LogDockWidgetPrivate> const d_ptr;
};

} // namespace ui
} // namespace tc
