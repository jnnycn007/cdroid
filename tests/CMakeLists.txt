find_package(GTEST)
if (BUILD_CDROID_TESTS AND GTEST_FOUND)
  add_subdirectory(gui)
  add_subdirectory(porting)
endif(BUILD_CDROID_TESTS AND GTEST_FOUND)
