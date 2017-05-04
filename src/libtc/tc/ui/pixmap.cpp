#include "pixmap.h"

#include <QDebug>
#include <QFile>
#include <QFontDatabase>
#include <QMap>
#include <QPainter>
#include <QSvgRenderer>

namespace tc {
namespace ui {

namespace {

class Helper
{
public:
    static const QFont& getFont(const QString& fontFile) {
        if (!m_fonts.contains(fontFile)) {
            int id = QFontDatabase::addApplicationFont(fontFile);

            if (id != -1) {
                QString family = QFontDatabase::applicationFontFamilies(id).at(0);
                auto font = QFont(family);
                m_fonts[fontFile] = font;
                qDebug() << "registered icon font:" << fontFile;
            } else {
                qWarning() << "failed to load font:" << fontFile;
                return InvalidFont;
            }
        }

        return m_fonts[fontFile];
    }

private:
    static QFont InvalidFont;
    static QMap<QString, QFont> m_fonts;
};

QFont Helper::InvalidFont;
QMap<QString, QFont> Helper::m_fonts;

} // namespace {}

QPixmap Pixmap::fromFont(const QString &fontFileName, const QString &text, int color, int size)
{
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);

    // Set painting options.
    QPainter painter(&pix);
    QFont painterFont = Helper::getFont(fontFileName);
    painterFont.setPixelSize(size - 2);
    painter.setFont(painterFont);
    painter.setPen(color);

    // Draw icon.
    painter.drawText(QRectF(0, 0, size, size), Qt::AlignCenter, text);
    return pix;
}

QPixmap Pixmap::fromSvg(const QString &svgFileName, int size, int finalColor, int originalColor)
{
    QFile file(svgFileName);
    if (!file.open(QIODevice::ReadOnly))
        return QPixmap();

    QByteArray data = file.readAll();
    if (originalColor != finalColor) {
        data = data.replace(QString("fill=\"#%1\"").arg((QString("%1").arg(originalColor, 6, 16, QChar('0')).toUpper())).toLatin1(),
                            QString("fill=\"#%1\"").arg(finalColor, 6, 16, QChar('0')).toLatin1());
    }

    QImage image (QSize(size, size), QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::transparent);
    QSvgRenderer r(data);
    QPainter p(&image);
    r.render(&p);
    return QPixmap::fromImage(image);
}

} // namespace ui
} // namespace tc
