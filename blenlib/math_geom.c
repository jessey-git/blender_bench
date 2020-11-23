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

#include "BLI_math_vector.h"
#include "BLI_math_geom.h"

void cross_tri_v3(float n[3], const float v1[3], const float v2[3], const float v3[3])
{
  float n1[3], n2[3];

  n1[0] = v1[0] - v2[0];
  n2[0] = v2[0] - v3[0];
  n1[1] = v1[1] - v2[1];
  n2[1] = v2[1] - v3[1];
  n1[2] = v1[2] - v2[2];
  n2[2] = v2[2] - v3[2];
  n[0] = n1[1] * n2[2] - n1[2] * n2[1];
  n[1] = n1[2] * n2[0] - n1[0] * n2[2];
  n[2] = n1[0] * n2[1] - n1[1] * n2[0];
}

float normal_tri_v3(float n[3], const float v1[3], const float v2[3], const float v3[3])
{
  float n1[3], n2[3];

  n1[0] = v1[0] - v2[0];
  n2[0] = v2[0] - v3[0];
  n1[1] = v1[1] - v2[1];
  n2[1] = v2[1] - v3[1];
  n1[2] = v1[2] - v2[2];
  n2[2] = v2[2] - v3[2];
  n[0] = n1[1] * n2[2] - n1[2] * n2[1];
  n[1] = n1[2] * n2[0] - n1[0] * n2[2];
  n[2] = n1[0] * n2[1] - n1[1] * n2[0];

  return normalize_v3(n);
}

int is_quad_flip_v3_first_third_fast(const float v1[3],
                                     const float v2[3],
                                     const float v3[3],
                                     const float v4[3])
{
  float d_12[3], d_13[3], d_14[3];
  float cross_a[3], cross_b[3];
  sub_v3_v3v3(d_12, v2, v1);
  sub_v3_v3v3(d_13, v3, v1);
  sub_v3_v3v3(d_14, v4, v1);
  cross_v3_v3v3(cross_a, d_12, d_13);
  cross_v3_v3v3(cross_b, d_14, d_13);
  return dot_v3v3(cross_a, cross_b) > 0.0f;
}

void closest_on_tri_to_point_v3(
    float r[3], const float p[3], const float v1[3], const float v2[3], const float v3[3])
{
  float ab[3], ac[3], ap[3], d1, d2;
  float bp[3], d3, d4, vc, cp[3], d5, d6, vb, va;
  float denom, v, w;

  /* Check if P in vertex region outside A */
  sub_v3_v3v3(ab, v2, v1);
  sub_v3_v3v3(ac, v3, v1);
  sub_v3_v3v3(ap, p, v1);
  d1 = dot_v3v3(ab, ap);
  d2 = dot_v3v3(ac, ap);
  if (d1 <= 0.0f && d2 <= 0.0f) {
    /* barycentric coordinates (1,0,0) */
    copy_v3_v3(r, v1);
    return;
  }

  /* Check if P in vertex region outside B */
  sub_v3_v3v3(bp, p, v2);
  d3 = dot_v3v3(ab, bp);
  d4 = dot_v3v3(ac, bp);
  if (d3 >= 0.0f && d4 <= d3) {
    /* barycentric coordinates (0,1,0) */
    copy_v3_v3(r, v2);
    return;
  }
  /* Check if P in edge region of AB, if so return projection of P onto AB */
  vc = d1 * d4 - d3 * d2;
  if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
    v = d1 / (d1 - d3);
    /* barycentric coordinates (1-v,v,0) */
    madd_v3_v3v3fl(r, v1, ab, v);
    return;
  }
  /* Check if P in vertex region outside C */
  sub_v3_v3v3(cp, p, v3);
  d5 = dot_v3v3(ab, cp);
  d6 = dot_v3v3(ac, cp);
  if (d6 >= 0.0f && d5 <= d6) {
    /* barycentric coordinates (0,0,1) */
    copy_v3_v3(r, v3);
    return;
  }
  /* Check if P in edge region of AC, if so return projection of P onto AC */
  vb = d5 * d2 - d1 * d6;
  if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
    w = d2 / (d2 - d6);
    /* barycentric coordinates (1-w,0,w) */
    madd_v3_v3v3fl(r, v1, ac, w);
    return;
  }
  /* Check if P in edge region of BC, if so return projection of P onto BC */
  va = d3 * d6 - d5 * d4;
  if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
    w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
    /* barycentric coordinates (0,1-w,w) */
    sub_v3_v3v3(r, v3, v2);
    mul_v3_fl(r, w);
    add_v3_v3(r, v2);
    return;
  }

  /* P inside face region. Compute Q through its barycentric coordinates (u,v,w) */
  denom = 1.0f / (va + vb + vc);
  v = vb * denom;
  w = vc * denom;

  /* = u*a + v*b + w*c, u = va * denom = 1.0f - v - w */
  /* ac * w */
  mul_v3_fl(ac, w);
  /* a + ab * v */
  madd_v3_v3v3fl(r, v1, ab, v);
  /* a + ab * v + ac * w */
  add_v3_v3(r, ac);
}

