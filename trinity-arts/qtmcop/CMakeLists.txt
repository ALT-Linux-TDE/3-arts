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

add_definitions(
  ${TQT_CFLAGS_OTHER}
)

include_directories(
  ${TQT_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/mcop
  ${CMAKE_SOURCE_DIR}/mcop
)

link_directories(
  ${TQT_LIBRARY_DIRS}
)


##### headers ###################################

install( FILES
    qiomanager.h
  DESTINATION ${INCLUDE_INSTALL_DIR} )


##### qtmcop ##############################

set( qtmcop_SRCS qiomanager.cc )

tde_add_library( qtmcop SHARED AUTOMOC
  SOURCES ${qtmcop_SRCS}
  VERSION 1.0.0
  LINK mcop-shared ${TQT_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)
