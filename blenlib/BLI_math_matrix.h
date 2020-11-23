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
 * */

#pragma once

/** \file
 * \ingroup bli
 */

#ifdef __cplusplus
extern "C" {
#endif

void add_m3_m3m3(float R[3][3], const float A[3][3], const float B[3][3]);
void add_m3_m3m3_sse(float R[3][3], const float A[3][3], const float B[3][3]);

void sub_m3_m3m3(float R[3][3], const float A[3][3], const float B[3][3]);
void sub_m3_m3m3_sse(float R[3][3], const float A[3][3], const float B[3][3]);

void mul_v3_m3v3(float r[3], const float M[3][3], const float a[3]);
void mul_v3_m3v3_sse(float r[3], const float M[3][3], const float a[3]);

#ifdef __cplusplus
}
#endif