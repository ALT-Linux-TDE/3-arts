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


##### set project version ########################

include( TDEVersion )
cmake_minimum_required( VERSION ${TDE_CMAKE_MINIMUM_VERSION} )
tde_set_project_version( )


##### project settings ##########################

project( arts )

set( ARTS_MAJOR_VERSION 1 )
set( ARTS_MINOR_VERSION 5 )
set( ARTS_MICRO_VERSION 10 )
set( ARTS_VERSION "${ARTS_MAJOR_VERSION}.${ARTS_MINOR_VERSION}.${ARTS_MICRO_VERSION}" )


##### include essential cmake modules ###########

include( CheckCXXSourceCompiles )
include( CheckFunctionExists )
include( CheckSymbolExists )
include( CheckPrototypeDefinition )
include( CheckIncludeFile )
include( CheckLibraryExists )
include( FindPkgConfig )


##### include our cmake modules #################

include( TDEMacros )


##### setup install paths #######################

include( TDESetupPaths )
tde_setup_paths( )


##### user requested modules ####################

option( WITH_ALL_OPTIONS "Enable all optional support" OFF )
option( WITH_ALSA "Enable ALSA support" ON )
option( WITH_AUDIOFILE "Enable audiofile (wav) support" ON )
option( WITH_VORBIS "Enable Ogg/Vorbis support" ON )
option( WITH_MAD "Enable MAD mp3 decoder support" ON )
option( WITH_ESOUND "Enable ESOUND support" ${WITH_ALL_OPTIONS} )
option( WITH_JACK "Enable JACK support" ${WITH_ALL_OPTIONS} )
option( WITH_SNDIO "Enable SNDIO support" OFF )
option( WITH_GCC_VISIBILITY "Enable fvisibility and fvisibility-inlines-hidden" ${WITH_ALL_OPTIONS} )

option( BUILD_ALL "Build all" ON )
option( BUILD_DOC "Build doc" ${BUILD_ALL} )


##### configure checks

set( PC_LIB_REQUIRE "" CACHE INTERNAL "List of required packages" FORCE )
include( ConfigureChecks.cmake )


##### global compiler settings ##################

add_definitions( -DHAVE_CONFIG_H )

set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined" )


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


##### other data ################################

tde_conditional_add_project_docs( BUILD_DOC )


##### write config.h file #######################

configure_file( config.h.cmake config.h @ONLY )


##### write pkgconfig file ######################

list( LENGTH PC_LIB_REQUIRE PC_LIB_REQUIRES_FOUND )
if( PC_LIB_REQUIRES_FOUND GREATER 0 )
  list( SORT PC_LIB_REQUIRE )
  list( REMOVE_DUPLICATES PC_LIB_REQUIRE )
  string( REPLACE ";" " " PC_LIB_REQUIRES "Requires: ${PC_LIB_REQUIRE}" )
else( )
  set( PC_LIB_REQUIRES "" )
endif( )

string( REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" PC_EXEC_PREFIX ${EXEC_INSTALL_PREFIX} )
string( REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" PC_INCLUDE_DIR ${INCLUDE_INSTALL_DIR}/${CMAKE_PROJECT_NAME} )
string( REGEX REPLACE "^${CMAKE_INSTALL_PREFIX}" "\${prefix}" PC_LIB_DIR ${LIB_INSTALL_DIR} )
configure_file( arts.pc.cmake arts.pc @ONLY )
install( FILES ${CMAKE_CURRENT_BINARY_DIR}/arts.pc DESTINATION ${PKGCONFIG_INSTALL_DIR} )
