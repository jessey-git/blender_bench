// Copyright(c) 2019-2020 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only

#include <benchmark/benchmark.h>

#include "simulation/implicit_blender.h"

#include "geo_quadsphere1.h"

//
// Baseline: muladd_fmatrix_fvector
//
static void BB_muladd_fmatrix_fvector(benchmark::State &state)
{
  float r[3];
  float m1[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  float v1[3] = {1, 0, 1};

  for (auto _ : state) {
    for (int i = 0; i < 10; i++) {
      muladd_fmatrix_fvector(r, m1, v1);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// SSE: muladd_fmatrix_fvector: implemented in terms of sse
//
static void BB_muladd_fmatrix_fvector_sse(benchmark::State &state)
{
  float r[3];
  float m1[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  float v1[3] = {1, 0, 1};

  for (auto _ : state) {
    for (int i = 0; i < 10; i++) {
      muladd_fmatrix_fvector_sse(r, m1, v1);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// Baseline: muladd_fmatrixT_fvector
//
static void BB_muladd_fmatrixT_fvector(benchmark::State &state)
{
  float r[3];
  float m1[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  float v1[3] = {1, 0, 1};

  for (auto _ : state) {
    for (int i = 0; i < 10; i++) {
      muladd_fmatrixT_fvector(r, m1, v1);

      benchmark::DoNotOptimize(r);
    }
  }
}

//
// SSE: muladd_fmatrixT_fvector: implemented in terms of sse
//
static void BB_muladd_fmatrixT_fvector_sse(benchmark::State &state)
{
  float r[3];
  float m1[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  float v1[3] = {1, 0, 1};

  for (auto _ : state) {
    for (int i = 0; i < 10; i++) {
      muladd_fmatrixT_fvector_sse(r, m1, v1);

      benchmark::DoNotOptimize(r);
    }
  }
}

BENCHMARK(BB_muladd_fmatrix_fvector);
BENCHMARK(BB_muladd_fmatrix_fvector_sse);
BENCHMARK(BB_muladd_fmatrixT_fvector);
BENCHMARK(BB_muladd_fmatrixT_fvector_sse);
