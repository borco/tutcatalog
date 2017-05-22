#include "infowidget.h"

#include "cachedtextedit.h"
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

#include <markdowncxx.h>
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
    QTextEdit* m_edit { nullptr };
    CachedTextEdit* m_infoView { nullptr };
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

        auto viewWidget = new QWidget;
        m_edit = new QTextEdit;
        m_edit->setReadOnly(true);
        m_edit->setFrameStyle(QFrame::NoFrame);

        m_infoView = new CachedTextEdit;
        m_infoView->setReadOnly(true);
        m_infoView->setFrameStyle(QFrame::NoFrame);

        auto viewLayout = new QVBoxLayout;
        viewLayout->setMargin(0);
        viewLayout->addWidget(m_edit);
        viewLayout->addWidget(m_infoView);
        viewWidget->setLayout(viewLayout);

        m_singleSelectionPage = viewWidget;

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

    void updateFromCache(tutorials::Tutorial* tutorial) {
        QString text = QString("Title: <b>%1</b>"
                               "<br>Index: <b>%2</b>")
                .arg(tutorial->title())
                .arg(tutorial->index());

        if (tutorial->hasInfo()) {
            auto infos = tutorial->collection()->cachedInfos(tutorial);
            if (infos.size() == 0) {
                qWarning() << "info expected but not found in cache";
            } else if (infos.size() > 1) {
                qWarning() << "more infos found in cache for the same tutorial";
            } else {
                text += "<br>Contents:<br>info.tc<br>";
                auto images = tutorial->collection()->cachedImages(tutorial);
                if (images.size()) {
                    text += images.keys().join("<br>");
                    CachedTextEdit::CachedResources resources;
                    for (auto i: images) {
                        resources[i.name] = i.data;
                    }
                    m_infoView->setCachedResources(resources);
                }

                // NOTE: info html must be set after configuring resources, if any are present
                std::string html;
                markdown2html(infos.first().data.toStdString(), html);
                m_infoView->setText(QString::fromStdString(html));
            }
        } else {
            m_infoView->setText("");
            m_infoView->setCachedResources(CachedTextEdit::CachedResources());
        }

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
