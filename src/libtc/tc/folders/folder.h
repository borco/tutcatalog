#pragma once

#include "libtc_global.h"
#include <QObject>

namespace tc {
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
    void refreshed();

private:
    Q_DECLARE_PRIVATE(Folder)
    QScopedPointer<FolderPrivate> const d_ptr;
};

} // namespace tf
} // namespace tc
