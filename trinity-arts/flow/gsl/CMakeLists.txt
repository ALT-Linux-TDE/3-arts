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

include( CheckCSourceCompiles )


##### check for ogg/vorbis ######################

set( GSL_HAVE_OGGVORBIS 0 )
if( WITH_VORBIS )

  pkg_search_module( VORBIS vorbis )

  if( VORBIS_FOUND )

    pkg_search_module( VORBISFILE vorbisfile )

    if( VORBISFILE_FOUND )

      set( GSL_HAVE_OGGVORBIS 1 )

      set( bak_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} )
      set( CMAKE_REQUIRED_LIBRARIES ${VORBISFILE_LIBRARIES} )
      check_c_source_compiles(
        "#include <vorbis/vorbisfile.h>
        int main() { ov_read_float(0,0,0); return 0; } "
        GSL_HAVE_OGGVORBIS_RC3 )
      if( NOT GSL_HAVE_OGGVORBIS_RC3 )
        set( GSL_HAVE_OGGVORBIS_RC3 0 )
      endif( NOT GSL_HAVE_OGGVORBIS_RC3 )
      set( CMAKE_REQUIRED_LIBRARIES ${bak_CMAKE_REQUIRED_LIBRARIES} )

    else( VORBISFILE_FOUND )

      message(FATAL_ERROR "\nOgg/Vorbis support are requested, but `libvorbisfile` not found" )

    endif( VORBISFILE_FOUND )

  else( VORBIS_FOUND )

    message(FATAL_ERROR "\nOgg/Vorbis support are requested, but `libvorbis` but found" )

  endif( VORBIS_FOUND )

endif( WITH_VORBIS )


##### check for libmad MPEG decoder #############

set( GSL_HAVE_LIBMAD 0 )
if( WITH_MAD )

  pkg_search_module( MAD mad )

  if( MAD_FOUND )
    set( GSL_HAVE_LIBMAD 1 )
  else( MAD_FOUND )
    find_library( MAD_LIBRARIES NAMES mad )
    find_path( MAD_INCLUDE_DIRS mad.h )
    if( NOT MAD_LIBRARIES )
        message(FATAL_ERROR "\nMAD support is requested, but `libmad` not found" )
    endif( NOT MAD_LIBRARIES )
  endif( MAD_FOUND )

endif( WITH_MAD )


##### check for some type sizes #################

include( CheckTypeSize )

check_type_size( pthread_mutex_t GSL_SIZEOF_PTH_MUTEX_T )
check_type_size( pthread_cond_t GSL_SIZEOF_PTH_COND_T )
check_type_size( intmax_t GSL_SIZEOF_STD_INTMAX_T )

set( bak_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} )
set( CMAKE_REQUIRED_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} )
check_c_source_compiles(
  "#define _XOPEN_SOURCE 500
  #include <pthread.h>
  int main()
  {
      int (*attr_settype) (pthread_mutexattr_t *__attr, int __kind) = pthread_mutexattr_settype;
      int val = PTHREAD_MUTEX_RECURSIVE; attr_settype = 0; val = 0;
      return 0;
  }"
  GSL_HAVE_MUTEXATTR_SETTYPE )
  if( NOT GSL_HAVE_MUTEXATTR_SETTYPE )
    set( GSL_HAVE_MUTEXATTR_SETTYPE 0 )
  endif( NOT GSL_HAVE_MUTEXATTR_SETTYPE )
set( CMAKE_REQUIRED_LIBRARIES ${bak_CMAKE_REQUIRED_LIBRARIES} )

set( GSL_USE_GSL_GLIB 1 )
set( GSL_USE_ARTS_THREADS 1 )


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
  DESTINATION ${INCLUDE_INSTALL_DIR}/gsl )


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
  gsloscillator.c gslfilehash.c gslartsthreads.cc
)

tde_add_library( ${target} STATIC_PIC
  SOURCES ${${target}_SRCS}
  LINK ${GLIB2_LIBRARIES} ${GTHREAD2_LIBRARIES} ${VORBISFILE_LIBRARIES} ${VORBIS_LIBRARIES} ${MAD_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)

