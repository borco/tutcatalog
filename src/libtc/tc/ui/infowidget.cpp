#include "infowidget.h"
#include "pixmap.h"
#include "theme.h"

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
    QList<Action*> m_dockToolBarActions;
    QList<Action*> m_appToolBarActions;
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
        q->setWindowTitle(tr("Info"));
        q->setWindowIcon(Pixmap::fromFont(Theme::AwesomeFont, "\uf27b", Theme::MainToolBarIconSize, Theme::MainToolBarIconColor));

        m_stackedWidget = new QStackedWidget;
        m_stackedWidget->setFrameStyle(QFrame::NoFrame);

        auto label = new QLabel(tr("No tutorial selected"));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_noSelectionPage = label;

        m_edit = new QTextEdit;
        m_edit->setReadOnly(true);
        m_edit->setFrameStyle(QFrame::NoFrame);
        m_singleSelectionPage = m_edit;

        label = new QLabel(tr("Multiple tutorials selected"));
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        m_multipleSelectionPage = label;

        m_stackedWidget->addWidget(m_noSelectionPage);
        m_stackedWidget->addWidget(m_singleSelectionPage);
        m_stackedWidget->addWidget(m_multipleSelectionPage);

        auto layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_stackedWidget);
        q->setLayout(layout);
    }

    void onSelectionChanged(const QSet<int>& selection) {
        if (selection.size() == 0) {
            m_stackedWidget->setCurrentWidget(m_noSelectionPage);
        } else if (selection.size() == 1) {
            m_stackedWidget->setCurrentWidget(m_singleSelectionPage);
            int row = selection.values()[0];
            m_edit->setText(QString::number(row));
        } else {
            m_stackedWidget->setCurrentWidget(m_multipleSelectionPage);
        }
    }
};

InfoWidget::InfoWidget(QWidget *parent)
    : DockableWidget(parent)
    , d_ptr(new InfoWidgetPrivate(this))
{
}

InfoWidget::~InfoWidget()
{
}

QList<Action *> InfoWidget::dockToolBarActions() const
{
    Q_D(const InfoWidget);
    return d->m_dockToolBarActions;
}

QList<Action *> InfoWidget::appToolBarActions() const
{
    Q_D(const InfoWidget);
    return d->m_appToolBarActions;
}

void InfoWidget::saveSettings(Settings &/*settings*/) const
{
}

void InfoWidget::loadSettings(const Settings &/*settings*/)
{
}

void InfoWidget::onSelectionChanged(const QSet<int> &selection)
{
    Q_D(InfoWidget);
    d->onSelectionChanged(selection);
}

} // namespace ui
} // namespace tc
