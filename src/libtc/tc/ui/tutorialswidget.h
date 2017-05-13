#pragma once

#include "tc/persistent.h"
#include "dockablewidget.h"
#include "QQmlVarPropertyHelpers.h"

#include <QSet>

namespace tc {

namespace tutorials {
class Model;
class ProxyModel;
}

namespace ui {

class TutorialsWidgetPrivate;
class LIBTCSHARED_EXPORT TutorialsWidget: public DockableWidget, public Persistent
{
    Q_OBJECT

    QML_READONLY_VAR_PROPERTY(QSet<int>, selection)

public:
    explicit TutorialsWidget(QWidget* parent = nullptr);
    ~TutorialsWidget();

    QList<Action *> dockToolBarActions() const override;
    QList<Action *> appToolBarActions() const override;

    void saveSettings(Settings &settings) const override;
    void loadSettings(const Settings &settings) override;

    void setModel(tutorials::Model* model);
    tutorials::ProxyModel* proxyModel() const;

private:
    Q_DECLARE_PRIVATE(TutorialsWidget)
    QScopedPointer<TutorialsWidgetPrivate> const d_ptr;
};

} // namespace ui
} // namespace tc
