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
  ${CMAKE_CURRENT_BINARY_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_BINARY_DIR}/mcop
  ${GLIB2_INCLUDE_DIRS}
)


##### gslpp (static lib) ########################

set( target artsgslpp )

tde_add_library( ${target} STATIC_PIC
  SOURCES datahandle.cpp
)
