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
 */

/** \file
 * \ingroup bli
 */

#include "BLI_math_vector.h"
#include "BLI_math_matrix.h"

void add_m3_m3m3(float R[3][3], const float A[3][3], const float B[3][3])
{
  int i, j;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      R[i][j] = A[i][j] + B[i][j];
    }
  }
}

void add_m3_m3m3_sse(float R[3][3], const float A[3][3], const float B[3][3])
{
  float *a = (float *)&A[0];
  float *b = (float *)&B[0];
  __m128 t0 = _mm_add_ps(load_m128_f4(a), load_m128_f4(b));
  __m128 t1 = _mm_add_ps(load_m128_f4(a + 4), load_m128_f4(b + 4));

  float *r = (float *)&R[0];
  _mm_store_ps(r, t0);
  _mm_store_ps(r + 4, t1);
  R[2][2] = A[2][2] + B[2][2];
}

void sub_m3_m3m3(float R[3][3], const float A[3][3], const float B[3][3])
{
  int i, j;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      R[i][j] = A[i][j] - B[i][j];
    }
  }
}

void sub_m3_m3m3_sse(float R[3][3], const float A[3][3], const float B[3][3])
{
  float *a = (float *)&A[0];
  float *b = (float *)&B[0];
  __m128 t0 = _mm_sub_ps(load_m128_f4(a), load_m128_f4(b));
  __m128 t1 = _mm_sub_ps(load_m128_f4(a + 4), load_m128_f4(b + 4));

  float *r = (float *)&R[0];
  _mm_store_ps(r, t0);
  _mm_store_ps(r + 4, t1);
  R[2][2] = A[2][2] - B[2][2];
}

void mul_v3_m3v3(float r[3], const float M[3][3], const float a[3])
{
  float t[3];
  copy_v3_v3(t, a);

  r[0] = M[0][0] * t[0] + M[1][0] * t[1] + M[2][0] * t[2];
  r[1] = M[0][1] * t[0] + M[1][1] * t[1] + M[2][1] * t[2];
  r[2] = M[0][2] * t[0] + M[1][2] * t[1] + M[2][2] * t[2];
}

void mul_v3_m3v3_sse(float r[3], const float M[3][3], const float a[3])
{
  __m128 C0 = _mm_mul_ps(load_m128_f3(M[0]), load_m128_f1(a[0]));
  __m128 C1 = _mm_mul_ps(load_m128_f3(M[1]), load_m128_f1(a[1]));
  __m128 C2 = _mm_mul_ps(load_m128_f3(M[2]), load_m128_f1(a[2]));
  __m128 R = _mm_add_ps(C0, _mm_add_ps(C1, C2));
  store_f3_m128(r, R);
}
