#pragma once

#include "libtc_global.h"
#include <QObject>

namespace tc {

namespace tutorials {
class Tutorial;
}

namespace folders {

class FolderInfo;

class FolderPrivate;
class LIBTCSHARED_EXPORT Folder : public QObject
{
    Q_OBJECT

public:
    explicit Folder(QObject *parent = nullptr);
    ~Folder();

    void setup(const FolderInfo* info);
    void load();

    void startRefresh();
    void cancelRefresh();

    const FolderInfo* info() const;
    bool isRefreshing() const;

signals:
    void loaded(tutorials::Tutorial* tutorial);
    void refreshed(int total, int progress);

private:
    Q_DECLARE_PRIVATE(Folder)
    QScopedPointer<FolderPrivate> const d_ptr;
};

} // namespace folders
} // namespace tc
