#pragma once

#include "libtc_global.h"
#include "QQmlVarPropertyHelpers.h"

namespace tc {
namespace folders {

class LIBTCSHARED_EXPORT FolderInfo : public QObject
{
    Q_OBJECT

    // the path for this tutorial folder
    QML_WRITABLE_VAR_PROPERTY(QString, path)

    // the location of the cache
    // if empty, no cache is created for this tutorial folder
    // if this is set, but the path is empty, this is assumed to be a read-only cache from another machine
    QML_WRITABLE_VAR_PROPERTY(QString, cachePath)

    // some name for this tutorial folder
    QML_WRITABLE_VAR_PROPERTY(QString, name)

    // the path where to store tutorials that don't need backup
    QML_WRITABLE_VAR_PROPERTY(QString, skipBackupPath)

    // the path where to put deleted tutorials
    QML_WRITABLE_VAR_PROPERTY(QString, trashPath)

    // * true if this folder contains copies
    // * false if this contains the original tutorials
    QML_WRITABLE_VAR_PROPERTY(bool, withCopies)

public:
    explicit FolderInfo(QObject *parent = 0);

    bool isValid() const { return !m_cachePath.isEmpty() || !m_path.isEmpty(); }
    bool isCached() const { return !m_cachePath.isEmpty(); }
    bool canUpdate() const { return !m_path.isEmpty(); }

    void clear();

    FolderInfo& operator=(const FolderInfo& other);
    bool operator==(const FolderInfo& other) const;
};

} // namespace folders
} // namespace tc
