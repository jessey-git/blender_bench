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

#include <math.h>

#include "BLI_math_inline.h"
#include "BLI_math_vector.h"

/*
    ORIGINAL BELOW
*/

MINLINE void zero_v3(float r[3])
{
  r[0] = 0.0f;
  r[1] = 0.0f;
  r[2] = 0.0f;
}

MINLINE void copy_v2_v2(float r[2], const float a[2])
{
  r[0] = a[0];
  r[1] = a[1];
}

MINLINE void copy_v3_v3(float r[3], const float a[3])
{
  r[0] = a[0];
  r[1] = a[1];
  r[2] = a[2];
}

MINLINE void add_v3_v3(float r[3], const float a[3])
{
  r[0] += a[0];
  r[1] += a[1];
  r[2] += a[2];
}

MINLINE void sub_v2_v2(float r[2], const float a[2])
{
  r[0] -= a[0];
  r[1] -= a[1];
}

MINLINE void sub_v2_v2v2(float r[2], const float a[2], const float b[2])
{
  r[0] = a[0] - b[0];
  r[1] = a[1] - b[1];
}

MINLINE void sub_v3_v3(float r[3], const float a[3])
{
  r[0] -= a[0];
  r[1] -= a[1];
  r[2] -= a[2];
}

MINLINE void sub_v3_v3v3(float r[3], const float a[3], const float b[3])
{
  r[0] = a[0] - b[0];
  r[1] = a[1] - b[1];
  r[2] = a[2] - b[2];
}

MINLINE void mul_v3_fl(float r[3], float f)
{
  r[0] *= f;
  r[1] *= f;
  r[2] *= f;
}

MINLINE void mul_v3_v3fl(float r[3], const float a[3], float f)
{
  r[0] = a[0] * f;
  r[1] = a[1] * f;
  r[2] = a[2] * f;
}

MINLINE void madd_v3_v3fl(float r[3], const float a[3], float f)
{
  r[0] += a[0] * f;
  r[1] += a[1] * f;
  r[2] += a[2] * f;
}

MINLINE void madd_v3_v3v3fl(float r[3], const float a[3], const float b[3], float f)
{
  r[0] = a[0] + b[0] * f;
  r[1] = a[1] + b[1] * f;
  r[2] = a[2] + b[2] * f;
}

MINLINE float dot_v3v3(const float a[3], const float b[3])
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

MINLINE void cross_v3_v3v3(float r[3], const float a[3], const float b[3])
{
  r[0] = a[1] * b[2] - a[2] * b[1];
  r[1] = a[2] * b[0] - a[0] * b[2];
  r[2] = a[0] * b[1] - a[1] * b[0];
}

MINLINE float normalize_v3_v3_length(float r[3], const float a[3], const float unit_length)
{
  float d = dot_v3v3(a, a);

  /* a larger value causes normalize errors in a
   * scaled down models with camera extreme close */
  if (d > 1.0e-35f) {
    d = sqrtf(d);
    mul_v3_v3fl(r, a, unit_length / d);
  }
  else {
    zero_v3(r);
    d = 0.0f;
  }

  return d;
}

MINLINE float normalize_v3_v3(float r[3], const float a[3])
{
  return normalize_v3_v3_length(r, a, 1.0f);
}

MINLINE float normalize_v3(float n[3])
{
  return normalize_v3_v3(n, n);
}

/*
    HYBRID
*/

MINLINE float dot_v3v3_pass(const float a[3], const float b[3])
{
  return dot_m128m128(load_m128_f3(a), load_m128_f3(b));
}

/*
    SSE SIMD
*/

MINLINE __m128 VECTORCALL load_m128_f1(const float f)
{
  return _mm_set1_ps(f);
}

MINLINE __m128 VECTORCALL load_m128_f3(const float v[3])
{
  return _mm_setr_ps(v[0], v[1], v[2], 0.0f);
}

MINLINE __m128 VECTORCALL load_m128_f4(const float v[4])
{
  return _mm_loadu_ps(v);
}

MINLINE void VECTORCALL store_f3_m128(float r[3], const __m128 a)
{
  float *f = (float *)&a;
  r[0] = f[0];
  r[1] = f[1];
  r[2] = f[2];
}

