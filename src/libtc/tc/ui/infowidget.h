#pragma once

#include "libtc_global.h"
#include "tc/persistent.h"

#include <QWidget>

namespace tc {
namespace ui {

class InfoWidgetPrivate;
class LIBTCSHARED_EXPORT InfoWidget: public QWidget
{
public:
    explicit InfoWidget(QWidget* parent = nullptr);
    ~InfoWidget();

    void onSelectionChanged(const QSet<int>& selection);

private:
    Q_DECLARE_PRIVATE(InfoWidget)
    QScopedPointer<InfoWidgetPrivate> const d_ptr;
};
} // namespace ui
} // namespace tc
