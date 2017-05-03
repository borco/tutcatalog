#include "logwidget.h"

#include <QIcon>

namespace tc {
namespace ui {

class LogWidgetPrivate: public QObject
{
    Q_DECLARE_PUBLIC(LogWidget)
    LogWidget* const q_ptr;

    explicit LogWidgetPrivate(LogWidget* ptr) : q_ptr(ptr) {
        Q_Q(LogWidget);
        q->setWindowTitle(tr("Log"));
        q->setWindowIcon(QIcon(":/images/log-dock-icon.svg"));
    }
};

LogWidget::LogWidget(QWidget *parent)
    : DockableWidget(parent)
    , d_ptr(new LogWidgetPrivate(this))
{
}

LogWidget::~LogWidget()
{
}

} // namespace ui
} // namespace tc
