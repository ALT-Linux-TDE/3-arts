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

##### configure checks

include( ConfigureChecks.cmake )


##### write configuration #######################

configure_file( gslconfig.h.in gslconfig.h @ONLY )


##### include paths #############################

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/mcop
  ${CMAKE_SOURCE_DIR}/mcop # for thread.h
  ${CMAKE_CURRENT_BINARY_DIR}/.. # for gsl/gslconfig.h
  ${CMAKE_CURRENT_SOURCE_DIR}/.. # for gsl/gslcommon.h
  ${GLIB2_INCLUDE_DIRS}
)


##### headers ###################################

install( FILES
    gslcommon.h gsldatacache.h gsldatahandle.h
    gsldefs.h gslloader.h gslmath.h gslfilter.h
    gsldatautils.h gsldatahandle-vorbis.h gslconvert.h
    gslfft.h gslieee754.h gslsignal.h gslmagic.h
    gslengine.h gslwaveosc.h gslwavechunk.h
    gsldatahandle-mad.h gslosctable.h gsloscillator.h
    ${CMAKE_CURRENT_BINARY_DIR}/gslconfig.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/${CMAKE_PROJECT_NAME}/gsl )


##### gsl (static lib) ##########################

set( target artsgsl )

set( ${target}_SRCS
  gsldatacache.c gsldatahandle.c gslwavechunk.c
  gsldatahandle-vorbis.c gslmath.c gslfilter.c
  gslcommon.c gsldatautils.c gslmagic.c gslloader-wav.c
  gslloader-gslwave.c gslloader-mad.c gslloader-oggvorbis.c
  gslconvert.c gslfft.c gslsignal.c gslloader.c
  gslwaveosc.c gslengine.c gsloputil.c gslopmaster.c
  gslopschedule.c gsldatahandle-mad.c gslosctable.c
  gsloscillator.c gslfilehash.c gslartsthreads.cpp
)

tde_add_library( ${target} STATIC_PIC
  SOURCES ${${target}_SRCS}
  LINK ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${VORBISFILE_LIBRARIES} ${VORBIS_LIBRARIES} ${MAD_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)
