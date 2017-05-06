#pragma once

#include "libtc_global.h"
#include "QQmlVarPropertyHelpers.h"

namespace tc {
namespace tutorials {

class LIBTCSHARED_EXPORT Tutorial : public QObject
{
    Q_OBJECT

    QML_WRITABLE_VAR_PROPERTY(QString, title)
    QML_WRITABLE_VAR_PROPERTY(QString, publisher)
    QML_WRITABLE_VAR_PROPERTY(QStringList, authors)

public:
    explicit Tutorial(QObject* parent = nullptr);
};

} // namespace tf
} // namespace tc
