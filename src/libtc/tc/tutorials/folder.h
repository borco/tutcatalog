#pragma once

#include "libtc_global.h"
#include <QObject>

namespace tc {
namespace tutorials {

class FolderInfo;
class Tutorial;

class FolderPrivate;
class LIBTCSHARED_EXPORT Folder : public QObject
{
    Q_OBJECT

public:
    explicit Folder(QObject *parent = nullptr);
    ~Folder();

    void setup(const FolderInfo* info);
    void load();

    const FolderInfo* info() const;

    bool noBackup(const Tutorial* tutorial) const;

signals:
    void loaded(Tutorial* tutorial);

private:
    Q_DECLARE_PRIVATE(Folder)
    QScopedPointer<FolderPrivate> const d_ptr;
};

} // namespace tutorials
} // namespace tc
