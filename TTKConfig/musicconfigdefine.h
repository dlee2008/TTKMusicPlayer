#ifndef MUSICCONFIGDEFINE_H
#define MUSICCONFIGDEFINE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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
 ================================================= */

#include "musicobject.h"

#define C_LRC_DIR_FULL            LRC_DIR_FULL
#define C_MUSIC_DIR_FULL          MUSIC_DIR_FULL
#define C_MOVIE_DIR_FULL          MOVIE_DIR_FULL
#define C_UPDATE_DIR_FULL         UPDATE_DIR_FULL
//
#define C_CACHE_DIR_FULL          CACHE_DIR_FULL
#define C_ART_DIR_FULL            ART_DIR_FULL
#define C_BACKGROUND_DIR_FULL     BACKGROUND_DIR_FULL
#define C_SCREEN_DIR_FULL         SCREEN_DIR_FULL
//
#define C_COFIGPATH_FULL          COFIGPATH_FULL
#define C_MUSICPATH_FULL          MUSICPATH_FULL
#define C_NORMALDOWNPATH_FULL     NORMALDOWNPATH_FULL
#define C_CLOUDDOWNPATH_FULL      CLOUDDOWNPATH_FULL
#define C_CLOUDUPPATH_FULL        CLOUDUPPATH_FULL
#define C_MUSICSEARCH_FULL        MUSICSEARCH_FULL
#define C_DARABASEPATH_FULL       DARABASEPATH_FULL
#define C_USERPATH_FULL           USERPATH_FULL
#define C_BARRAGEPATH_FULL        BARRAGEPATH_FULL
#define C_AVATAR_DIR_FULL         AVATAR_DIR_FULL
#define C_USER_THEME_DIR_FULL     USER_THEME_DIR_FULL
//
#define C_TEMPPATH_FULL           TTK_STRCAT(CONFIG_DLL_DIR, TEMPPATH)
#define C_THEME_DIR_FULL          TTK_STRCAT(CONFIG_DLL_DIR, THEME_DIR)
#define C_PLUGINS_DIR_FULL        TTK_STRCAT(CONFIG_DLL_DIR, PLUGINS_DIR)
#define C_LANGUAGE_DIR_FULL       TTK_STRCAT(CONFIG_DLL_DIR, LANGUAGE_DIR)


#ifdef Q_OS_WIN
#define C_TTKSERVICE_FULL         TTK_STRCAT(CONFIG_DLL_DIR, "TTKService.exe")
#else
#define C_TTKINIT_FULL            TTK_STRCAT(CONFIG_DLL_DIR, "../TTKInit.sh")
#define C_TTKMUSICPLAYER_FULL     TTK_STRCAT(CONFIG_DLL_DIR, "../TTKMusicPlayer.sh")
#define C_TTKROUTINE_FULL         TTK_STRCAT(CONFIG_DLL_DIR, "../TTKRoutine.sh")
#define C_TTKCONSOLE_FULL         TTK_STRCAT(CONFIG_DLL_DIR, "../TTKConsole.sh")
#define C_TTKSERVICE_FULL         TTK_STRCAT(CONFIG_DLL_DIR, "TTKService.sh")
#define C_TTKROUTINECOPY_FULL     TTK_STRCAT(CONFIG_DLL_DIR, "TTKRoutineCopy.sh")
#endif

#endif