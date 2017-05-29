#include "infoviewwidget.h"
#include "cachedtextedit.h"
#include "tc/tutorials/collection.h"
#include "tc/tutorials/folderinfo.h"
#include "tc/tutorials/tutorial.h"

#include <QDebug>
#include <QFormLayout>
#include <QLabel>
#include <QScrollArea>

#include <markdowncxx.h>
#include <functional>

namespace tc {
namespace ui {

using namespace tutorials;

class InfoViewWidgetPrivate : public QObject
{

    Q_DECLARE_PUBLIC(InfoViewWidget)
    InfoViewWidget* const q_ptr { nullptr };
    Tutorial* m_tutorial { nullptr };

    QLabel* m_isCachedLabel { nullptr };
    QLabel* m_pathLabel { nullptr };
    QLabel* m_canonicalPathLabel { nullptr };
    QLabel* m_parentPathLabel { nullptr };
    QLabel* m_publisherLabel { nullptr };
    QLabel* m_titleLabel { nullptr };
    QLabel* m_authorsLabel { nullptr };
    QLabel* m_urlLabel { nullptr };
    QLabel* m_onToDoListLabel { nullptr };
    QLabel* m_onKeepListLabel { nullptr };
    QLabel* m_isCompleteLabel { nullptr };
    QLabel* m_isViewedLabel { nullptr };
    QLabel* m_isDeletedLabel { nullptr };
    QLabel* m_noBackupLabel { nullptr };
    QLabel* m_durationLabel { nullptr };
    QLabel* m_ratingLabel { nullptr };
    QLabel* m_sizeLabel { nullptr };
    QLabel* m_levelsLabel { nullptr };
    QLabel* m_createdLabel { nullptr };
    QLabel* m_modifiedLabel { nullptr };
    QLabel* m_releasedLabel { nullptr };
    QLabel* m_tagsLabel { nullptr };
    QLabel* m_extraTagsLabel { nullptr };
    QLabel* m_learningPathsLabel { nullptr };
    CachedTextEdit* m_descriptionWidget { nullptr };

    explicit InfoViewWidgetPrivate(InfoViewWidget* ptr) : q_ptr(ptr) {
        setupUi();
    }

    void setupUi() {
        const int Spacing { 6 };

        Q_Q(InfoViewWidget);
        auto mainLayout = new QVBoxLayout;
        mainLayout->setSpacing(Spacing);

        auto formLayout = new QFormLayout;
        formLayout->setMargin(0);
        formLayout->setSpacing(Spacing);

        formLayout->addRow(tr("From cache:"), m_isCachedLabel = new QLabel);
        formLayout->addRow(tr("Tutorial:"), m_pathLabel = new QLabel);
        formLayout->addRow(tr("Canonical:"), m_canonicalPathLabel = new QLabel);
        formLayout->addRow(tr("Location:"), m_parentPathLabel = new QLabel);
        formLayout->addRow(tr("Publisher:"), m_publisherLabel = new QLabel);
        formLayout->addRow(tr("Title:"), m_titleLabel = new QLabel);
        formLayout->addRow(tr("Authors:"), m_authorsLabel = new QLabel);
        formLayout->addRow(tr("Url:"), m_urlLabel = new QLabel);
        formLayout->addRow(tr("To do:"), m_onToDoListLabel = new QLabel);
        formLayout->addRow(tr("Keep:"), m_onKeepListLabel = new QLabel);
        formLayout->addRow(tr("Complete:"), m_isCompleteLabel = new QLabel);
        formLayout->addRow(tr("Viewed:"), m_isViewedLabel = new QLabel);
        formLayout->addRow(tr("Deleted:"), m_isDeletedLabel = new QLabel);
        formLayout->addRow(tr("Backup:"), m_noBackupLabel = new QLabel);
        formLayout->addRow(tr("Duration:"), m_durationLabel = new QLabel);
        formLayout->addRow(tr("Rating:"), m_ratingLabel = new QLabel);
        formLayout->addRow(tr("Size:"), m_sizeLabel = new QLabel);
        formLayout->addRow(tr("Levels:"), m_levelsLabel = new QLabel);
        formLayout->addRow(tr("Created:"), m_createdLabel = new QLabel);
        formLayout->addRow(tr("Modified:"), m_modifiedLabel = new QLabel);
        formLayout->addRow(tr("Released:"), m_releasedLabel = new QLabel);
        formLayout->addRow(tr("Tags:"), m_tagsLabel = new QLabel);
        formLayout->addRow(tr("Extra tags:"), m_extraTagsLabel = new QLabel);
        formLayout->addRow(tr("Learning paths:"), m_learningPathsLabel = new QLabel);

        m_descriptionWidget = new CachedTextEdit;
        m_descriptionWidget->setFrameStyle(QFrame::NoFrame);
        m_descriptionWidget->setReadOnly(true);
        m_descriptionWidget->viewport()->setAutoFillBackground(false);

        m_descriptionWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        auto scroll = new QScrollArea;
        scroll->setWidget(m_descriptionWidget);
        scroll->setWidgetResizable(true);
        scroll->setFrameStyle(QFrame::NoFrame);

        mainLayout->addItem(formLayout);
        mainLayout->addWidget(new QLabel(tr("Description:")));
        mainLayout->addWidget(scroll);

        q->setLayout(mainLayout);
    }

