SET(PRJ_PATH ..)

SET(TESTS
  values_unittest
  manifest_unittest
  manifest_handler_unittest
  parser_unittest
  string_util_unittest
  version_number_unittest
)
SET(DESTINATION_DIR manifest-parser-ut)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../)

# Executables
ADD_EXECUTABLE(values_unittest
               utils_values_unittest.cc)
ADD_EXECUTABLE(manifest_unittest
               parser_manifest_unittest.cc)
ADD_EXECUTABLE(manifest_handler_unittest
               parser_manifest_handler_unittest.cc)
ADD_EXECUTABLE(parser_unittest
               parser_unittest.cc)
ADD_EXECUTABLE(string_util_unittest string_util_unittest.cc)
ADD_EXECUTABLE(version_number_unittest version_number_unittest.cc)

INSTALL(DIRECTORY test_samples/ DESTINATION ${SHAREDIR}/${DESTINATION_DIR}/test_samples)

FOREACH(test ${TESTS})
APPLY_PKG_CONFIG(${test} PUBLIC
  LIBXML_DEPS
  PKGMGR_INFO_DEPS
  PKGMGR_PARSER_DEPS
  VCONF_DEPS
  Boost
  GTEST
)
ENDFOREACH(test)

# FindGTest module do not sets all needed libraries in GTEST_LIBRARIES and
# GTest main libraries is still missing, so additional linking of
# GTEST_MAIN_LIBRARIES is needed.
TARGET_LINK_LIBRARIES(values_unittest PUBLIC ${TARGET_LIBNAME_PARSER} ${GTEST_MAIN_LIBRARIES})
TARGET_LINK_LIBRARIES(manifest_unittest PUBLIC ${TARGET_LIBNAME_PARSER} ${GTEST_MAIN_LIBRARIES})
TARGET_LINK_LIBRARIES(manifest_handler_unittest PUBLIC ${TARGET_LIBNAME_PARSER} ${GTEST_MAIN_LIBRARIES})
TARGET_LINK_LIBRARIES(parser_unittest PUBLIC ${TARGET_LIBNAME_PARSER} ${GTEST_MAIN_LIBRARIES})
TARGET_LINK_LIBRARIES(string_util_unittest PUBLIC ${TARGET_LIBNAME_UTILS} ${GTEST_MAIN_LIBRARIES})
TARGET_LINK_LIBRARIES(version_number_unittest PUBLIC ${TARGET_LIBNAME_UTILS} ${GTEST_MAIN_LIBRARIES})

FOREACH(test ${TESTS})
  INSTALL(TARGETS ${test} DESTINATION ${BINDIR}/${DESTINATION_DIR})
ENDFOREACH(test)
