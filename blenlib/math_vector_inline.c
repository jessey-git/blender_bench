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

MINLINE void zero_v3_sse(xmmvecf *r)
{
  r->m128 = _mm_setzero_ps();
}

MINLINE void copy_v3_v3_sse(xmmvecf *r, const xmmvecf a)
{
  r->m128 = a.m128;
}

MINLINE void sub_v3_v3v3_sse(xmmvecf *r, const xmmvecf a, const xmmvecf b)
{
  r->m128 = _mm_sub_ps(a.m128, b.m128);
}

MINLINE void sub_v3_v3_sse(xmmvecf *r, const xmmvecf a)
{
  r->m128 = _mm_sub_ps(r->m128, a.m128);
}

MINLINE void mul_v3_v3fl_sse(xmmvecf *r, const xmmvecf a, const float f)
{
  r->m128 = _mm_mul_ps(a.m128, _mm_set1_ps(f));
}

MINLINE void madd_v3_v3fl_sse(xmmvecf *r, const xmmvecf a, const float f)
{
#if 0 /* requires FMA instruction set */
    r->m128 = _mm_fmadd_ps(a.m128, _mm_set1_ps(f), r->m128);
#else
  r->m128 = _mm_add_ps(r->m128, _mm_mul_ps(a.m128, _mm_set1_ps(f)));
#endif
}

MINLINE float dot_v3v3_sse(const xmmvecf a, const xmmvecf b)
{
#if 0 /* requires sse4 */
    return _mm_cvtss_f32(_mm_dp_ps(a.m128, b.m128, 0x7F));
#else
  __m128 dot = _mm_mul_ps(a.m128, b.m128);
  __m128 temp = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 1, 2, 1));
  dot = _mm_add_ss(dot, temp);
  temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
  dot = _mm_add_ss(dot, temp);
  dot = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 0, 0, 0));
  return _mm_cvtss_f32(dot);
#endif
}

MINLINE void cross_v3_v3v3_sse(xmmvecf *r, const xmmvecf a, const xmmvecf b)
{
  __m128 a_yzx = _mm_shuffle_ps(a.m128, a.m128, _MM_SHUFFLE(3, 0, 2, 1));
  __m128 b_yzx = _mm_shuffle_ps(b.m128, b.m128, _MM_SHUFFLE(3, 0, 2, 1));
  __m128 normal = _mm_sub_ps(_mm_mul_ps(a.m128, b_yzx), _mm_mul_ps(a_yzx, b.m128));
  r->m128 = _mm_shuffle_ps(normal, normal, _MM_SHUFFLE(3, 0, 2, 1));
}

MINLINE void add_newell_cross_v3_v3v3_sse(xmmvecf *n, const xmmvecf v_prev, const xmmvecf v_curr)
{
  __m128 sub = _mm_sub_ps(v_prev.m128, v_curr.m128);
  __m128 add = _mm_add_ps(v_prev.m128, v_curr.m128);
  __m128 temp = _mm_mul_ps(_mm_shuffle_ps(sub, sub, _MM_SHUFFLE(3, 0, 2, 1)),
                           _mm_shuffle_ps(add, add, _MM_SHUFFLE(3, 1, 0, 2)));
  n->m128 = _mm_add_ps(n->m128, temp);
}

MINLINE float normalize_v3_v3_sse(xmmvecf *r, const xmmvecf a)
{
#if 0 /* requires sse4 */
    __m128 dot = _mm_dp_ps(r->m128, a.m128, 0x7f);
#else
  __m128 dot = _mm_mul_ps(r->m128, a.m128);
  __m128 temp = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(2, 1, 2, 1));
  dot = _mm_add_ss(dot, temp);
  temp = _mm_shuffle_ps(temp, temp, _MM_SHUFFLE(1, 1, 1, 1));
  dot = _mm_add_ss(dot, temp);
  dot = _mm_shuffle_ps(dot, dot, _MM_SHUFFLE(0, 0, 0, 0));
#endif

  const __m128 eps = _mm_set1_ps(1e-35f);
  const __m128 mask = _mm_cmpge_ps(dot, eps);
  const __m128 length = _mm_sqrt_ps(dot);
  __m128 normal = _mm_div_ps(r->m128, length);
  r->m128 = _mm_xor_ps(_mm_setzero_ps(), _mm_and_ps(mask, _mm_xor_ps(normal, _mm_setzero_ps())));

  return _mm_cvtss_f32(length);
}

MINLINE float normalize_v3_sse(xmmvecf *n)
{
  return normalize_v3_v3_sse(n, *n);
}

MINLINE void normal_short_to_float_v3_sse(xmmvecf *out, const short in[3])
{
  __m128i t = _mm_setr_epi32(in[0], in[1], in[2], 0);
  out->m128 = _mm_mul_ps(_mm_cvtepi32_ps(t), _mm_set1_ps(1.0f / 32767.0f));
}

/*
    ORIGINAL BELOW
*/

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

MINLINE void zero_v3(float r[3])
{
  r[0] = 0.0f;
  r[1] = 0.0f;
  r[2] = 0.0f;
}

MINLINE void mul_v3_v3fl(float r[3], const float a[3], float f)
{
  r[0] = a[0] * f;
  r[1] = a[1] * f;
  r[2] = a[2] * f;
}

MINLINE float dot_v3v3(const float a[3], const float b[3])
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

MINLINE float dot_v3v3_internalsse(const float a[3], const float b[3])
{
  return dot_v3v3_sse(load_xmmvecf_f3(a), load_xmmvecf_f3(b));
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
