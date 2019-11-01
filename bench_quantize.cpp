// Copyright(c) 2019 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <benchmark/benchmark.h>

#include "blenlib/BLI_math_geom.h"
#include "draw/GPU_vertex_format.h"

#include "geo_quadsphere1.h"

//
// Baseline: GPU_normal_convert_i10_v3
//
static void BB_GPU_normal_convert_i10_v3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  // Setup
  float normals[48][3];
  for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
    xmmvecf n;
    normal_tri_v3_sse(&n,
                      load_xmmvecf_f3(verts[tris[i][0]]),
                      load_xmmvecf_f3(verts[tris[i][1]]),
                      load_xmmvecf_f3(verts[tris[i][2]]));

    store_f3_xmmvecf(normals[i], n);
  }

  // Benchmark
  for (auto _ : state) {
    for (int i = 0; i < 48; i++) {
      GPUPackedNormal pn = GPU_normal_convert_i10_v3(normals[i]);
      benchmark::DoNotOptimize(pn);
    }
  }
}

//
//  SSE Variant: GPU_normal_convert_i10_v3 see: load from f3
//
static void BB_GPU_normal_convert_i10_v3_sse_lf3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  // Setup
  float normals[48][3];
  for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
    xmmvecf n;
    normal_tri_v3_sse(&n,
                      load_xmmvecf_f3(verts[tris[i][0]]),
                      load_xmmvecf_f3(verts[tris[i][1]]),
                      load_xmmvecf_f3(verts[tris[i][2]]));

    store_f3_xmmvecf(normals[i], n);
  }

  // Benchmark
  for (auto _ : state) {
    for (int i = 0; i < 48; i++) {
      GPUPackedNormal pn = GPU_normal_convert_i10_v3_sse(load_xmmvecf_f3(normals[i]));
      benchmark::DoNotOptimize(pn);
    }
  }
}

//
//  SSE Variant: GPU_normal_convert_i10_v3 see: load from f4
//
static void BB_GPU_normal_convert_i10_v3_sse_lf4(benchmark::State &state)
{
  const float(*verts)[4] = geo_quadsphere1_f4_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  // Setup
  float normals[48][4];
  for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
    xmmvecf n;
    normal_tri_v3_sse(&n,
                      load_xmmvecf_f4(verts[tris[i][0]]),
                      load_xmmvecf_f4(verts[tris[i][1]]),
                      load_xmmvecf_f4(verts[tris[i][2]]));

    store_f4_xmmvecf(normals[i], n);
  }

  // Benchmark
  for (auto _ : state) {
    for (int i = 0; i < 48; i++) {
      GPUPackedNormal pn = GPU_normal_convert_i10_v3_sse(load_xmmvecf_f4(normals[i]));
      benchmark::DoNotOptimize(pn);
    }
  }
}

//
//  SSE Speed-of-light: GPU_normal_convert_i10_v3 see: load from xmm
//
static void BB_GPU_normal_convert_i10_v3_sse_lxmm(benchmark::State &state)
{
  const float(*verts)[4] = geo_quadsphere1_f4_verts;
  const int(*tris)[3] = geo_quadspehere1_indices;

  // Setup
  xmmvecf normals[48];
  for (int i = 0; i < geo_quadsphehere1_numtris; i++) {
    normal_tri_v3_sse(&normals[i],
                      load_xmmvecf_f4(verts[tris[i][0]]),
                      load_xmmvecf_f4(verts[tris[i][1]]),
                      load_xmmvecf_f4(verts[tris[i][2]]));
  }

  // Benchmark
  for (auto _ : state) {
    for (int i = 0; i < 48; i++) {
      GPUPackedNormal pn = GPU_normal_convert_i10_v3_sse(normals[i]);
      benchmark::DoNotOptimize(pn);
    }
  }
}


BENCHMARK(BB_GPU_normal_convert_i10_v3);
BENCHMARK(BB_GPU_normal_convert_i10_v3_sse_lf3);
BENCHMARK(BB_GPU_normal_convert_i10_v3_sse_lf4);
BENCHMARK(BB_GPU_normal_convert_i10_v3_sse_lxmm);
