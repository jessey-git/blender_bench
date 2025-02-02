# blender_bench

A playground/benchmarking program for testing out alternative implementations of low level math routines within [Blender](https://www.blender.org/).

## Motivating Scenarios
The following scenarios were profiled to determine which math-heavy code paths within Blender could be improved. From those code paths, select mathematical functions were tested out.

### Cloth simulation
 * 2 different cloth simulations, yielding very different profiles, were created
 * Baking of the simulation was done on the commandline to reduce interaction and aid consistency

### Modeling with modifiers / procedural non-destructive workflows
* Open blender as follows (keeps drawing overhead low): `blender --window-geometry 0 0 960 540`
* Download the [blenderman](https://cloud.blender.org/p/gallery/57e5084f0fcf294119c5055c) demo file
* Open the downloaded `blenderman.blend` with `Load UI` _unchecked_
* Switch to camera view (NUMPAD-0)
* Profile: Hit play on the timeline

### Heavy array modifier usage
 * Open blender as follows (keeps drawing overhead low): `blender --window-geometry 0 0 960 540`
 * Create a Suzanne monkey
 * Array the monkey 40 times in X, 40 times in Y (1600 total)
 * Animate the monkey position
 * Profile: Hit play on the timeline

### Transformation of vertices while in edit mode
 * Open blender as follows (keeps drawing overhead low): `blender --window-geometry 0 0 960 540`
 * Create a UV Sphere with 12 segments, 6 rings
 * Apply 7 levels of subdivision
 * Go to edit mode, use vertex select, and select 8k faces worth of vertices
 * Profile: Hit G and attempt to move the selected vertices around

Interesting code paths found:
- cloth: `muladd_fmatrix_fvector`, `mul_bfmatrix_lfvector`, `closest_on_tri_to_point_v3`, `isect_seg_seg_v3`
- general: `split_loop_nor_fan_do`, `mesh_calc_normals_poly_prepare_cb`, `mesh_verts_calc_normals_accum_cb`, `mesh_edges_calc_vectors_cb`, `mesh_edges_sharp_tag`, `extract_lnor_loop_mesh`, and `extract_pos_nor_loop_mesh` 

## Notes

### Mimicking Blender
* The file, lib structure, and even file names, are lifted directly from Blender where possible.  This is to help those already familar with Blender know exactly what is what.
* The `bf_bmesh` project under Blender is currently compiled as `C` code; not `C++`. As such, class-like types and parameter references are not used in the API here when reimplementing `bf_blenlib` functions since they need to remain consumable in `bf_bmesh`.

* For now, naming conventions for APIs like sub_v3_v3v3(...) is kept even though the SSE variant of this API may now be working with a SSE type (effectively 4 floats).

### SSE Details
* Passing the SSE types, like `__m128`, by reference is not necessary in function signatures.  No speedup was observed when doing so.  Even the intrinsic functions themselves take their parameters as value types.

* Usage of the [_VECTORCALL](https://docs.microsoft.com/en-us/cpp/cpp/vectorcall?view=vs-2019) calling convention will benefit some non-inlined methods.

* The focus is on SSE2.  Known downsides include:
  * Dot products are slower than necessary (requires SSE4 for better performance)
  * A useful FMA intrinsic cannot be used (requires FMA CPU support for better performance)

### Compiler Flags
* The most optimal set of compiler flags for this benchmark program deviates from what Blender uses. Rather than using the optimal flags, the flags are instead kept as close as possible with Blender. See the CMakeLists.txt for some notes there.

### Workload Details
* To simulate a realistic workflow, a quad-sphere is used for geometry data where applicable (a cube with sub-d level 1 applied). This yields 48 triangles worth of data to loop over in a given benchmark iteration.  For example, the timing output from the "normal_tri" tests is effectively the time it takes to calculate the normals for all 48 triangles.


## Performance Scorecard
Command: `blender_bench.exe --benchmark_report_aggregates_only=true --benchmark_repetitions=10`

| Test        | i7-8750H Time | Xeon E5-1650 v4 Time | Notes |
| ------------| ------------- | ----------------- | ----- |
| BB_dot_v3v3_mean | 121 ns (1x) | 152 ns (1x) | Baseline |
| BB_dot_m128m128_mean | 131 ns (0.91x) | 152 ns (1x) | Bad |
| BB_dot_m128m128_lf3_mean | 131 ns (0.91x) | 160 ns (0.95x) | Bad |
| BB_dot_m128m128_lf4_mean | 109 ns (1.09x) | 130 ns (1.17x) | Good |
| BB_dot_m128m128_native_mean | 109 ns (1.09x) | 127 ns (1.20x) | Best |
| | | | |
| BB_cross_tri_v3_mean | 178 ns (1x) | 201 ns (1x) | Baseline |
| BB_cross_tri_m128_mean | 158 ns (1.12x) | 183 ns (1.1x) | Good |
| BB_cross_tri_m128_lf3sf3_mean | 189 ns (0.94x) | 210 ns (0.96x) | Bad |
| BB_cross_tri_m128_lf3sf4_mean | 174 ns (1.02x) | 180 ns (1.12x) | Ok |
| BB_cross_tri_m128_lf4sf4_mean | 168 ns (1.06x) | 187 ns (1.08x) | Ok |
| BB_cross_tri_m128_native_mean | 145 ns (1.23x) | 156 ns (1.29x) | Best |
| | | | |
| BB_normal_tri_v3_mean | 408 ns (1x) | 481 ns (1x) | Baseline |
| BB_normal_tri_m128_mean | 265 ns (1.54x) | 305 ns (1.58x) | Good |
| BB_normal_tri_m128_lf3sf3_mean | 286 ns (1.43x) | 320 ns (1.5x) | Good |
| BB_normal_tri_m128_lf3sf4_mean | 259 ns (1.58x) | 297 ns (1.62x) | Good |
| BB_normal_tri_m128_lf4sf4_mean | 237 ns (1.72x) | 271 ns (1.78x) | Good |
| BB_normal_tri_m128_native_mean | 227 ns (1.80x) | 254 ns (1.9x) | Best |
| | | | |
| BB_is_quad_flip_v3_mean | 155 ns (1x) | 200 ns (1x) | Baseline |
| BB_is_quad_flip_m128_mean | 128 ns (1.21x) | 153 ns (1.31x) | Good |
| BB_is_quad_flip_m128_lf3_mean | 136 ns (1.14x) | 163 ns (1.23x) | Good |
| BB_is_quad_flip_m128_lf4_mean | 117 ns (1.32x) | 146 ns (1.37x) | Good |
| BB_is_quad_flip_m128_native_mean | 116 ns (1.34x) | 144 ns (1.39x) | Best |
| | | | |
| BB_GPU_normal_convert_i10_v3_mean | 142 ns (1x) | 159 ns (1x) | Baseline |
| BB_GPU_normal_convert_i10_m128_mean | 141 ns (1.01x) | 142 ns (1.12x) | Ok |
| BB_GPU_normal_convert_i10_m128_lf3_mean | 141 ns (1.01x) | 150 ns (1.06x) | Ok |
| BB_GPU_normal_convert_i10_m128_lf4_mean | 130 ns (1.09x) | 140 ns (1.14x) | Good |
| BB_GPU_normal_convert_i10_m128_native_mean | 128 ns (1.11x) | 135 ns (1.18x) | Best |


## Summary
### Odd results
The *_sse variations above should not be any faster/slower than the *_lf3sf3 variants.  However, this is not the case; they are substantially faster in some cases.  It's surprising that even Clang shows codegen differences between the 2 variants.

This would allow for quick integration back into the main Blender codebase as all the callers would not notice. The functions would just get faster; for "free".

### Is SSE or optimization at this level worth it?
Speedups of any magnitude are worthwhile if:
- Maintainability is enhanced or not made any worse
- Readability is enhanced or not made any worse
- Chances of additional bugs are low
- The speedups are meaningful for the human operating the software
- There's few, if any, cases where things become slower

If this benchmark's code were to be used, it would score favorably enough in some of those categories but not so much in others.

Is it worth it? Maybe.

Functions with three or more blocks of mathematical operations would stand to gain the most (like cross_tri_v3 and normal_tri_v3 etc.) especially if intermediate results can be kept in SSE types instead of loading/storing to float arrays.

### Blender's vector storage format
The current scorecard speaks for itself:  The usage of 3 floats in the APIs, DNA structs,and RNA interfaces is suboptimal when compared against either 4 floats or the usage of SSE types directly. This should not come as a surprise.

  * Performance is affected so much due to this that, in some simple cases, SSE becomes slower than normal code (dot product especially).
  * To fully leverage SSE, the usage of SSE types would be ideal, followed closely by 4 float storage.
  * SoA, data-oriented, designs in theory can be used for further speedups. However, those designs often run counter to the access patterns that Blender needs (e.g. during editing).

Unfortunately, the memory (extra 25% at minimum) and 16 byte alignment requirements would bloat runtime memory usage well beyond what would be tolerable.

TODO: Figure out what changes are allowed in DNA and explore ways of testing a better format out in isolated cases. For example, for calculating normals:
* Could just the `CD_NORMAL` layer be changed as an experiment?
* Could a `CD_NORMAL_SSE` layer be created and enlighten a few critical code paths to see if performance can be gained?


## Building and Usage

### Dependencies
* [benchmark](https://github.com/google/benchmark)

You can use [vcpkg](https://github.com/Microsoft/vcpkg) to install it like this:
```Shell
Windows:
> vcpkg install benchmark --triplet x64-windows

Linux:
~ vcpkg install benchmark --triplet x64-linux
```

### Compile
See [integration](https://github.com/microsoft/vcpkg/blob/master/docs/users/integration.md) for how to attain the location of the vcpkg.cmake file.

```Shell
Windows:
> makedir build
> cd build
> cmake .. -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE=<LOCATION OF vcpkg.cmake FILE>
> cmake --build . --config Release

Linux:
~ makedir build
~ cd build
~ cmake .. -DVCPKG_TARGET_TRIPLET=x64-linux -DCMAKE_TOOLCHAIN_FILE=<LOCATION OF vcpkg.cmake FILE>
~ cmake --build . --config Release
```

### Run
Help
```Shell
blender_bench.exe --help
```
For quick spot checks:
```Shell
blender_bench.exe --benchmark_repetitions=5 --benchmark_filter=normal_tri
```
For reporting and better averages:
```Shell
blender_bench.exe --benchmark_report_aggregates_only=true --benchmark_repetitions=10
```

### Test
```Shell
ctest --output-on-failure -C Release
```

## Supported Compilers
* Microsoft Visual C++ 2019 (and likely earlier)
* Clang 5.0+ (and likely earlier)
* GCC 6.1+ (and likely earlier)


## License

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

Licensed under the GNU General Public License version 3 <https://opensource.org/licenses/GPL-3.0>.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

## Used third-party tools

This software would not be possible without the help of these great resources. Thanks a lot!

* [benchmark](https://github.com/google/benchmark) for benchmarking support and reporting
