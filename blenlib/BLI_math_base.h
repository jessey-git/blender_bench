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

#include "BLI_math_inline.h"

MINLINE float min_ff(float a, float b);
MINLINE float max_ff(float a, float b);

MINLINE int min_ii(int a, int b);
MINLINE int max_ii(int a, int b);

MINLINE int clamp_i(int value, int min, int max);
MINLINE float clamp_f(float value, float min, float max);

#include "math_base_inline.c"
