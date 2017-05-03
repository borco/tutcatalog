#pragma once

#include "libtc_global.h"

#include <QWidget>
#include <QIcon>

namespace tc {
namespace ui {

class LIBTCSHARED_EXPORT DockableWidget : public QWidget
{
public:
    explicit DockableWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    virtual QList<QAction*> dockToolBarActions() const = 0;
    virtual QList<QAction*> appToolBarActions() const = 0;

    virtual QString dockToolBarTitle() const { return windowTitle(); }
    virtual QString appToolBarToggleViewToolTip() const { return QString(tr("Show %1").arg(windowTitle())); }
    virtual QIcon appToolBarToggleViewIcon() const { return windowIcon(); }
};

} // namespace ui
} // namespace tc
