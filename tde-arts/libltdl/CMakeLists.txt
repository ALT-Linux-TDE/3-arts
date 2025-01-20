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
)

tde_add_library( ltdlc STATIC_PIC
  SOURCES ltdl.c
  LINK ${DL_LIBRARIES}
)