    void connectProperty(QLabel* label,
                         QString (Tutorial::*getter)() const,
                         void (Tutorial::*notifier)(QString)) {
        if (m_tutorial) {
            connect(m_tutorial, notifier, this, [=](QString v){ label->setText(v); });
            label->setText((m_tutorial->*getter)());
        } else {
            label->clear();
        }
    }

    void connectProperty(QLabel* label,
                         QString (Tutorial::*getter)() const,
                         void (Tutorial::*notifier)(QString),
                         QString (*adapter) (Tutorial*, QString)) {
        if (m_tutorial) {
            connect(m_tutorial, notifier, this, [=](QString v){ label->setText((*adapter)(m_tutorial, v)); });
            label->setText((*adapter)(m_tutorial, (m_tutorial->*getter)()));
        } else {
            label->clear();
        }
    }

    void connectProperty(QLabel* label,
                         QStringList (Tutorial::*getter)() const,
                         void (Tutorial::*notifier)(QStringList)) {
        if (m_tutorial) {
            auto adapter = [](const QStringList& l) { return l.join(", "); };
            connect(m_tutorial, notifier, this, [=](QStringList v){ label->setText(adapter(v)); });
            label->setText(adapter((m_tutorial->*getter)()));
        } else {
            label->clear();
        }
    }

    void connectProperty(QLabel* label,
                         qint64 (Tutorial::*getter)() const,
                         void (Tutorial::*notifier)(qint64),
                         std::function<QString(qint64)> adapter) {
        if (m_tutorial) {
            connect(m_tutorial, notifier, this, [=](qint64 v){ label->setText(adapter(v)); });
            label->setText(adapter((m_tutorial->*getter)()));
        } else {
            label->clear();
        }
    }

    void connectProperty(QLabel* label,
                         QDateTime (Tutorial::*getter)() const,
                         void (Tutorial::*notifier)(QDateTime)) {
        if (m_tutorial) {
            auto adapter = [](QDateTime v) { return v.toString(tr("yyyy/MM/dd hh:mm:ss")); };
            connect(m_tutorial, notifier, this, [=](QDateTime v){ label->setText(adapter(v)); });
            label->setText(adapter((m_tutorial->*getter)()));
        } else {
            label->clear();
        }
    }

    void connectProperty(QLabel* label,
                         bool (Tutorial::*getter)() const,
                         void (Tutorial::*notifier)(bool),
                         QString yesValue = tr("Yes"),
                         QString noValue = tr("")) {
        if (m_tutorial) {
            auto adapter = [=](bool v) { return v ? yesValue : noValue; };
            connect(m_tutorial, notifier, this, [=](bool v){ label->setText(adapter(v)); });
            label->setText(adapter((m_tutorial->*getter)()));
        } else {
            label->clear();
        }
    }

    void connectProperty(QLabel* label,
                         bool (Tutorial::*getter)() const,
                         void (FolderInfo::*notifier)(QString),
                         QString yesValue = tr("Yes"),
                         QString noValue = tr("")) {
        if (m_tutorial) {
            auto adapter = [=](bool v) { return v ? yesValue : noValue; };
            connect(m_tutorial->folderInfo(), notifier, this, [=](){ label->setText(adapter((m_tutorial->*getter)())); });
            label->setText(adapter((m_tutorial->*getter)()));
        } else {
            label->clear();
        }
    }

