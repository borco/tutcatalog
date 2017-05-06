#include "tutorial.h"

namespace tc {
namespace tutorials {

Tutorial::Tutorial(QObject *parent)
    : QObject(parent)
    , m_isCached(true)
    , m_isReadOnly(false)
{
}

} // namespace tutorials
} // namespace tc
