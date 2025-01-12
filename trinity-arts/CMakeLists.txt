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

cmake_minimum_required( VERSION 2.8 )


##### include essential cmake modules ###########

include( CheckCXXSourceCompiles )
include( CheckFunctionExists )
include( CheckIncludeFile )
include( CheckLibraryExists )
include( FindPkgConfig )


##### include our cmake modules #################
set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )
include( TDEMacros )


##### project settings ##########################

project( arts )

set( ARTS_MAJOR_VERSION 1 )
set( ARTS_MINOR_VERSION 5 )
set( ARTS_MICRO_VERSION 10 )
set( ARTS_VERSION "${ARTS_MAJOR_VERSION}.${ARTS_MINOR_VERSION}.${ARTS_MICRO_VERSION}" )


##### user requested options ####################

option( WITH_ALL_OPTIONS "Enable all optional support" OFF )
option( WITH_ALSA "Enable ALSA support" ON )
option( WITH_AUDIOFILE "Enable audiofile (wav) support" ON )
option( WITH_VORBIS "Enable Ogg/Vorbis support" ON )
option( WITH_MAD "Enable MAD mp3 decoder support" ON )
option( WITH_ESOUND "Enable ESOUND support" ${WITH_ALL_OPTIONS} )
option( WITH_JACK "Enable JACK support" ${WITH_ALL_OPTIONS} )
option( WITH_SNDIO "Enable SNDIO support" OFF )
option( WITH_GCC_VISIBILITY "Enable fvisibility and fvisibility-inlines-hidden" ${WITH_ALL_OPTIONS} )


##### paths setup ###############################

tde_setup_install_path( EXEC_INSTALL_PREFIX   "${CMAKE_INSTALL_PREFIX}" )
tde_setup_install_path( BIN_INSTALL_DIR       "${EXEC_INSTALL_PREFIX}/bin"  )
tde_setup_install_path( LIB_INSTALL_DIR       "${EXEC_INSTALL_PREFIX}/lib${LIB_SUFFIX}" )
tde_setup_install_path( INCLUDE_INSTALL_DIR   "${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME}" )
tde_setup_install_path( PKGCONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/pkgconfig" )


##### setup architecture flags ##################

tde_setup_architecture_flags( )


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

set( bak_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} )
set( CMAKE_REQUIRED_LIBRARIES ${DL_LIBRARIES} )
check_function_exists( dlerror HAVE_DLERROR )
check_function_exists( strcmp HAVE_STRCMP )
check_function_exists( strchr HAVE_STRCHR )
check_function_exists( index HAVE_INDEX )
check_function_exists( strrchr HAVE_STRRCHR )
check_function_exists( rindex HAVE_RINDEX )
check_function_exists( memcpy HAVE_MEMCPY )
check_function_exists( bcopy HAVE_BCOPY )
set( CMAKE_REQUIRED_LIBRARIES ${bak_CMAKE_REQUIRED_LIBRARIES} )
unset( bak_CMAKE_REQUIRED_LIBRARIES )


##### check for audiofile #######################

set( HAVE_LIBAUDIOFILE 0 )
if( WITH_AUDIOFILE )

  pkg_search_module( AUDIOFILE audiofile )
  if( AUDIOFILE_FOUND )
    set( HAVE_LIBAUDIOFILE 1 )
  else( AUDIOFILE_FOUND )
    message(FATAL_ERROR "\naudiofile (wav) support are requested, but `libaudiofile` not found" )
  endif( AUDIOFILE_FOUND )

endif( WITH_AUDIOFILE )


##### check for alsa ############################

set( HAVE_LIBASOUND2 0 )
if( WITH_ALSA )

  pkg_search_module( ALSA alsa )

  if( ALSA_FOUND )

    # there is support only for ALSA 1.x

    set( HAVE_LIBASOUND2 1 )
    set( ALSA_PCM_OLD_SW_PARAMS_API 1 )
    set( ALSA_PCM_OLD_HW_PARAMS_API 1 )

    check_include_file( "alsa/asoundlib.h" HAVE_ALSA_ASOUNDLIB_H )
    if( NOT HAVE_ALSA_ASOUNDLIB_H )
      check_include_file( "sys/asoundlib.h" HAVE_SYS_ASOUNDLIB_H )
    endif( NOT HAVE_ALSA_ASOUNDLIB_H )

    set( bak_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} )
    set( CMAKE_REQUIRED_LIBRARIES ${ALSA_LIBRARIES} )
    check_function_exists( snd_pcm_resume HAVE_SND_PCM_RESUME )
    set( CMAKE_REQUIRED_LIBRARIES ${bak_CMAKE_REQUIRED_LIBRARIES} )
    unset( bak_CMAKE_REQUIRED_LIBRARIES )

  else( ALSA_FOUND )

    message(FATAL_ERROR "\nALSA support are requested, but not found on your system" )

  endif( ALSA_FOUND )

