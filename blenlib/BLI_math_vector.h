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

typedef struct ATTR_ALIGN(16) xmmvecf {
  union {
    __m128 m128;
    struct {
      float x, y, z, w;
    };
  };
} xmmvecf;

typedef struct ATTR_ALIGN(16) xmmveci {
  union {
    __m128i m128;
    struct {
      int x, y, z, w;
    };
  };
} xmmveci;

MINLINE xmmvecf load_xmmvecf_f1(float f)
{
  xmmvecf r;
  r.m128 = _mm_set1_ps(f);
  return r;
}

MINLINE xmmvecf load_xmmvecf_f3(const float v[3])
{
  xmmvecf r;
  r.m128 = _mm_setr_ps(v[0], v[1], v[2], 0);
  return r;
}

MINLINE xmmvecf load_xmmvecf_f4(const float v[4])
{
  xmmvecf r;
  r.m128 = _mm_loadu_ps(v);
  return r;
}

MINLINE void store_f3_xmmvecf(float r[3], const xmmvecf a)
{
  r[0] = a.x;
  r[1] = a.y;
  r[2] = a.z;
}

MINLINE void store_f4_xmmvecf(float r[4], const xmmvecf a)
{
  _mm_store_ps(r, a.m128);
}

MINLINE void zero_v3_sse(xmmvecf *r);
MINLINE void copy_v3_v3_sse(xmmvecf *r, const xmmvecf a);
MINLINE void sub_v3_v3v3_sse(xmmvecf *r, const xmmvecf a, const xmmvecf b);
MINLINE void sub_v3_v3_sse(xmmvecf *r, const xmmvecf a);
MINLINE void mul_v3_v3fl_sse(xmmvecf *r, const xmmvecf a, const float f);
MINLINE void madd_v3_v3fl_sse(xmmvecf *r, const xmmvecf a, const float f);
MINLINE float dot_v3v3_sse(const xmmvecf a, const xmmvecf b);
MINLINE void cross_v3_v3v3_sse(xmmvecf *r, const xmmvecf a, const xmmvecf b);
MINLINE float normalize_v3_v3_sse(xmmvecf *r, const xmmvecf a);
MINLINE float normalize_v3_sse(xmmvecf *n);
MINLINE void normal_short_to_float_v3_sse(xmmvecf *out, const short in[3]);

/* -------------------------------------- */

MINLINE void copy_v2_v2(float r[2], const float a[2]);
MINLINE void copy_v3_v3(float r[3], const float a[3]);

MINLINE void sub_v2_v2(float r[2], const float a[2]);
MINLINE void sub_v2_v2v2(float r[2], const float a[2], const float b[2]);
MINLINE void sub_v3_v3(float r[3], const float a[3]);
MINLINE void sub_v3_v3v3(float r[3], const float a[3], const float b[3]);

MINLINE void zero_v3(float r[3]);

MINLINE void mul_v3_v3fl(float r[3], const float a[3], float f);

MINLINE float dot_v3v3(const float a[3], const float b[3]);

MINLINE void cross_v3_v3v3(float r[3], const float a[3], const float b[3]);

MINLINE float normalize_v3_v3_length(float r[3], const float a[3], const float unit_length);
MINLINE float normalize_v3_v3(float r[3], const float a[3]);
MINLINE float normalize_v3(float n[3]);

#if BLI_MATH_DO_INLINE
#  include "math_vector_inline.c"
#endif

#ifdef __cplusplus
}
#endif