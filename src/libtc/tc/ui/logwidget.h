#pragma once

#include "libtc_global.h"

#include "dockablewidget.h"

namespace tc {
namespace ui {

class LogWidgetPrivate;
class LIBTCSHARED_EXPORT LogWidget: public DockableWidget
{
public:
    explicit LogWidget(QWidget* parent = nullptr);
    ~LogWidget();

    QList<QAction *> dockToolBarActions() const override;
    QList<QAction *> appToolBarActions() const override;

private:
    Q_DECLARE_PRIVATE(LogWidget)
    QScopedPointer<LogWidgetPrivate> const d_ptr;
};
} // namespace ui
} // namespace tc
