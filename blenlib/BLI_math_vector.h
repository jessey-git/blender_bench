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

#include "BLI_math_inline.h"

#include <xmmintrin.h>
#include <smmintrin.h>

/* -------------------------------------- */

MINLINE void zero_v3(float r[3]);

MINLINE void copy_v2_v2(float r[2], const float a[2]);
MINLINE void copy_v3_v3(float r[3], const float a[3]);

MINLINE void add_v3_v3(float r[3], const float a[3]);

MINLINE void sub_v2_v2(float r[2], const float a[2]);
MINLINE void sub_v2_v2v2(float r[2], const float a[2], const float b[2]);
MINLINE void sub_v3_v3(float r[3], const float a[3]);
MINLINE void sub_v3_v3v3(float r[3], const float a[3], const float b[3]);

MINLINE void mul_v3_fl(float r[3], float f);
MINLINE void mul_v3_v3fl(float r[3], const float a[3], float f);

MINLINE void madd_v3_v3fl(float r[3], const float a[3], float f);
MINLINE void madd_v3_v3v3fl(float r[3], const float a[3], const float b[3], float f);

MINLINE float dot_v3v3(const float a[3], const float b[3]);

MINLINE void cross_v3_v3v3(float r[3], const float a[3], const float b[3]);

MINLINE float normalize_v3_v3_length(float r[3], const float a[3], const float unit_length);
MINLINE float normalize_v3_v3(float r[3], const float a[3]);
MINLINE float normalize_v3(float n[3]);

/* -------------------------------------- */
#if 1
#  define VECTORCALL _vectorcall
#else
#  define VECTORCALL
#endif

MINLINE __m128 VECTORCALL load_m128_f1(const float f);
MINLINE __m128 VECTORCALL load_m128_f3(const float v[3]);
MINLINE __m128 VECTORCALL load_m128_f4(const float v[4]);
MINLINE void VECTORCALL store_f3_m128(float r[3], const __m128 a);
MINLINE void VECTORCALL store_f4_m128(float r[4], const __m128 a);

MINLINE void VECTORCALL zero_m128(__m128 *r);

MINLINE void VECTORCALL add_m128_m128(__m128 *r, const __m128 a);

MINLINE void VECTORCALL sub_m128_m128m128(__m128 *r, const __m128 a, const __m128 b);
MINLINE void VECTORCALL sub_m128_m128(__m128 *r, const __m128 a);

MINLINE void VECTORCALL mul_m128_fl(__m128 *r, const float f);
MINLINE void VECTORCALL mul_m128_m128fl(__m128 *r, const __m128 a, const float f);

MINLINE void VECTORCALL madd_m128_m128fl(__m128 *r, const __m128 a, const float f);
MINLINE void VECTORCALL madd_m128_m128m128fl(__m128 *r, const __m128 a, const __m128 b, float f);

MINLINE float VECTORCALL dot_m128m128(const __m128 a, const __m128 b);

MINLINE void VECTORCALL cross_m128_m128m128(__m128 *r, const __m128 a, const __m128 b);

MINLINE float VECTORCALL normalize_m128_m128(__m128 *r, const __m128 a);
MINLINE float VECTORCALL normalize_m128(__m128 *n);

MINLINE void VECTORCALL add_newell_cross_m128_m128m128(__m128 *n,
                                                       const __m128 v_prev,
                                                       const __m128 v_curr);

#  if BLI_MATH_DO_INLINE
#    include "math_vector_inline.c"
#  endif

#  ifdef __cplusplus
}
#  endif