#pragma once

#include "libtc_global.h"
#include "QQmlVarPropertyHelpers.h"

#include <QDateTime>

namespace tc {
namespace tutorials {

class LIBTCSHARED_EXPORT Tutorial : public QObject
{
    Q_OBJECT

    QML_WRITABLE_VAR_PROPERTY(QString, cache)       // an path to the cache file
    QML_WRITABLE_VAR_PROPERTY(int, tableId)         // an ID to identify the tutorial in the cache (it's id/rowid value)

    QML_WRITABLE_VAR_PROPERTY(bool, isCached)
    QML_WRITABLE_VAR_PROPERTY(bool, isReadOnly)

    // true: this will not be backed up
    // * informative;
    // * determined by comparing the skip path from the project with the actual path
    QML_WRITABLE_VAR_PROPERTY(bool, skipBackup)

    QML_WRITABLE_VAR_PROPERTY(QString, group)       // this is the name of the tutorial path as defined in the project file
    QML_WRITABLE_VAR_PROPERTY(QString, path)        // the full disk path
    QML_WRITABLE_VAR_PROPERTY(QString, title)
    QML_WRITABLE_VAR_PROPERTY(QString, publisher)
    QML_WRITABLE_VAR_PROPERTY(QStringList, authors)

    QML_WRITABLE_VAR_PROPERTY(bool, hasInfo)        // true: the info.tc exists
    QML_WRITABLE_VAR_PROPERTY(bool, onToDoList)     // true: it is on your ToDo list
    QML_WRITABLE_VAR_PROPERTY(bool, isComplete)     // true: you have all modules of this tutorial
    QML_WRITABLE_VAR_PROPERTY(bool, isViewed)       // true: you have viewed this tutorial
    QML_WRITABLE_VAR_PROPERTY(bool, isDeleted)      // true: you've had this tutorial, but deleted its files (except info*, image* and cover*)
    QML_WRITABLE_VAR_PROPERTY(bool, isOnline)       // true: this was never stored locally - all info is online
    QML_WRITABLE_VAR_PROPERTY(QString, duration)    // total tutorial duration (ex: 1h 45m, 15m, 30h 5m)
    QML_WRITABLE_VAR_PROPERTY(QStringList, levels)  // beginner, intermediate, advanced (or combinations or none)
    QML_WRITABLE_VAR_PROPERTY(int, rating)          // -5 .. +5: your rating
    QML_WRITABLE_VAR_PROPERTY(int, size)            // total size on disk (in kB)

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
};

} // namespace tutorials
} // namespace tc
