
SET(TESTSRC
	"test_math.cpp"
)

add_executable(blender_bench_test "${TESTSRC}")
target_link_libraries(blender_bench_test PRIVATE bf_blenlib)

add_test(NAME "blender_bench_test" COMMAND blender_bench_test)