/*
    HYBRID PASS-THROUGH
*/

void cross_tri_v3_pass(float n[3], const float v1[3], const float v2[3], const float v3[3])
{
  __m128 no;
  cross_tri_m128(&no, load_m128_f3(v1), load_m128_f3(v2), load_m128_f3(v3));
  store_f3_m128(n, no);
}

float normal_tri_v3_pass(float n[3], const float v1[3], const float v2[3], const float v3[3])
{
  __m128 no;
  float l = normal_tri_m128(&no, load_m128_f3(v1), load_m128_f3(v2), load_m128_f3(v3));

  store_f3_m128(n, no);
  return l;
}

int is_quad_flip_v3_first_third_fast_pass(const float v1[3],
                                            const float v2[3],
                                            const float v3[3],
                                            const float v4[3])
{
  return is_quad_flip_v3_first_third_fast_m128(
      load_m128_f3(v1), load_m128_f3(v2), load_m128_f3(v3), load_m128_f3(v4));
}

void closest_on_tri_to_point_v3_pass(
    float r[3], const float p[3], const float v1[3], const float v2[3], const float v3[3])
{
  __m128 R;
  closest_on_tri_to_point_m128(
      &R, load_m128_f3(p), load_m128_f3(v1), load_m128_f3(v2), load_m128_f3(v3));
  store_f3_m128(r, R);
}

/*
    HYBRID RE-IMPLEMENTATION
*/

void cross_tri_v3_sse(float n[3], const float v1[3], const float v2[3], const float v3[3])
{
  __m128 a, b, V2 = load_m128_f3(v2);
  sub_m128_m128m128(&a, load_m128_f3(v1), V2);
  sub_m128_m128m128(&b, V2, load_m128_f3(v3));

  __m128 N;
  cross_m128_m128m128(&N, a, b);
  store_f3_m128(n, N);
}

float normal_tri_v3_sse(float n[3], const float v1[3], const float v2[3], const float v3[3])
{
  __m128 a, b, V2 = load_m128_f3(v2);
  sub_m128_m128m128(&a, load_m128_f3(v1), V2);
  sub_m128_m128m128(&b, V2, load_m128_f3(v3));

  __m128 N;
  cross_m128_m128m128(&N, a, b);
  float l = normalize_m128(&N);

  store_f3_m128(n, N);
  return l;
}

/*
    SSE SIMD
*/

void VECTORCALL cross_tri_m128(__m128 *n, const __m128 v1, const __m128 v2, const __m128 v3)
{
  __m128 a, b;
  sub_m128_m128m128(&a, v1, v2);
  sub_m128_m128m128(&b, v2, v3);

  cross_m128_m128m128(n, a, b);
}

