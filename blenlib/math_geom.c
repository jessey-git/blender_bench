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

void cross_tri_v3_internalsse(float n[3], const float v1[3], const float v2[3], const float v3[3])
{
  xmmvecf no;
  cross_tri_v3_sse(&no, load_xmmvecf_f3(v1), load_xmmvecf_f3(v2), load_xmmvecf_f3(v3));
  store_f3_xmmvecf(n, no);
}

void cross_tri_v3_sse(xmmvecf *n, const xmmvecf v1, const xmmvecf v2, const xmmvecf v3)
{
  xmmvecf a, b;
  sub_v3_v3v3_sse(&a, v1, v2);
  sub_v3_v3v3_sse(&b, v2, v3);

  cross_v3_v3v3_sse(n, a, b);
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

float normal_tri_v3_internalsse(float n[3],
                                const float v1[3],
                                const float v2[3],
                                const float v3[3])
{
  xmmvecf no;
  float d = normal_tri_v3_sse(&no, load_xmmvecf_f3(v1), load_xmmvecf_f3(v2), load_xmmvecf_f3(v3));

  store_f3_xmmvecf(n, no);
  return d;
}

float normal_tri_v3_sse(xmmvecf *n, const xmmvecf v1, const xmmvecf v2, const xmmvecf v3)
{
  xmmvecf a, b;
  sub_v3_v3v3_sse(&a, v1, v2);
  sub_v3_v3v3_sse(&b, v2, v3);

  cross_v3_v3v3_sse(n, a, b);

  return normalize_v3_sse(n);
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

int is_quad_flip_v3_first_third_fast_internalsse(const float v1[3],
                                                 const float v2[3],
                                                 const float v3[3],
                                                 const float v4[3])
{
  return is_quad_flip_v3_first_third_fast_sse(
      load_xmmvecf_f3(v1), load_xmmvecf_f3(v2), load_xmmvecf_f3(v3), load_xmmvecf_f3(v4));
}

int is_quad_flip_v3_first_third_fast_sse(const xmmvecf v1,
                                         const xmmvecf v2,
                                         const xmmvecf v3,
                                         const xmmvecf v4)
{
  xmmvecf d_12, d_13, d_14;
  xmmvecf cross_a, cross_b;
  sub_v3_v3v3_sse(&d_12, v2, v1);
  sub_v3_v3v3_sse(&d_13, v3, v1);
  sub_v3_v3v3_sse(&d_14, v4, v1);
  cross_v3_v3v3_sse(&cross_a, d_12, d_13);
  cross_v3_v3v3_sse(&cross_b, d_14, d_13);
  return dot_v3v3_sse(cross_a, cross_b) > 0.0f;
}