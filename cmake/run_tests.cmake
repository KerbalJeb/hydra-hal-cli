set(CTEST_COVERAGE_COMMAND "gcov")
set(CTEST_MEMORYCHECK_TYPE "AddressSanitizer;UndefinedBehaviorSanitizer")

set(CTEST_SOURCE_DIRECTORY "./")
set(CTEST_BINARY_DIRECTORY cmake-build-debug-wsl/test)
set(CTEST_CMAKE_GENERATOR "Unix Makefiles")

ctest_start(Continuous)
ctest_configure()
ctest_build()
ctest_test()
ctest_coverage()
ctest_memcheck()
ctest_submit()