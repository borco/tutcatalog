#pragma once

#include "libtc_global.h"
#include "tc/persistent.h"

#include "dockablewidget.h"

namespace tc {
namespace ui {

class InfoWidgetPrivate;
class LIBTCSHARED_EXPORT InfoWidget: public DockableWidget, public Persistent
{
public:
    explicit InfoWidget(QWidget* parent = nullptr);
    ~InfoWidget();

    QList<Action *> dockToolBarActions() const override;
    QList<Action *> appToolBarActions() const override;

    void saveSettings(Settings &settings) const override;
    void loadSettings(const Settings &settings) override;

    void onSelectionChanged(const QSet<int>& selection);

private:
    Q_DECLARE_PRIVATE(InfoWidget)
    QScopedPointer<InfoWidgetPrivate> const d_ptr;
};
} // namespace ui
} // namespace tc
