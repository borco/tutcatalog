#pragma once

#include "tc/persistent.h"
#include "dockablewidget.h"

namespace tc {
namespace ui {

class TutorialsWidgetPrivate;
class LIBTCSHARED_EXPORT TutorialsWidget: public DockableWidget, public Persistent
{
    Q_OBJECT

public:
    explicit TutorialsWidget(QWidget* parent = nullptr);
    ~TutorialsWidget();

    QList<QAction *> dockToolBarActions() const override;
    QList<QAction *> appToolBarActions() const override;

    void saveSettings(Settings &settings) const override;
    void loadSettings(const Settings &settings) override;

private:
    Q_DECLARE_PRIVATE(TutorialsWidget)
    QScopedPointer<TutorialsWidgetPrivate> const d_ptr;
};

} // namespace ui
} // namespace tc
