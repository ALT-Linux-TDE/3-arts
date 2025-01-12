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
include( CheckTypeSize )


##### check for ogg/vorbis ######################

set( GSL_HAVE_OGGVORBIS 0 )
if( WITH_VORBIS )

  pkg_search_module( VORBIS vorbis )

  if( VORBIS_FOUND )

    pkg_search_module( VORBISFILE vorbisfile )

    if( VORBISFILE_FOUND )

      set( GSL_HAVE_OGGVORBIS 1 )

      tde_save_and_set( CMAKE_REQUIRED_LIBRARIES ${VORBISFILE_LIBRARIES} )
      check_c_source_compiles(
        "#include <vorbis/vorbisfile.h>
        int main() { ov_read_float(0,0,0); return 0; } "
        GSL_HAVE_OGGVORBIS_RC3 )
      if( NOT GSL_HAVE_OGGVORBIS_RC3 )
        set( GSL_HAVE_OGGVORBIS_RC3 0 )
      endif( NOT GSL_HAVE_OGGVORBIS_RC3 )
      tde_restore( CMAKE_REQUIRED_LIBRARIES )

      list( APPEND PC_LIB_REQUIRE "vorbis" "vorbisfile" )

    else( VORBISFILE_FOUND )

      tde_message_fatal( "Ogg/Vorbis was requested but `libvorbisfile` was not found on your system." )

    endif( VORBISFILE_FOUND )

  else( VORBIS_FOUND )

    tde_message_fatal( "Ogg/Vorbis was requested but `libvorbis` was not found on your system." )

  endif( VORBIS_FOUND )

endif( WITH_VORBIS )


##### check for libmad MPEG decoder #############

set( GSL_HAVE_LIBMAD 0 )
if( WITH_MAD )

  pkg_search_module( MAD libmad )
  set( MAD_MODULE_NAME "libmad" )
  if( NOT MAD_FOUND )
    pkg_search_module( MAD mad )
    set( MAD_MODULE_NAME "mad" )
  endif()

  if( MAD_FOUND )
    set( GSL_HAVE_LIBMAD 1 )
    list( APPEND PC_LIB_REQUIRE ${MAD_MODULE_NAME} )
  else( MAD_FOUND )
    find_library( MAD_LIBRARIES NAMES mad )
    find_path( MAD_INCLUDE_DIRS mad.h )
    if( NOT MAD_LIBRARIES )
        tde_message_fatal( "MAD support was requested but `libmad` was not found on your system." )
    endif( NOT MAD_LIBRARIES )
  endif( MAD_FOUND )

endif( WITH_MAD )


##### check for some type sizes #################

check_type_size( pthread_mutex_t GSL_SIZEOF_PTH_MUTEX_T )
check_type_size( pthread_cond_t GSL_SIZEOF_PTH_COND_T )
check_type_size( intmax_t GSL_SIZEOF_STD_INTMAX_T )

tde_save_and_set( CMAKE_REQUIRED_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} )
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
tde_restore( CMAKE_REQUIRED_LIBRARIES )

set( GSL_USE_GSL_GLIB 1 )
set( GSL_USE_ARTS_THREADS 1 )


##### save cached value of required packages ####

set( PC_LIB_REQUIRE "${PC_LIB_REQUIRE}" CACHE INTERNAL "List of required packages" FORCE )
