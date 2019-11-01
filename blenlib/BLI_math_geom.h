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
#ifdef __cplusplus
extern "C" {
#endif

#include "BLI_math_vector.h"

void cross_tri_v3(float n[3], const float v1[3], const float v2[3], const float v3[3]);
void cross_tri_v3_internalsse(float n[3], const float v1[3], const float v2[3], const float v3[3]);
void cross_tri_v3_sse(xmmvecf *n, const xmmvecf v1, const xmmvecf v2, const xmmvecf v3);

float normal_tri_v3(float r[3], const float a[3], const float b[3], const float c[3]);
float normal_tri_v3_internalsse(float r[3], const float a[3], const float b[3], const float c[3]);
float normal_tri_v3_sse(xmmvecf *n, const xmmvecf v1, const xmmvecf v2, const xmmvecf v3);

#ifdef __cplusplus
}
#endif
