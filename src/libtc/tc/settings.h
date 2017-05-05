#pragma once

#include "libtc_global.h"
#include <QScopedPointer>
#include <QVariant>

namespace tc {

class SettingsPrivate;
class LIBTCSHARED_EXPORT Settings
{
public:
    explicit Settings();
    ~Settings();

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    int beginReadArray(const QString &prefix);
    void beginWriteArray(const QString &prefix, int size = -1);
    void endArray();
    void setArrayIndex(int i);

    void beginGroup(const QString &prefix);
    void endGroup();
    QString group() const;

    void sync();

    static void setFileName(const QString& fileName);
    QString fileName() const;

private:
    Q_DECLARE_PRIVATE(Settings)
    QScopedPointer<SettingsPrivate> const d_ptr;
};

} // namespace tc
