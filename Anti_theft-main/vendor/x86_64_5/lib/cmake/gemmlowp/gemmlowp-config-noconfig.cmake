#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gemmlowp::eight_bit_int_gemm" for configuration ""
set_property(TARGET gemmlowp::eight_bit_int_gemm APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(gemmlowp::eight_bit_int_gemm PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libeight_bit_int_gemm.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS gemmlowp::eight_bit_int_gemm )
list(APPEND _IMPORT_CHECK_FILES_FOR_gemmlowp::eight_bit_int_gemm "${_IMPORT_PREFIX}/lib/libeight_bit_int_gemm.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
