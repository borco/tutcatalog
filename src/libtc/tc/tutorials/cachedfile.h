#pragma once

#include "libtc_global.h"

#include <QDateTime>
#include <QMap>

namespace tc {
namespace tutorials {

struct LIBTCSHARED_EXPORT CachedFile
{
    QString name;
    QByteArray data;
    QString checksum; // crc32
    QDateTime modified;

    bool operator ==(const CachedFile& o) const;
    bool operator !=(const CachedFile& o) const;
    void operator =(const CachedFile& o);
};

typedef QMap<QString, CachedFile> CachedFiles;

} // namespace tutorials
} // namespace tc
