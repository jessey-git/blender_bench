/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: some of this file.
 *
 * */

#pragma once

#define BLI_MATH_DO_INLINE 1

#ifdef _MSC_VER
#  define MINLINE static __forceinline
#  define MALWAYS_INLINE MINLINE
#  define ATTR_ALIGN(x) __declspec(align(x))
#else
#  define MINLINE static inline
#  define MALWAYS_INLINE static inline __attribute__((always_inline)) __attribute__((unused))
#  define ATTR_ALIGN(x) __attribute__((aligned(__VA_ARGS__)))
#endif