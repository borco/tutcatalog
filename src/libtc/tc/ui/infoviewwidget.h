#pragma once

#include "libtc_global.h"

#include <QWidget>

namespace tc {

namespace tutorials {
class Tutorial;
}

namespace ui {

class InfoViewWidgetPrivate;
class LIBTCSHARED_EXPORT InfoViewWidget: public QWidget
{
public:
    explicit InfoViewWidget(QWidget* parent = nullptr);
    ~InfoViewWidget();

    void setTutorial(tutorials::Tutorial* tutorial);

private:
    Q_DECLARE_PRIVATE(InfoViewWidget)
    QScopedPointer<InfoViewWidgetPrivate> const d_ptr;
};
} // namespace ui
} // namespace tc
