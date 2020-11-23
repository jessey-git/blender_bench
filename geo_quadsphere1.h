// Copyright(c) 2019-2020 Jesse Yurkovich
// SPDX-License-Identifier: GPL-3.0-only
#pragma once

static const int geo_quadsphehere1_numverts = 26;
static const int geo_quadsphehere1_numtris = 48;

static const float geo_quadsphere1_f3_verts[][3] =
{
    { -0.289f,-0.289f,-0.289f },
    { -0.289f,-0.289f,0.289f },
    { -0.289f,0.289f,-0.289f },
    { -0.289f,0.289f,0.289f },
    { 0.289f,-0.289f,-0.289f },
    { 0.289f,-0.289f,0.289f },
    { 0.289f,0.289f,-0.289f },
    { 0.289f,0.289f,0.289f },
    { -0.354f,0.000f,-0.354f },
    { -0.354f,-0.354f,-0.000f },
    { -0.354f,-0.000f,0.354f },
    { -0.354f,0.354f,-0.000f },
    { 0.000f,0.354f,-0.354f },
    { -0.000f,0.354f,0.354f },
    { 0.354f,0.354f,-0.000f },
    { 0.354f,-0.000f,-0.354f },
    { 0.354f,-0.000f,0.354f },
    { 0.354f,-0.354f,-0.000f },
    { -0.000f,-0.354f,-0.354f },
    { -0.000f,-0.354f,0.354f },
    { -0.500f,-0.000f,0.000f },
    { 0.000f,0.500f,0.000f },
    { 0.500f,-0.000f,0.000f },
    { -0.000f,-0.500f,0.000f },
    { 0.000f,-0.000f,-0.500f },
    { -0.000f,-0.000f,0.500f },
};

static const float geo_quadsphere1_f4_verts[][4] =
{
    { -0.289f,-0.289f,-0.289f, 0.0f },
    { -0.289f,-0.289f,0.289f, 0.0f },
    { -0.289f,0.289f,-0.289f, 0.0f },
    { -0.289f,0.289f,0.289f, 0.0f },
    { 0.289f,-0.289f,-0.289f, 0.0f },
    { 0.289f,-0.289f,0.289f, 0.0f },
    { 0.289f,0.289f,-0.289f, 0.0f },
    { 0.289f,0.289f,0.289f, 0.0f },
    { -0.354f,0.000f,-0.354f, 0.0f },
    { -0.354f,-0.354f,-0.000f, 0.0f },
    { -0.354f,-0.000f,0.354f, 0.0f },
    { -0.354f,0.354f,-0.000f, 0.0f },
    { 0.000f,0.354f,-0.354f, 0.0f },
    { -0.000f,0.354f,0.354f, 0.0f },
    { 0.354f,0.354f,-0.000f, 0.0f },
    { 0.354f,-0.000f,-0.354f, 0.0f },
    { 0.354f,-0.000f,0.354f, 0.0f },
    { 0.354f,-0.354f,-0.000f, 0.0f },
    { -0.000f,-0.354f,-0.354f, 0.0f },
    { -0.000f,-0.354f,0.354f, 0.0f },
    { -0.500f,-0.000f,0.000f, 0.0f },
    { 0.000f,0.500f,0.000f, 0.0f },
    { 0.500f,-0.000f,0.000f, 0.0f },
    { -0.000f,-0.500f,0.000f, 0.0f },
    { 0.000f,-0.000f,-0.500f, 0.0f },
    { -0.000f,-0.000f,0.500f, 0.0f },
};

static const int geo_quadspehere1_indices[48][3] =
{
    { 0, 9, 20 },
    { 0, 20, 8 },
    { 9, 1, 10 },
    { 9, 10, 20 },
    { 20, 10, 3 },
    { 20, 3, 11 },
    { 8, 20, 11 },
    { 8, 11, 2 },
    { 2, 11, 21 },
    { 2, 21, 12 },
    { 11, 3, 13 },
    { 11, 13, 21 },
    { 21, 13, 7 },
    { 21, 7, 14 },
    { 12, 21, 14 },
    { 12, 14, 6 },
    { 6, 14, 22 },
    { 6, 22, 15 },
    { 14, 7, 16 },
    { 14, 16, 22 },
    { 22, 16, 5 },
    { 22, 5, 17 },
    { 15, 22, 17 },
    { 15, 17, 4 },
    { 4, 17, 23 },
    { 4, 23, 18 },
    { 17, 5, 19 },
    { 17, 19, 23 },
    { 23, 19, 1 },
    { 23, 1, 9 },
    { 18, 23, 9 },
    { 18, 9, 0 },
    { 2, 12, 24 },
    { 2, 24, 8 },
    { 12, 6, 15 },
    { 12, 15, 24 },
    { 24, 15, 4 },
    { 24, 4, 18 },
    { 8, 24, 18 },
    { 8, 18, 0 },
    { 7, 13, 25 },
    { 7, 25, 16 },
    { 13, 3, 10 },
    { 13, 10, 25 },
    { 25, 10, 1 },
    { 25, 1, 19 },
    { 16, 25, 19 },
    { 16, 19, 5 },
};