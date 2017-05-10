#pragma once

#include "libtc_global.h"
#include "QQmlVarPropertyHelpers.h"

#include <QDateTime>

namespace tc {

namespace folders {
class Folder;
}

namespace tutorials {

class LIBTCSHARED_EXPORT Tutorial : public QObject
{
    Q_OBJECT

    QML_WRITABLE_VAR_PROPERTY(tc::folders::Folder*, folder) // folder that owns this tutorial
    QML_WRITABLE_VAR_PROPERTY(int, tableId)         // an ID to identify the tutorial in the cache (it's id/rowid value)

    QML_WRITABLE_VAR_PROPERTY(bool, isCached)
    QML_WRITABLE_VAR_PROPERTY(bool, isReadOnly)

    QML_WRITABLE_VAR_PROPERTY(QString, path)        // the full disk path
    QML_WRITABLE_VAR_PROPERTY(QString, title)
    QML_WRITABLE_VAR_PROPERTY(QString, publisher)
    QML_WRITABLE_VAR_PROPERTY(QStringList, authors)

    QML_WRITABLE_VAR_PROPERTY(bool, hasInfo)        // true: the info.tc exists
    QML_WRITABLE_VAR_PROPERTY(bool, hasChecksum)    // true: the info.sfv exists
    QML_WRITABLE_VAR_PROPERTY(bool, onToDoList)     // true: it is on your ToDo list
    QML_WRITABLE_VAR_PROPERTY(bool, onKeepList)     // true: it is on your Keep list (should not be able to delete the tutorial from the app)
    QML_WRITABLE_VAR_PROPERTY(bool, isComplete)     // true: you have all modules of this tutorial
    QML_WRITABLE_VAR_PROPERTY(bool, isViewed)       // true: you have viewed this tutorial
    QML_WRITABLE_VAR_PROPERTY(bool, isDeleted)      // true: you've had this tutorial, but deleted its files (except info*, image* and cover*)
    QML_WRITABLE_VAR_PROPERTY(bool, isOnline)       // true: this was never stored locally - all info is online
    QML_WRITABLE_VAR_PROPERTY(bool, noBackup)     // true: this tutorial isn't backed up
    QML_WRITABLE_VAR_PROPERTY(int, duration)        // total tutorial duration, in minutes
    QML_WRITABLE_VAR_PROPERTY(int, rating)          // -5 .. +5: your rating
    QML_WRITABLE_VAR_PROPERTY(qint64, size)         // total size on disk (in kB)
    QML_WRITABLE_VAR_PROPERTY(QStringList, levels)  // beginner, intermediate, advanced (or combinations or none)

    QML_WRITABLE_VAR_PROPERTY(QDateTime, created)   // when was the info.tc created
    QML_WRITABLE_VAR_PROPERTY(QDateTime, modified)  // when was the info.tc modified
    QML_WRITABLE_VAR_PROPERTY(QDateTime, released)  // when was the tutorial published

    QML_WRITABLE_VAR_PROPERTY(QStringList, tags)      // tags filled in by the scrapper (automatically)
    QML_WRITABLE_VAR_PROPERTY(QStringList, extraTags) // tags added manually
    QML_WRITABLE_VAR_PROPERTY(QStringList, learningPaths)

public:
    static const int InvalidTableId { -1 };
    static const int InvalidSize { -1 };

    static const int DefaultRating { 0 };

    explicit Tutorial(QObject* parent = nullptr);

    static QString fileSizeAsString(qint64 size);
    static QString ratingAsString(int rating);
    static QString durationAsString(int duration);
    static QString fileSizeToDurationAsString(qint64 size, int duration);

    bool hasCanonicalName() const;
};

} // namespace tutorials
} // namespace tc
