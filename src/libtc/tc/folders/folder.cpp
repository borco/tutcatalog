#include "folder.h"
#include "folderinfo.h"

#include <QDebug>

namespace tc {
namespace folders {

class FolderPrivate : public QObject
{
    Q_DECLARE_PUBLIC(Folder)
    Folder* const q_ptr { nullptr };
    FolderInfo m_info;
    bool m_isRefreshing;

    FolderPrivate(Folder* ptr) : q_ptr(ptr) {}

    void setup(const FolderInfo* info) {
        if (info) {
            m_info = *info;
            qDebug() << "- tutorial folder - setup:" << m_info.name();
        } else {
            m_info.clear();
        }
    }

    void load() {
        qDebug() << "- tutorial folder - load:" << m_info.name();
    }

    void startRefresh() {
        // TODO: startRefresh
    }

    void cancelRefresh() {
        // TODO: cancelRefresh
    }
};

Folder::Folder(QObject *parent)
    : QObject(parent)
    , d_ptr(new FolderPrivate(this))
{
}

Folder::~Folder()
{
}

void Folder::setup(const FolderInfo *info)
{
    Q_D(Folder);
    d->setup(info);
}

void Folder::load()
{
    Q_D(Folder);
    d->load();
}

const FolderInfo *Folder::info() const
{
    Q_D(const Folder);
    return &d->m_info;
}

void Folder::startRefresh()
{
    Q_D(Folder);
    d->startRefresh();
}

void Folder::cancelRefresh()
{
    Q_D(Folder);
    d->cancelRefresh();
}

bool Folder::isRefreshing() const
{
    Q_D(const Folder);
    return d->m_isRefreshing;
}

} // namespace folders
} // namespace tc
