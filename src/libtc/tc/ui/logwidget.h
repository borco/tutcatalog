#pragma once

#include "libtc_global.h"
#include "tc/persistent.h"

#include "dockablewidget.h"

namespace tc {
namespace ui {

class LogWidgetPrivate;
class LIBTCSHARED_EXPORT LogWidget: public DockableWidget, public Persistent
{
public:
    explicit LogWidget(QWidget* parent = nullptr);
    ~LogWidget();

    QList<Action *> dockToolBarActions() const override;
    QList<Action *> appToolBarActions() const override;

    void saveSettings(Settings &settings) const override;
    void loadSettings(const Settings &settings) override;

private:
    Q_DECLARE_PRIVATE(LogWidget)
    QScopedPointer<LogWidgetPrivate> const d_ptr;
};
} // namespace ui
} // namespace tc