    void updateDescription() {
        if (m_tutorial->hasInfo()) {
            auto images = m_tutorial->images();
            if (images.size()) {
                CachedTextEdit::CachedResources resources;
                for (auto i: images) {
                    resources[i.name] = i.data;
                }
                m_descriptionWidget->setCachedResources(resources);
            }

            // NOTE: info html must be set after configuring resources, if any are present
            auto info = m_tutorial->info();
            std::string html;
            markdown2html(info.data.toStdString(), html);
            m_descriptionWidget->setText(QString::fromStdString(html));
        } else {
            m_descriptionWidget->clear();
        }
    }

    void setTutorial(Tutorial* tutorial) {
        if (m_tutorial == tutorial)
            return;

        if (m_tutorial) {
            m_tutorial->disconnect(this);
        }

        m_tutorial = tutorial;

        connectProperty(m_isCachedLabel, &Tutorial::isCached, &FolderInfo::cachePathChanged, tr("<font color=\"red\">YES</font>"));

        connectProperty(m_pathLabel, &Tutorial::path, &Tutorial::pathChanged,
                        [](Tutorial* tutorial, QString v){
            return tutorial->hasCanonicalPath() ? v : "<font color=\"red\">" + v + "</font>";
        });
        connectProperty(m_canonicalPathLabel, &Tutorial::canonicalPath, &Tutorial::canonicalPathChanged,
                        [](Tutorial* tutorial, QString) {
            return tutorial->hasCanonicalPath() ? "" : tutorial->canonicalPath();
        });

        connectProperty(m_titleLabel, &Tutorial::title, &Tutorial::titleChanged);
        connectProperty(m_publisherLabel, &Tutorial::publisher, &Tutorial::publisherChanged);
        connectProperty(m_authorsLabel, &Tutorial::authors, &Tutorial::authorsChanged);
        connectProperty(m_urlLabel, &Tutorial::url, &Tutorial::urlChanged);
        connectProperty(m_onToDoListLabel, &Tutorial::onToDoList, &Tutorial::onToDoListChanged);
        connectProperty(m_onKeepListLabel, &Tutorial::onKeepList, &Tutorial::onKeepListChanged);
        connectProperty(m_isCompleteLabel, &Tutorial::isComplete, &Tutorial::isCompleteChanged, tr(""), tr("<font color=\"red\">NO</font>"));
        connectProperty(m_isViewedLabel, &Tutorial::isViewed, &Tutorial::isViewedChanged);
        connectProperty(m_isDeletedLabel, &Tutorial::isDeleted, &Tutorial::isDeletedChanged);
        connectProperty(m_durationLabel, &Tutorial::duration, &Tutorial::durationChanged, [=](qint64 v) { return Tutorial::durationAsString(v); });
        connectProperty(m_ratingLabel, &Tutorial::rating, &Tutorial::ratingChanged, [=](qint64 v) -> QString { return Tutorial::ratingAsString(v); });
        connectProperty(m_sizeLabel, &Tutorial::size, &Tutorial::sizeChanged, [=](qint64 v) { return Tutorial::fileSizeAsString(v); });
        connectProperty(m_levelsLabel, &Tutorial::levels, &Tutorial::levelsChanged);
        connectProperty(m_createdLabel, &Tutorial::created, &Tutorial::createdChanged);
        connectProperty(m_modifiedLabel, &Tutorial::modified, &Tutorial::modifiedChanged);
        connectProperty(m_releasedLabel, &Tutorial::released, &Tutorial::releasedChanged);
        connectProperty(m_tagsLabel, &Tutorial::tags, &Tutorial::tagsChanged);
        connectProperty(m_extraTagsLabel, &Tutorial::extraTags, &Tutorial::extraTagsChanged);
        connectProperty(m_learningPathsLabel, &Tutorial::learningPaths, &Tutorial::learningPathsChanged);

        connect(m_tutorial, &Tutorial::hasInfoChanged, this, &InfoViewWidgetPrivate::updateDescription);
        updateDescription();
    }
};

InfoViewWidget::InfoViewWidget(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new InfoViewWidgetPrivate(this))
{
}

InfoViewWidget::~InfoViewWidget()
{
}

void InfoViewWidget::setTutorial(tutorials::Tutorial *tutorial)
{
    Q_D(InfoViewWidget);
    d->setTutorial(tutorial);
}

} // namespace ui
} // namespace tc
