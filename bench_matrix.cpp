// Copyright(c) 2019-2020 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <benchmark/benchmark.h>

#include "blenlib/BLI_math_matrix.h"

#include "geo_quadsphere1.h"

//
// Baseline: add_m3_m3m3
//
static void BB_add_m3_m3m3(benchmark::State &state)
{
  float r[3][3];
  float m1[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  float m2[3][3] = {{1, 1, 0}, {0, 1, 1}, {1, 0, 1}};

  for (auto _ : state) {
    for (int i = 0; i < 10; i++) {
      add_m3_m3m3(r, m1, m2);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// SSE: add_m3_m3m3: implemented in terms of sse
//
static void BB_add_m3_m3m3_sse(benchmark::State &state)
{
  float r[3][3];
  float m1[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  float m2[3][3] = {{1, 1, 0}, {0, 1, 1}, {1, 0, 1}};

  for (auto _ : state) {
    for (int i = 0; i < 10; i++) {
      add_m3_m3m3_sse(r, m1, m2);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// Baseline: sub_m3_m3m3
//
static void BB_sub_m3_m3m3(benchmark::State &state)
{
  float r[3][3];
  float m1[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  float m2[3][3] = {{1, 1, 0}, {0, 1, 1}, {1, 0, 1}};

  for (auto _ : state) {
    for (int i = 0; i < 10; i++) {
      sub_m3_m3m3(r, m1, m2);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// SSE: sub_m3_m3m3: implemented in terms of sse
//
static void BB_sub_m3_m3m3_sse(benchmark::State &state)
{
  float r[3][3];
  float m1[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  float m2[3][3] = {{1, 1, 0}, {0, 1, 1}, {1, 0, 1}};

  for (auto _ : state) {
    for (int i = 0; i < 10; i++) {
      sub_m3_m3m3_sse(r, m1, m2);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// Baseline: mul_v3_m3v3
//
static void BB_mul_v3_m3v3(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;

  float r[3];
  float m[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numverts; i++) {
      mul_v3_m3v3(r, m, geo_quadsphere1_f3_verts[i]);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// SSE: mul_v3_m3v3: implemented in terms of sse
//
static void BB_mul_v3_m3v3_sse(benchmark::State &state)
{
  const float(*verts)[3] = geo_quadsphere1_f3_verts;

  float r[3];
  float m[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};

  for (auto _ : state) {
    for (int i = 0; i < geo_quadsphehere1_numverts; i++) {
      mul_v3_m3v3_sse(r, m, geo_quadsphere1_f3_verts[i]);

      benchmark::DoNotOptimize(r);
    }
  }
}

BENCHMARK(BB_add_m3_m3m3);
BENCHMARK(BB_add_m3_m3m3_sse);
BENCHMARK(BB_sub_m3_m3m3);
BENCHMARK(BB_sub_m3_m3m3_sse);
BENCHMARK(BB_mul_v3_m3v3);
BENCHMARK(BB_mul_v3_m3v3_sse);
