#################################################
#
#  (C) 2010 Serghei Amelian
#  serghei (DOT) amelian (AT) gmail.com
#
#  Improvements and feedback are welcome
#
#  This file is released under GPL >= 2
#
#################################################

include_directories(
  ${CMAKE_BINARY_DIR}/mcop # for arts_export.h
  ${CMAKE_SOURCE_DIR}/mcop # for common.h
)


##### mcopidl ###################################

set( target mcopidl )

set( ${target}_SRCS
  mcopidl.cc yacc.cc scanner.cc namespace.cc
)

tde_add_executable( ${target}
  SOURCES ${${target}_SRCS}
  LINK mcop-shared
  DESTINATION ${BIN_INSTALL_DIR}
)
