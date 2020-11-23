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
 * The Original Code is Copyright (C) Blender Foundation
 * All rights reserved.
 */

#include "BLI_math_vector.h"

MINLINE void muladd_fmatrix_fvector(float to[3], const float matrix[3][3], const float from[3])
{
  to[0] += dot_v3v3(matrix[0], from);
  to[1] += dot_v3v3(matrix[1], from);
  to[2] += dot_v3v3(matrix[2], from);
}

MINLINE void muladd_fmatrix_fvector_sse(float to[3], const float matrix[3][3], const float from[3])
{
  __m128 F = load_m128_f3(from);
  to[0] += dot_m128m128(load_m128_f3(matrix[0]), F);
  to[1] += dot_m128m128(load_m128_f3(matrix[1]), F);
  to[2] += dot_m128m128(load_m128_f3(matrix[2]), F);
}

MINLINE void muladd_fmatrixT_fvector(float to[3], const float matrix[3][3], const float from[3])
{
  to[0] += matrix[0][0] * from[0] + matrix[1][0] * from[1] + matrix[2][0] * from[2];
  to[1] += matrix[0][1] * from[0] + matrix[1][1] * from[1] + matrix[2][1] * from[2];
  to[2] += matrix[0][2] * from[0] + matrix[1][2] * from[1] + matrix[2][2] * from[2];
}

MINLINE void muladd_fmatrixT_fvector_sse(float to[3], const float matrix[3][3], const float from[3])
{
  __m128 F = load_m128_f3(from);
  __m128 C0 = _mm_mul_ps(load_m128_f3(matrix[0]), _mm_shuffle_ps(F, F, _MM_SHUFFLE(0, 0, 0, 0))); // load_m128_f1(from[0]));
  __m128 C1 = _mm_mul_ps(load_m128_f3(matrix[1]), _mm_shuffle_ps(F, F, _MM_SHUFFLE(1, 1, 1, 1))); // load_m128_f1(from[1]));
  __m128 C2 = _mm_mul_ps(load_m128_f3(matrix[2]), _mm_shuffle_ps(F, F, _MM_SHUFFLE(2, 2, 2, 2))); // load_m128_f1(from[2]));
  __m128 R = _mm_add_ps(C0, _mm_add_ps(C1, C2));
  __m128 T = _mm_add_ps(load_m128_f3(to), R);

  float *t = (float *)&T;
  to[0] = t[0];
  to[1] = t[1];
  to[2] = t[2];
}
