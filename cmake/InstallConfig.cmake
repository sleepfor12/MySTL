if(NOT DEFINED MYSTL_INCLUDE_DIR)
    set(MYSTL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
endif()

install(
    DIRECTORY ${MYSTL_INCLUDE_DIR}/mystl
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.hpp"
    PATTERN "*.inl"
    PATTERN "__detail" EXCLUDE
)

include(GNUInstallDirs)

install(
    TARGETS mystl
    EXPORT mystl-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT mystl-targets
    FILE mystl-targets.cmake
    NAMESPACE mystl::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mystl
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/mystl-config.cmake.in
    ${CMAKE_BINARY_DIR}/mystl-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mystl
    PATH_VARS MYSTL_INCLUDE_DIR
    INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
)

install(
    FILES ${CMAKE_BINARY_DIR}/mystl-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mystl
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/mystl-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES ${CMAKE_BINARY_DIR}/mystl-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mystl
)

configure_file(
    ${CMAKE_SOURCE_DIR}/cmake/mystl.pc.in
    ${CMAKE_BINARY_DIR}/mystl.pc
    @ONLY
)

install(
    FILES ${CMAKE_BINARY_DIR}/mystl.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

