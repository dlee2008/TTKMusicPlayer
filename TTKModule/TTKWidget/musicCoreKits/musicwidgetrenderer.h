#ifndef MUSICWIDGETRENDERER_H
#define MUSICWIDGETRENDERER_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QLabel>
#include "musicwidgetutils.h"
#include "musicsinglemanager.h"

/*! @brief The class of the widget background renderer.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWidgetRenderer
{
    TTK_DECLARE_MODULE(MusicWidgetRenderer)
public:
    /*!
     * Object constructor.
     */
    MusicWidgetRenderer();

    /*!
     * Set background label.
     */
    inline void setBackgroundLabel(QLabel *label) noexcept { m_background = label; }

protected:
    /*!
     * Set background pixmap.
     */
    void setBackgroundPixmap(const QSize &size);

private:
    QLabel *m_background;

};

#endif // MUSICWIDGETRENDERER_H
