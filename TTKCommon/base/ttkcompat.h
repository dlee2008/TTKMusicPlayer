#ifndef TTKCOMPAT_H
#define TTKCOMPAT_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkglobal.h"

namespace TTK
{
template <typename _Tp>
inline constexpr const _Tp& min(const _Tp &a, const _Tp &b) noexcept
{
    return (a < b) ? a : b;
}

template <typename _Tp>
inline constexpr const _Tp& max(const _Tp &a, const _Tp &b) noexcept
{
    return (a < b) ? b : a;
}
}

namespace std
{
#if !TTK_HAS_CXX14
/// Alias template for remove_extent
template <typename _Tp>
using remove_extent_t = typename remove_extent<_Tp>::type;

/// Alias template for remove_all_extents
template <typename _Tp>
using remove_all_extents_t = typename remove_all_extents<_Tp>::type;
#endif

#if !defined(_MSC_VER) && !TTK_HAS_CXX14
/// Assign @p __new_val to @p __obj and return its previous value.
template <typename _Tp, typename _Up = _Tp>
inline _Tp exchange(_Tp &__obj, _Up &&__new_val) noexcept
{
    _Tp __old_val = std::move(__obj);
    __obj = std::forward<_Up>(__new_val);
    return __old_val;
}
#endif

#if !TTK_HAS_CXX17
template <typename _Tp>
inline constexpr const _Tp& clamp(const _Tp &_Val, const _Tp &_Min_val, const _Tp &_Max_val) noexcept
{
    return TTK::max(_Min_val, TTK::min(_Max_val, _Val));
}
#endif
}

#endif // TTKCOMPAT_H
