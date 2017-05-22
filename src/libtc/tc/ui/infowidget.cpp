#include "infowidget.h"

#include "infoviewwidget.h"
#include "pixmap.h"
#include "theme.h"

#include "tc/tutorials/collection.h"
#include "tc/tutorials/tutorial.h"

#include <QBuffer>
#include <QDebug>
#include <QLabel>
#include <QSet>
#include <QStackedWidget>
#include <QTextEdit>
#include <QVBoxLayout>

#include <iostream>

namespace tc {
namespace ui {

class InfoWidgetPrivate : public QObject
{
    Q_DECLARE_PUBLIC(InfoWidget)
    InfoWidget* const q_ptr { nullptr };
    QList<Action*> m_dockToolBarAction;
    QList<Action*> m_appToolBarAction;

    QWidget* m_noSelectionPage { nullptr };
    QWidget* m_singleSelectionPage { nullptr };
    QWidget* m_multipleSelectionPage { nullptr };
    InfoViewWidget* m_infoView { nullptr };
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

        m_infoView = new InfoViewWidget;
        m_singleSelectionPage = m_infoView;

        m_stackedWidget->addWidget(m_noSelectionPage);
        m_stackedWidget->addWidget(m_singleSelectionPage);
        m_stackedWidget->addWidget(m_multipleSelectionPage);

        auto layout = new QVBoxLayout;
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(m_stackedWidget);
        q->setLayout(layout);

        q->setWindowTitle(tr("Info"));
        q->setWindowIcon(Pixmap::fromFont(Theme::AwesomeFont, "\uf27b", Theme::MainToolBarIconSize, Theme::MainToolBarIconColor));
    }

    void onSelectionChanged(const QSet<tutorials::Tutorial*> &selection) {
        if (selection.size() == 0) {
            m_stackedWidget->setCurrentWidget(m_noSelectionPage);
        } else if (selection.size() == 1) {
            m_stackedWidget->setCurrentWidget(m_singleSelectionPage);
            tutorials::Tutorial* t = selection.values()[0];
            m_infoView->setTutorial(t);
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
    return d->m_dockToolBarAction;
}

QList<Action *> InfoWidget::appToolBarActions() const
{
    Q_D(const InfoWidget);
    return d->m_appToolBarAction;
}

void InfoWidget::onSelectionChanged(const QSet<tutorials::Tutorial*> &selection)
{
    Q_D(InfoWidget);
    d->onSelectionChanged(selection);
}

} // namespace ui
} // namespace tc
