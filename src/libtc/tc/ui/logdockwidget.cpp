#include "logdockwidget.h"
#include "logwidget.h"

namespace tc {
namespace ui {

class LogDockWidgetPrivate: public QObject
{
    Q_DECLARE_PUBLIC(LogDockWidget)
    LogDockWidget* const q_ptr { nullptr };
    LogWidget* const m_widget { nullptr };

    explicit LogDockWidgetPrivate(LogDockWidget* ptr)
        : q_ptr(ptr)
        , m_widget(new LogWidget(ptr))
    {}
};

LogDockWidget::LogDockWidget(QWidget *parent)
    : QDockWidget(parent)
    , d_ptr(new LogDockWidgetPrivate(this))
{
}

LogDockWidget::~LogDockWidget()
{
}

} // namespace ui
} // namespace tc
