#pragma once

#include "libtc_global.h"

#include "QQmlVarPropertyHelpers.h"

namespace tc {

namespace tutorials {
class FolderInfo;
}

class ProjectPrivate;
class LIBTCSHARED_EXPORT Project : public QObject
{
    Q_OBJECT

    QML_WRITABLE_VAR_PROPERTY(QList<tc::tutorials::FolderInfo*>, folderInfos)
    QML_WRITABLE_VAR_PROPERTY(QStringList, videoExtensions)
    QML_WRITABLE_VAR_PROPERTY(QString, ffprobePath)
    QML_WRITABLE_VAR_PROPERTY(QString, scrapScriptPath)

public:
    explicit Project(QObject *parent = 0);
    ~Project();

    bool readFromFile(const QString& fileName);
    bool readFromData(const QByteArray& data);

private:
    Q_DECLARE_PRIVATE(Project)
    QScopedPointer<ProjectPrivate> const d_ptr;
};

} // namespace tc
