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

# FIXME pthread support need review

configure_file( artsversion-new.h.in artsversion.h )


set( MCOPINC
  -I${CMAKE_CURRENT_SOURCE_DIR} # for kmedia2.idl
  -I${CMAKE_SOURCE_DIR}/mcop
  -I${CMAKE_SOURCE_DIR}/flow
)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/flow
  ${CMAKE_SOURCE_DIR}/flow
  ${CMAKE_BINARY_DIR}/mcop
  ${CMAKE_SOURCE_DIR}/mcop
)


##### headers ###################################

install( FILES
    soundserver.idl kmedia2.idl
    ${CMAKE_CURRENT_BINARY_DIR}/artsversion.h
    ${CMAKE_CURRENT_BINARY_DIR}/soundserver.h
    ${CMAKE_CURRENT_BINARY_DIR}/kmedia2.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/${CMAKE_PROJECT_NAME} )


##### other data ################################

install( FILES
    FileInputStream.mcopclass WavPlayObject.mcopclass
    GSLPlayObject.mcopclass
  DESTINATION ${LIB_INSTALL_DIR}/mcop/Arts )

install( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/soundserver.mcopclass
    ${CMAKE_CURRENT_BINARY_DIR}/kmedia2.mcoptype
    ${CMAKE_CURRENT_BINARY_DIR}/kmedia2.mcopclass
    ${CMAKE_CURRENT_BINARY_DIR}/soundserver.mcoptype
  DESTINATION ${LIB_INSTALL_DIR}/mcop )


##### kmedia2_idl (shared lib) ##################

add_custom_command(
  OUTPUT kmedia2.cpp
  COMMAND ../mcopidl/mcopidl ARGS -t ${MCOPINC} ${CMAKE_CURRENT_SOURCE_DIR}/kmedia2.idl
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kmedia2.idl
)

tde_add_library( kmedia2_idl SHARED
  SOURCES kmedia2.cpp
  VERSION 1.0.0
  LINK artsflow_idl-shared
  DESTINATION ${LIB_INSTALL_DIR}
  DEPENDENCIES mcopidl
)


##### soundserver_idl (shared lib) ##############

add_custom_command(
  OUTPUT soundserver.cpp soundserver.h
  COMMAND ../mcopidl/mcopidl ARGS -t ${MCOPINC} ${CMAKE_CURRENT_SOURCE_DIR}/soundserver.idl
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/soundserver.idl
)

tde_add_library( soundserver_idl SHARED
  SOURCES soundserver.cpp
  VERSION 1.0.0
  LINK kmedia2_idl-shared
  DESTINATION ${LIB_INSTALL_DIR}
  DEPENDENCIES mcopidl
)


##### artsgslplayobject (shared lib) ############

tde_add_library( artsgslplayobject SHARED
  SOURCES gslplayobject_impl.cpp
  VERSION 0.0.0
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${LIB_INSTALL_DIR}
)


##### artswavplayobject (shared lib) ############

tde_add_library( artswavplayobject SHARED
  SOURCES wavplayobject_impl.cpp
  VERSION 0.0.0
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${LIB_INSTALL_DIR}
)


##### kmedia2 (shared lib) ######################

tde_add_library( kmedia2 SHARED
  SOURCES fileinputstream_impl.cpp stdoutwriter_impl.cpp
  VERSION 1.0.0
  LINK kmedia2_idl-shared artsflow-shared
  DESTINATION ${LIB_INSTALL_DIR}
)


##### artsd #####################################

set( target artsd )

set( ${target}_SRCS
  soundserverv2_impl.cpp soundserver_impl.cpp simplesoundserver_impl.cpp
  artsd.cpp cpuusage.cpp samplestorage_impl.cpp crashhandler.cpp
  soundserverstartup_impl.cpp
)

tde_add_executable( ${target}
  SOURCES ${${target}_SRCS}
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### artsplay ##################################

tde_add_executable( artsplay
  SOURCES artsplay.cpp
  LINK soundserver_idl-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### artscat ###################################

tde_add_executable( artscat
  SOURCES artscat.cpp
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### artswrapper ###############################

tde_add_executable( artswrapper
  SOURCES artswrapper.c
  DESTINATION ${BIN_INSTALL_DIR}
)

# FIXME "artsdpath" seems inconsistent name
# necessary for artswrapper
set( artsdpath  ${BIN_INSTALL_DIR}/artsd )
set_property( TARGET artswrapper APPEND PROPERTY COMPILE_DEFINITIONS EXECUTE="${artsdpath}" )


##### artsshell #################################

tde_add_executable( artsshell
  SOURCES artsshell.cpp tradercheck.cpp
  LINK soundserver_idl-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### artsrec ###################################

tde_add_executable( artsrec
  SOURCES artsrec.cpp
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${BIN_INSTALL_DIR}
)
