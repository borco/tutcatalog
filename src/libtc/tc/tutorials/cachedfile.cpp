#include "cachedfile.h"

namespace tc {
namespace tutorials {

bool CachedFile::operator ==(const CachedFile &o) const {
    return name == o.name
            && data == o.data
            && checksum == o.checksum
            && modified == o.modified;
}

bool CachedFile::operator !=(const CachedFile &o) const {
    return ! (*this == o);
}

void CachedFile::operator =(const CachedFile &o) {
    if (this == &o)
        return;
    name = o.name;
    data = o.data;
    checksum = o.checksum;
    modified = o.modified;
}

} // namespace tutorials
} // namespace tc
