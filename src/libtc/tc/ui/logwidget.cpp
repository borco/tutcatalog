#include "logwidget.h"

namespace tc {
namespace ui {

class LogWidgetPrivate: public QObject
{
    Q_DECLARE_PUBLIC(LogWidget)
    LogWidget* const q_ptr;

    explicit LogWidgetPrivate(LogWidget* ptr) : q_ptr(ptr) {}
};

LogWidget::LogWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new LogWidgetPrivate(this))
{
}

LogWidget::~LogWidget()
{
}

} // namespace ui
} // namespace tc
