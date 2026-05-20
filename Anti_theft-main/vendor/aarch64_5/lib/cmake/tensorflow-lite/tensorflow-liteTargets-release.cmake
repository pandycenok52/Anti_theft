#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "tensorflow-lite::farmhash" for configuration "Release"
set_property(TARGET tensorflow-lite::farmhash APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(tensorflow-lite::farmhash PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libfarmhash.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS tensorflow-lite::farmhash )
list(APPEND _IMPORT_CHECK_FILES_FOR_tensorflow-lite::farmhash "${_IMPORT_PREFIX}/lib/libfarmhash.a" )

# Import target "tensorflow-lite::fft2d_fftsg" for configuration "Release"
set_property(TARGET tensorflow-lite::fft2d_fftsg APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(tensorflow-lite::fft2d_fftsg PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libfft2d_fftsg.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS tensorflow-lite::fft2d_fftsg )
list(APPEND _IMPORT_CHECK_FILES_FOR_tensorflow-lite::fft2d_fftsg "${_IMPORT_PREFIX}/lib/libfft2d_fftsg.a" )

# Import target "tensorflow-lite::fft2d_fftsg2d" for configuration "Release"
set_property(TARGET tensorflow-lite::fft2d_fftsg2d APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(tensorflow-lite::fft2d_fftsg2d PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libfft2d_fftsg2d.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS tensorflow-lite::fft2d_fftsg2d )
list(APPEND _IMPORT_CHECK_FILES_FOR_tensorflow-lite::fft2d_fftsg2d "${_IMPORT_PREFIX}/lib/libfft2d_fftsg2d.a" )

# Import target "tensorflow-lite::tensorflow-lite" for configuration "Release"
set_property(TARGET tensorflow-lite::tensorflow-lite APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(tensorflow-lite::tensorflow-lite PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libtensorflow-lite.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS tensorflow-lite::tensorflow-lite )
list(APPEND _IMPORT_CHECK_FILES_FOR_tensorflow-lite::tensorflow-lite "${_IMPORT_PREFIX}/lib/libtensorflow-lite.a" )

# Import target "tensorflow-lite::pthreadpool" for configuration "Release"
set_property(TARGET tensorflow-lite::pthreadpool APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(tensorflow-lite::pthreadpool PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libpthreadpool.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS tensorflow-lite::pthreadpool )
list(APPEND _IMPORT_CHECK_FILES_FOR_tensorflow-lite::pthreadpool "${_IMPORT_PREFIX}/lib/libpthreadpool.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
