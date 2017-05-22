#pragma once

#include "libtc_global.h"

#include <QTextEdit>

namespace tc {
namespace ui {

class CachedTextEditPrivate;
class CachedTextEdit : public QTextEdit
{
public:
    typedef QMap<QString, QByteArray> CachedResources;
    explicit CachedTextEdit(QWidget* parent = nullptr);
    ~CachedTextEdit();

    QVariant loadResource(int type, const QUrl &name) override;

    void setCachedResources(const CachedResources& data);
    void clear();

private:
    Q_DECLARE_PRIVATE(CachedTextEdit)
    QScopedPointer<CachedTextEditPrivate> const d_ptr;
};

} // namespace ui
} // namespace tc
