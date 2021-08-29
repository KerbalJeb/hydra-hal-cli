find_package(Doxygen)
# todo fix doc generation
if (DOXYGEN_FOUND)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  # request to configure the file
  configure_file(config.doxygen ${DOXYGEN_OUT} @ONLY)

  # note the option ALL which allows to build the docs together with the application
  add_custom_target( doc_doxygen ALL
                     COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
                     COMMENT "Generating API documentation with Doxygen")
else()
  message("Doxygen need to be installed to generate the doxygen documentation")
endif ()