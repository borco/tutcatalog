#pragma once

#include "libtc_global.h"

namespace tc {

class Settings;

class LIBTCSHARED_EXPORT Persistent
{
public:
    virtual ~Persistent() {}

    virtual void saveSettings(Settings& settings) const = 0;
    virtual void loadSettings(const Settings& settings) = 0;
};

} // namespace tc
