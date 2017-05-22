#include "tutorial.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>

namespace {
static const int MinRating = -5;
static const int MaxRating = 5;
static const int DefaultRating = 0;
}

namespace tc {
namespace tutorials {

Tutorial::Tutorial(QObject *parent)
    : QObject(parent)
    , m_folderInfo(nullptr)
    , m_index(InvalidTableId)
    , m_isCached(true)
    , m_isReadOnly(true)
    , m_hasInfo(false)
    , m_hasChecksum(false)
    , m_onToDoList(false)
    , m_onKeepList(false)
    , m_isComplete(false)
    , m_noBackup(false)
    , m_isViewed(false)
    , m_isDeleted(false)
    , m_isOnline(false)
    , m_duration(0)
    , m_rating(DefaultRating)
    , m_size(InvalidSize)
{
    connect(this, &Tutorial::pathChanged, this, &Tutorial::canonicalPathChanged);
    connect(this, &Tutorial::titleChanged, this, &Tutorial::canonicalPathChanged);
}

QString Tutorial::fileSizeAsString(qint64 size)
{
    if (size == 0)
        return QString();

    double kilo = 1024;
    double s = size;

    if (s < kilo)
        return QString("%1").arg(s);

    s /= kilo;
    if (s < kilo)
        return QString("%1 K").arg(s, 0, 'f', 2);

    s /= kilo;
    if (s < kilo)
        return QString("%1 M").arg(s, 0, 'f',  2);

    s /= kilo;
    return QString("%1 G").arg(s, 0, 'f',  2);
}

QString Tutorial::ratingAsString(int rating)
{
    rating = qBound(MinRating, rating, MaxRating);
    rating -= DefaultRating;
    return QString(rating >= 0 ? "★" : "☆").repeated(qAbs(rating));
}

QString Tutorial::durationAsString(int duration)
{
    int minutes = duration % 60;
    int hours = duration / 60;
    return duration > 0 ? ( hours > 0
                            ? QString("%1h %2m").arg(hours).arg(minutes)
                            : QString("%1m").arg(minutes)
                              )
                        : "";

}

bool Tutorial::hasCanonicalPath() const
{
    return m_path == canonicalPath();
}

QString Tutorial::canonicalPath() const
{
    return QFileInfo(m_path).dir().filePath(m_title);
}

} // namespace tutorials
} // namespace tc
