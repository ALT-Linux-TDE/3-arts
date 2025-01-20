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


# required stuff

tde_setup_architecture_flags( )

include(TestBigEndian)
test_big_endian(WORDS_BIGENDIAN)

tde_setup_largefiles( )


##### check for TQt #############################

find_package( TQt )


##### check for gcc visibility support #########

if( WITH_GCC_VISIBILITY )
  tde_setup_gcc_visibility( )
endif( )


##### check for include files ###################

check_include_file( "sys/time.h" HAVE_SYS_TIME_H )
check_include_file( "time.h" TIME_WITH_SYS_TIME )
check_include_file( "stdio.h" HAVE_STDIO_H )
check_include_file( "stdlib.h" HAVE_STDLIB_H )
check_include_file( "string.h" HAVE_STRING_H )
check_include_file( "strings.h" HAVE_STRINGS_H )
check_include_file( "ctype.h" HAVE_CTYPE_H )
check_include_file( "malloc.h" HAVE_MALLOC_H )
check_include_file( "memory.h" HAVE_MEMORY_H )
check_include_file( "dlfcn.h" HAVE_DLFCN_H )
check_include_file( "sys/soundcard.h" HAVE_SYS_SOUNDCARD_H )
check_include_file( "pthread.h" HAVE_LIBPTHREAD )


##### check for system libraries ################

set( DL_LIBRARIES dl )
check_library_exists( ${DL_LIBRARIES} dlopen /lib HAVE_LIBDL )
if( NOT HAVE_LIBDL )
  unset( DL_LIBRARIES )
  check_function_exists( dlopen HAVE_DLOPEN )
  if( HAVE_DLOPEN )
    set( HAVE_LIBDL 1 )
  endif( HAVE_DLOPEN )
endif( NOT HAVE_LIBDL )

find_package( Threads )


##### check for functions #######################

tde_save_and_set( CMAKE_REQUIRED_LIBRARIES ${DL_LIBRARIES} )
check_function_exists( dlerror HAVE_DLERROR )
check_function_exists( strcmp HAVE_STRCMP )
check_function_exists( strchr HAVE_STRCHR )
check_function_exists( index HAVE_INDEX )
check_function_exists( strrchr HAVE_STRRCHR )
check_function_exists( rindex HAVE_RINDEX )
check_function_exists( memcpy HAVE_MEMCPY )
check_function_exists( bcopy HAVE_BCOPY )
tde_restore( CMAKE_REQUIRED_LIBRARIES )

check_prototype_definition( ioctl "int ioctl(int d, int request, ...)" "-1" "unistd.h;sys/ioctl.h" HAVE_IOCTL_INT_INT_DOTS )
check_prototype_definition( ioctl "int ioctl(int d, unsigned long request, ...)" "-1" "unistd.h;sys/ioctl.h" HAVE_IOCTL_INT_ULONG_DOTS )


##### check for audiofile #######################

set( HAVE_LIBAUDIOFILE 0 )
if( WITH_AUDIOFILE )

  pkg_search_module( AUDIOFILE audiofile )
  if( AUDIOFILE_FOUND )
    set( HAVE_LIBAUDIOFILE 1 )
    list( APPEND PC_LIB_REQUIRE "audiofile" )
  else( AUDIOFILE_FOUND )
    tde_message_fatal( "audiofile (wav) support is requested, but `libaudiofile` not found" )
  endif( AUDIOFILE_FOUND )

endif( WITH_AUDIOFILE )


##### check for alsa ############################

set( HAVE_LIBASOUND2 0 )
if( WITH_ALSA )

  pkg_search_module( ALSA alsa )

  if( ALSA_FOUND )

    set( HAVE_LIBASOUND2 1 )

    check_include_file( "alsa/asoundlib.h" HAVE_ALSA_ASOUNDLIB_H )
    if( NOT HAVE_ALSA_ASOUNDLIB_H )
      check_include_file( "sys/asoundlib.h" HAVE_SYS_ASOUNDLIB_H )
    endif( NOT HAVE_ALSA_ASOUNDLIB_H )

    tde_save_and_set( CMAKE_REQUIRED_LIBRARIES ${ALSA_LIBRARIES} )
    check_function_exists( snd_pcm_resume HAVE_SND_PCM_RESUME )
    tde_restore( CMAKE_REQUIRED_LIBRARIES )

    list( APPEND PC_LIB_REQUIRE "alsa" )

  else( ALSA_FOUND )

    tde_message_fatal( "ALSA support is requested, but not found on your system" )

  endif( ALSA_FOUND )

endif( WITH_ALSA )


##### check for esound #######################

set( HAVE_LIBESD 0 )
if( WITH_ESOUND )

  pkg_search_module( ESOUND esound )
  if( ESOUND_FOUND )
    set( HAVE_LIBESD 1 )
    list( APPEND PC_LIB_REQUIRE "esound" )
  else( ESOUND_FOUND )
    tde_message_fatal( "ESOUND support is requested, but `libesd` not found" )
  endif( ESOUND_FOUND )

endif( WITH_ESOUND )


##### check for JACK ############################

set( HAVE_LIBJACK 0 )
if( WITH_JACK )
  pkg_search_module( LIBJACK jack )
  if( LIBJACK_FOUND )
    set( HAVE_LIBJACK 1 )
    list( APPEND PC_LIB_REQUIRE "jack" )
  else( LIBJACK_FOUND )
    tde_message_fatal( "JACK support is requested, but `jack.pc` was not found" )
  endif( LIBJACK_FOUND )
endif( WITH_JACK )


##### check for SNDIO ###########################

set( HAVE_LIBSNDIO 0 )
if( WITH_SNDIO )
  check_include_file( "sndio.h" HAVE_SNDIO_H )
  if( HAVE_SNDIO_H )
    set( HAVE_LIBSNDIO 1 )
    set( LIBSNDIO_LIBRARIES "sndio" )
  else( HAVE_SNDIO_H )
    tde_message_fatal( "SNDIO support is requested, but `sndio.h` was not found" )
  endif( HAVE_SNDIO_H )
endif( WITH_SNDIO )


##### check for glib/gthread modules ############

pkg_search_module( GLIB2 glib-2.0 )

if( GLIB2_FOUND )
  pkg_search_module( GTHREAD2 gthread-2.0 )
  if( NOT GTHREAD2_FOUND )
    tde_message_message( "gthread-2.0 is required, but not found on your system" )
  endif( NOT GTHREAD2_FOUND )
else( GLIB2_FOUND )
  tde_message_fatal( "glib-2.0 is required, but not found on your system" )
endif( GLIB2_FOUND )


##### save cached value of required packages ####

set( PC_LIB_REQUIRE "${PC_LIB_REQUIRE}" CACHE INTERNAL "List of required packages" FORCE )
