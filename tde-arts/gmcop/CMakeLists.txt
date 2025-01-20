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
  ${CMAKE_BINARY_DIR}/mcop
  ${CMAKE_SOURCE_DIR}/mcop
  ${GLIB2_INCLUDE_DIRS}
)


##### headers ###################################

install(
  FILES giomanager.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/${CMAKE_PROJECT_NAME} )


##### gmcop (shared lib) ########################

tde_add_library( gmcop SHARED
  SOURCES giomanager.cpp
  VERSION 1.0.0
  LINK mcop-shared ${GLIB2_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)
