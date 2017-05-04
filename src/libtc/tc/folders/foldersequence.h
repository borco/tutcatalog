#pragma once

#include "libtc_global.h"
#include <QObject>

class QAction;

namespace tc {
namespace folders {

class Folder;
class FolderInfo;

class FolderSequencePrivate;
class LIBTCSHARED_EXPORT FolderSequence : public QObject
{
    Q_OBJECT

public:
    explicit FolderSequence(QObject *parent = nullptr);
    ~FolderSequence();

    void setup(const QVector<FolderInfo*>& infos);
    void load();

    QList<QAction*> actions() const;

private:
    Q_DECLARE_PRIVATE(FolderSequence)
    QScopedPointer<FolderSequencePrivate> const d_ptr;
};

} // namespace folders
} // namespace tc
