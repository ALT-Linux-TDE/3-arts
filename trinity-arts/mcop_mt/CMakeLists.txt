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
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/flow
  ${CMAKE_BINARY_DIR}/mcop
  ${CMAKE_SOURCE_DIR}/flow
  ${CMAKE_SOURCE_DIR}/mcop
  ${GLIB2_INCLUDE_DIRS}
)


##### mcop_mt (shared lib ) #####################

tde_add_library( mcop_mt SHARED
  SOURCES threads_posix.cc
  VERSION 1.0.0
  LINK artsflow-shared pthread
  DESTINATION ${LIB_INSTALL_DIR}
)
