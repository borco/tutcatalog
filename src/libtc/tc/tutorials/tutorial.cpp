#include "tutorial.h"

#include <QDebug>
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
    , m_folder(nullptr)
    , m_tableId(InvalidTableId)
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
    int mins = duration % 60;
    int hours = duration / 60;
    return QString(tr("%1h %2m").arg(mins, hours));
}

bool Tutorial::hasCanonicalName() const
{
    return QFileInfo(m_path).fileName() == m_title;
}

} // namespace tutorials
} // namespace tc