float VECTORCALL normal_tri_m128(__m128 *n, const __m128 v1, const __m128 v2, const __m128 v3)
{
  __m128 a, b;
  sub_m128_m128m128(&a, v1, v2);
  sub_m128_m128m128(&b, v2, v3);

  cross_m128_m128m128(n, a, b);
  return normalize_m128(n);
}

int VECTORCALL is_quad_flip_v3_first_third_fast_m128(const __m128 v1,
                                                     const __m128 v2,
                                                     const __m128 v3,
                                                     const __m128 v4)
{
  __m128 d_12, d_13, d_14;
  __m128 cross_a, cross_b;
  sub_m128_m128m128(&d_12, v2, v1);
  sub_m128_m128m128(&d_13, v3, v1);
  sub_m128_m128m128(&d_14, v4, v1);
  cross_m128_m128m128(&cross_a, d_12, d_13);
  cross_m128_m128m128(&cross_b, d_14, d_13);
  return dot_m128m128(cross_a, cross_b) > 0.0f;
}

void VECTORCALL closest_on_tri_to_point_m128(
    __m128 *r, const __m128 p, const __m128 v1, const __m128 v2, const __m128 v3)
{
  __m128 ab, ac, ap;
  __m128 bp, cp;

  float d1, d2;
  float d3, d4, vc, d5, d6, vb, va;
  float denom, v, w;

  /* Check if P in vertex region outside A */
  sub_m128_m128m128(&ab, v2, v1);
  sub_m128_m128m128(&ac, v3, v1);
  sub_m128_m128m128(&ap, p, v1);
  d1 = dot_m128m128(ab, ap);
  d2 = dot_m128m128(ac, ap);
  if (d1 <= 0.0f && d2 <= 0.0f) {
    /* barycentric coordinates (1,0,0) */
    *r = v1;
    return;
  }

  /* Check if P in vertex region outside B */
  sub_m128_m128m128(&bp, p, v2);
  d3 = dot_m128m128(ab, bp);
  d4 = dot_m128m128(ac, bp);
  if (d3 >= 0.0f && d4 <= d3) {
    /* barycentric coordinates (0,1,0) */
    *r = v2;
    return;
  }
  /* Check if P in edge region of AB, if so return projection of P onto AB */
  vc = d1 * d4 - d3 * d2;
  if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
    v = d1 / (d1 - d3);
    /* barycentric coordinates (1-v,v,0) */
    madd_m128_m128m128fl(r, v1, ab, v);
    return;
  }
  /* Check if P in vertex region outside C */
  sub_m128_m128m128(&cp, p, v3);
  d5 = dot_m128m128(ab, cp);
  d6 = dot_m128m128(ac, cp);
  if (d6 >= 0.0f && d5 <= d6) {
    /* barycentric coordinates (0,0,1) */
    *r = v3;
    return;
  }
  /* Check if P in edge region of AC, if so return projection of P onto AC */
  vb = d5 * d2 - d1 * d6;
  if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
    w = d2 / (d2 - d6);
    /* barycentric coordinates (1-w,0,w) */
    madd_m128_m128m128fl(r, v1, ac, w);
    return;
  }
  /* Check if P in edge region of BC, if so return projection of P onto BC */
  va = d3 * d6 - d5 * d4;
  if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
    w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
    /* barycentric coordinates (0,1-w,w) */
    sub_m128_m128m128(r, v3, v2);
    mul_m128_fl(r, w);
    add_m128_m128(r, v2);
    return;
  }

  /* P inside face region. Compute Q through its barycentric coordinates (u,v,w) */
  denom = 1.0f / (va + vb + vc);
  v = vb * denom;
  w = vc * denom;

  /* = u*a + v*b + w*c, u = va * denom = 1.0f - v - w */
  /* ac * w */
  mul_m128_fl(&ac, w);
  /* a + ab * v */
  madd_m128_m128m128fl(r, v1, ab, v);
  /* a + ab * v + ac * w */
  add_m128_m128(r, ac);
}