endif( WITH_ALSA )


##### check for esound #######################

set( HAVE_LIBESD 0 )
if( WITH_ESOUND )

  pkg_search_module( ESOUND esound )
  if( ESOUND_FOUND )
    set( HAVE_LIBESD 1 )
  else( ESOUND_FOUND )
    message(FATAL_ERROR "\nESOUND support is requested, but `libesd` not found" )
  endif( ESOUND_FOUND )

endif( WITH_ESOUND )


##### check for JACK ############################

set( HAVE_LIBJACK 0 )
if( WITH_JACK )
  pkg_search_module( LIBJACK jack )
  if( LIBJACK_FOUND )
    set( HAVE_LIBJACK 1 )
  else( LIBJACK_FOUND )
    message(FATAL_ERROR "\nJACK support is requested, but `jack.pc` was not found" )
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
    message(FATAL_ERROR "\ngthread-2.0 are required, but not found on your system" )
  endif( NOT GTHREAD2_FOUND )
else( GLIB2_FOUND )
  message(FATAL_ERROR "\nglib-2.0 are required, but not found on your system" )
endif( GLIB2_FOUND )


##### check for gcc visibility support #########
# FIXME
# This should check for [T]Qt3 visibility support

if( WITH_GCC_VISIBILITY )
 if( NOT UNIX )
 message(FATAL_ERROR "\ngcc visibility support was requested, but your system is not *NIX" )
 endif( NOT UNIX )
 set( __KDE_HAVE_GCC_VISIBILITY 1 )
 set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
 set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden")
endif( )


##### check for TQt #############################

find_package( TQt )


##### write config.h file #######################

configure_file( config.h.cmake config.h @ONLY )


##### write pkgconfig file ######################

set( PC_LIB_REQUIRE "" )
if( WITH_ALSA )
  list( APPEND PC_LIB_REQUIRE "alsa" )
endif( WITH_ALSA )
if( WITH_AUDIOFILE )
  list( APPEND PC_LIB_REQUIRE "audiofile" )
endif( WITH_AUDIOFILE )
if( WITH_VORBIS )
  list( APPEND PC_LIB_REQUIRE "vorbis" )
  list( APPEND PC_LIB_REQUIRE "vorbisfile" )
endif( WITH_VORBIS )
if( WITH_MAD )
  list( APPEND PC_LIB_REQUIRE "mad" )
endif( WITH_MAD )
if( WITH_ESOUND )
  list( APPEND PC_LIB_REQUIRE "esound" )
endif( WITH_ESOUND )
if( WITH_JACK )
  list( APPEND PC_LIB_REQUIRE "jack" )
endif( WITH_JACK )

list( LENGTH PC_LIB_REQUIRE PC_LIB_REQUIRES_FOUND )
if( PC_LIB_REQUIRES_FOUND GREATER 0 )
  string( REPLACE ";" " " PC_LIB_REQUIRES "Requires: ${PC_LIB_REQUIRE}" )
else( )
  set( PC_LIB_REQUIRES "" )
endif( )

string( REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" PC_EXEC_PREFIX ${EXEC_INSTALL_PREFIX} )
string( REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" PC_INCLUDE_DIR ${INCLUDE_INSTALL_DIR} )
string( REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" PC_LIB_DIR ${LIB_INSTALL_DIR} )
configure_file( arts.pc.cmake arts.pc @ONLY )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/arts.pc DESTINATION ${PKGCONFIG_INSTALL_DIR} )


##### global compiler settings ##################

add_definitions(
  -DHAVE_CONFIG_H
)

set( CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined" )


##### project subdirectories ####################

add_subdirectory( libltdl )
add_subdirectory( mcop )
add_subdirectory( mcopidl )
add_subdirectory( flow )
add_subdirectory( mcop_mt )
add_subdirectory( soundserver )
add_subdirectory( artsc )
add_subdirectory( gmcop )
add_subdirectory( qtmcop )
