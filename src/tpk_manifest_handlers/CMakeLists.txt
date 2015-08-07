# Target - sources
SET(SRCS
  application_manifest_constants.cc
  author_handler.cc
  datacontrol_handler.cc
  description_handler.cc
  metadata_handler.cc
  service_application_handler.cc
  tpk_config_parser.cc
  ui_application_handler.cc
)

# Target - definition
ADD_LIBRARY(${TARGET_LIBNAME_TPK_MANIFEST_HANDLERS} SHARED ${SRCS})

# Target - includes
TARGET_INCLUDE_DIRECTORIES(${TARGET_LIBNAME_TPK_MANIFEST_HANDLERS} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../")

# Extra
SET_TARGET_PROPERTIES(${TARGET_LIBNAME_TPK_MANIFEST_HANDLERS} PROPERTIES VERSION ${VERSION})
SET_TARGET_PROPERTIES(${TARGET_LIBNAME_TPK_MANIFEST_HANDLERS} PROPERTIES SOVERSION ${VERSION_MAJOR})

# Install
INSTALL(TARGETS ${TARGET_LIBNAME_TPK_MANIFEST_HANDLERS} DESTINATION ${LIB_INSTALL_DIR})

# Install includes
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${INCLUDEDIR}
        FILES_MATCHING PATTERN "*.h")

# Configure and install pkgconfig file
SET(PKG_FILE tpk-manifest-handlers.pc)
CONFIGURE_FILE(${PKG_FILE}.in ${PKG_FILE} @ONLY)
INSTALL(FILES ${PKG_FILE} DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
