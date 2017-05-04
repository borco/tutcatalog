#pragma once

#include "libtc_global.h"

#include <QPixmap>

namespace tc {
namespace ui {

class Pixmap {
public:
    /*!
     * \brief Create a pixmap from a font.
     *
     * \param fontFileName: the file with the font
     * \param text: text to be rendered (normaly, some Unicode char)
     * \param size: size of the icon
     * \param color: color of the icon
     * \return
     */
    static QPixmap fromFont(const QString& fontFileName,
                            const QString& text,
                            int size,
                            QRgb color = 0xffffff);

    /*!
     * \brief Create a pixmap from a SVG, after replacing some color with another one.
     *
     * \param svgFileName: the file with the SVG
     * \param size: the size of the icon
     * \param finalColor: desired color, to be used by the pixmap (eg: 0x888888)
     * \param originalColor: original color, as found in the SVG (eg: 0xffffff)
     * \return
     */
    static QPixmap fromSvg(const QString& svgFileName,
                           int size,
                           QRgb finalColor = 0xffffff,
                           QRgb originalColor = 0xffffff);

};

} // namespace ui
} // namespace tc
