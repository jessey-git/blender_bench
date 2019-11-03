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
 * The Original Code is Copyright (C) 2016 by Mike Erwin.
 * All rights reserved.
 */

#pragma once

#include "blenlib/BLI_compiler_compat.h"
#include "blenlib/BLI_math_vector.h"

typedef struct GPUPackedNormal {
  int x : 10;
  int y : 10;
  int z : 10;
  int w : 2; /* 0 by default, can manually set to { -2, -1, 0, 1 } */
} GPUPackedNormal;

/* OpenGL ES packs in a different order as desktop GL but component conversion is the same.
 * Of the code here, only struct GPUPackedNormal needs to change. */

#define SIGNED_INT_10_MAX 511
#define SIGNED_INT_10_MIN -512

BLI_INLINE int clampi(int x, int min_allowed, int max_allowed)
{
#if TRUST_NO_ONE
  assert(min_allowed <= max_allowed);
#endif
  if (x < min_allowed) {
    return min_allowed;
  }
  else if (x > max_allowed) {
    return max_allowed;
  }
  else {
    return x;
  }
}

BLI_INLINE int gpu_convert_normalized_f32_to_i10(float x)
{
  int qx = x * 511.0f;
  return clampi(qx, SIGNED_INT_10_MIN, SIGNED_INT_10_MAX);
}

BLI_INLINE GPUPackedNormal GPU_normal_convert_i10_v3(const float data[3])
{
  GPUPackedNormal n = {
      gpu_convert_normalized_f32_to_i10(data[0]),
      gpu_convert_normalized_f32_to_i10(data[1]),
      gpu_convert_normalized_f32_to_i10(data[2]),
  };
  return n;
}

// The new SSE based quantize code is technically more accurate since rounding is done
// at the very end, instead of at the beginning...
BLI_INLINE GPUPackedNormal GPU_normal_convert_i10_v3_sse(const xmmvecf data)
{
  const xmmvecf min = load_xmmvecf_f1(SIGNED_INT_10_MIN);
  const xmmvecf max = load_xmmvecf_f1(SIGNED_INT_10_MAX);
  __m128 clamped = _mm_min_ps(_mm_max_ps(_mm_mul_ps(data.m128, max.m128), min.m128), max.m128);

  xmmveci result_i;
  result_i.m128 = _mm_cvtps_epi32(clamped);

  return {
      result_i.x,
      result_i.y,
      result_i.z,
  };
}

BLI_INLINE GPUPackedNormal GPU_normal_convert_i10_v3_internalsse(const float data[3])
{
  return GPU_normal_convert_i10_v3_sse(load_xmmvecf_f3(data));
}
