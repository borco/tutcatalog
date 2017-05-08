#include "tutorial.h"

namespace tc {
namespace tutorials {

Tutorial::Tutorial(QObject *parent)
    : QObject(parent)
    , m_tableId(InvalidTableId)
    , m_isCached(true)
    , m_isReadOnly(true)
    , m_skipBackup(false)
    , m_hasInfo(false)
    , m_onToDoList(false)
    , m_isComplete(false)
    , m_isViewed(false)
    , m_isDeleted(false)
    , m_isOnline(false)
    , m_rating(DefaultRating)
    , m_size(InvalidSize)
{
}

} // namespace tutorials
} // namespace tc
