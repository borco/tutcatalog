#include "infowidget.h"
#include "pixmap.h"
#include "theme.h"

#include "tc/tutorials/tutorial.h"

#include <QLabel>
#include <QSet>
#include <QStackedWidget>
#include <QTextEdit>
#include <QVBoxLayout>

namespace tc {
namespace ui {

class InfoWidgetPrivate : public QObject
{
    Q_DECLARE_PUBLIC(InfoWidget)
    InfoWidget* const q_ptr { nullptr };
    QWidget* m_noSelectionPage { nullptr };
    QWidget* m_singleSelectionPage { nullptr };
    QWidget* m_multipleSelectionPage { nullptr };
    QTextEdit* m_edit { nullptr };
    QStackedWidget* m_stackedWidget { nullptr };

    explicit InfoWidgetPrivate(InfoWidget* ptr) : q_ptr(ptr) {
        setupUi();
    }

    void setupUi() {
        Q_Q(InfoWidget);

        m_stackedWidget = new QStackedWidget;
        m_stackedWidget->setFrameStyle(QFrame::NoFrame);

        auto label = new QLabel(tr("No tutorial selected"));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_noSelectionPage = label;

        label = new QLabel(tr("Multiple tutorials selected"));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_multipleSelectionPage = label;

        m_edit = new QTextEdit;
        m_edit->setReadOnly(true);
        m_edit->setFrameStyle(QFrame::NoFrame);
        m_singleSelectionPage = m_edit;

        m_stackedWidget->addWidget(m_noSelectionPage);
        m_stackedWidget->addWidget(m_singleSelectionPage);
        m_stackedWidget->addWidget(m_multipleSelectionPage);

        auto layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_stackedWidget);
        q->setLayout(layout);
    }

    void updateFromCache(tutorials::Tutorial* tutorial) {
        QString text = QString("Title: <b>%1</b>"
                               "<br>Index: <b>%2</b>")
                .arg(tutorial->title())
                .arg(tutorial->index());
        m_edit->setText(text);
    }

    void updateFromDisk(tutorials::Tutorial* tutorial) {
        QString text = QString("Title: <b>%1</b><br>Index: <b>%2</b>").arg(tutorial->title()).arg(tutorial->index());
        text += "<br><b>Update from disk not implemented yet.</b>";
        m_edit->setText(text);
    }

    void onSelectionChanged(const QSet<tutorials::Tutorial*> &selection) {
        if (selection.size() == 0) {
            m_stackedWidget->setCurrentWidget(m_noSelectionPage);
        } else if (selection.size() == 1) {
            m_stackedWidget->setCurrentWidget(m_singleSelectionPage);
            tutorials::Tutorial* t = selection.values()[0];
            if (t->isCached()) {
                updateFromCache(t);
            } else {
                updateFromDisk(t);
            }
        } else {
            m_stackedWidget->setCurrentWidget(m_multipleSelectionPage);
        }
    }
};

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new InfoWidgetPrivate(this))
{
}

InfoWidget::~InfoWidget()
{
}

void InfoWidget::onSelectionChanged(const QSet<tutorials::Tutorial*> &selection)
{
    Q_D(InfoWidget);
    d->onSelectionChanged(selection);
}

} // namespace ui
} // namespace tc