MINLINE void VECTORCALL store_f4_m128(float r[4], const __m128 a)
{
  _mm_store_ps(r, a);
}

MINLINE void VECTORCALL zero_m128(__m128 *r)
{
  *r = _mm_setzero_ps();
}

MINLINE void VECTORCALL add_m128_m128(__m128 *r, const __m128 a)
{
  *r = _mm_add_ps(*r, a);
}

MINLINE void VECTORCALL sub_m128_m128m128(__m128 *r, const __m128 a, const __m128 b)
{
  *r = _mm_sub_ps(a, b);
}

MINLINE void VECTORCALL sub_m128_m128(__m128 *r, const __m128 a)
{
  *r = _mm_sub_ps(*r, a);
}

MINLINE void VECTORCALL mul_m128_fl(__m128 *r, const float f)
{
  *r = _mm_mul_ps(*r, _mm_set1_ps(f));
}

MINLINE void VECTORCALL mul_m128_m128fl(__m128 *r, const __m128 a, const float f)
{
  *r = _mm_mul_ps(a, _mm_set1_ps(f));
}

MINLINE void VECTORCALL madd_m128_m128fl(__m128 *r, const __m128 a, const float f)
{
  *r = _mm_add_ps(*r, _mm_mul_ps(a, _mm_set1_ps(f)));
}

MINLINE void VECTORCALL madd_m128_m128m128fl(__m128 *r, const __m128 a, const __m128 b, float f)
{
  *r = _mm_add_ps(a, _mm_mul_ps(b, _mm_set1_ps(f)));
}

MINLINE float VECTORCALL dot_m128m128(const __m128 a, const __m128 b)
{
  // return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x7F));
  __m128 dot = _mm_mul_ps(a, b);
  __m128 temp = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 1, 2, 1));
  dot = _mm_add_ss(dot, temp);
  temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
  dot = _mm_add_ss(dot, temp);
  dot = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 0, 0, 0));
  return _mm_cvtss_f32(dot);
}

MINLINE void VECTORCALL cross_m128_m128m128(__m128 *r, const __m128 a, const __m128 b)
{
  __m128 a_yzx = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1));
  __m128 b_yzx = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1));
  __m128 normal = _mm_sub_ps(_mm_mul_ps(a, b_yzx), _mm_mul_ps(a_yzx, b));
  *r = _mm_shuffle_ps(normal, normal, _MM_SHUFFLE(3, 0, 2, 1));
}

MINLINE float VECTORCALL normalize_m128_m128(__m128 *r, const __m128 a)
{
  __m128 dot = _mm_mul_ps(*r, a);
  __m128 temp = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 1, 2, 1));
  dot = _mm_add_ss(dot, temp);
  temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
  dot = _mm_add_ss(dot, temp);
  dot = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 0, 0, 0));

  const __m128 eps = _mm_set1_ps(1e-35f);
  const __m128 mask = _mm_cmpge_ps(dot, eps);
  const __m128 length = _mm_sqrt_ps(dot);
  __m128 normal = _mm_div_ps(*r, length);
  *r = _mm_xor_ps(_mm_setzero_ps(), _mm_and_ps(mask, _mm_xor_ps(normal, _mm_setzero_ps())));

  return _mm_cvtss_f32(length);
}

MINLINE float VECTORCALL normalize_m128(__m128 *n)
{
  return normalize_m128_m128(n, *n);
}

MINLINE void VECTORCALL add_newell_cross_m128_m128m128(__m128 *n,
                                                       const __m128 v_prev,
                                                       const __m128 v_curr)
{
  __m128 sub = _mm_sub_ps(v_prev, v_curr);
  __m128 add = _mm_add_ps(v_prev, v_curr);
  __m128 temp = _mm_mul_ps(_mm_shuffle_ps(sub, sub, _MM_SHUFFLE(3, 0, 2, 1)),
                           _mm_shuffle_ps(add, add, _MM_SHUFFLE(3, 1, 0, 2)));
  *n = _mm_add_ps(*n, temp);
}

MINLINE void VECTORCALL normal_short_to_float_m128(__m128 *out, const short in[3])
{
  __m128i t = _mm_setr_epi32(in[0], in[1], in[2], 0);
  *out = _mm_mul_ps(_mm_cvtepi32_ps(t), _mm_set1_ps(1.0f / 32767.0f));
}
