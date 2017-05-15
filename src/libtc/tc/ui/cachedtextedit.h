#pragma once

#include "libtc_global.h"

#include <QTextEdit>

namespace tc {
namespace ui {

class CachedTextEditPrivate;
class CachedTextEdit : public QTextEdit
{
public:
    explicit CachedTextEdit(QWidget* parent = nullptr);
    ~CachedTextEdit();

    QVariant loadResource(int type, const QUrl &name) override;

    void setCachedResources(const QMap<QString, QByteArray>& data);

private:
    Q_DECLARE_PRIVATE(CachedTextEdit)
    QScopedPointer<CachedTextEditPrivate> const d_ptr;
};

} // namespace ui
} // namespace tc
