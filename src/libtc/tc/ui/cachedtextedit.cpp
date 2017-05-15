#include "cachedtextedit.h"

namespace tc {
namespace ui {

struct CachedTextEditPrivate
{
    QMap<QString, QByteArray> data;
};

CachedTextEdit::CachedTextEdit(QWidget *parent)
    : QTextEdit(parent)
    , d_ptr(new CachedTextEditPrivate)
{
}

CachedTextEdit::~CachedTextEdit()
{
}

QVariant CachedTextEdit::loadResource(int type, const QUrl &name)
{
    Q_D(CachedTextEdit);
    if (type == QTextDocument::ImageResource && d->data.contains(name.url())) {
        QPixmap img;
        img.loadFromData(d->data.value(name.url()));
        return QVariant(img);
    }
    return QTextEdit::loadResource(type, name);
}

void CachedTextEdit::setCachedResources(const QMap<QString, QByteArray> &data)
{
    Q_D(CachedTextEdit);
    d->data = data;
}

} // namespace ui
} // namespace tc
