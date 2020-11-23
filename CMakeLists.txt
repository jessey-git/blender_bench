cmake_minimum_required (VERSION 3.8)
project(blender_bench VERSION 1.0.0)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/blenlib)

if(MSVC)
	# Note: More performance can be gained by using these settings:
	#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /GL")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GL")
	#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG:incremental")
endif()

set(SRC
	"bench_cross_tri.cpp"
	"bench_is_quad_flip.cpp"
	"bench_geom.cpp"
	"bench_math.cpp"
	"bench_matrix.cpp"
	"bench_normal_tri.cpp"
	"bench_simulation.cpp"
	"bench_quantize.cpp"
)

add_executable (blender_bench "blender_bench.cpp" "${SRC}")

find_package(benchmark CONFIG REQUIRED)
target_link_libraries(blender_bench PRIVATE benchmark::benchmark benchmark::benchmark_main)

add_subdirectory(blenlib)
target_link_libraries(blender_bench PRIVATE bf_blenlib)

include(CTest)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
