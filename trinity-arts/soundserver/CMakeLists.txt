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
  DESTINATION ${INCLUDE_INSTALL_DIR} )


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
  OUTPUT kmedia2.cc
  COMMAND ../mcopidl/mcopidl ARGS -t ${MCOPINC} ${CMAKE_CURRENT_SOURCE_DIR}/kmedia2.idl
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kmedia2.idl
)

tde_add_library( kmedia2_idl SHARED
  SOURCES kmedia2.cc
  VERSION 1.0.0
  LINK artsflow_idl-shared
  DESTINATION ${LIB_INSTALL_DIR}
  DEPENDENCIES mcopidl
)


##### soundserver_idl (shared lib) ##############

add_custom_command(
  OUTPUT soundserver.cc soundserver.h
  COMMAND ../mcopidl/mcopidl ARGS -t ${MCOPINC} ${CMAKE_CURRENT_SOURCE_DIR}/soundserver.idl
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/soundserver.idl
)

tde_add_library( soundserver_idl SHARED
  SOURCES soundserver.cc
  VERSION 1.0.0
  LINK kmedia2_idl-shared
  DESTINATION ${LIB_INSTALL_DIR}
  DEPENDENCIES mcopidl
)


##### artsgslplayobject (shared lib) ############

tde_add_library( artsgslplayobject SHARED
  SOURCES gslplayobject_impl.cc
  VERSION 0.0.0
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${LIB_INSTALL_DIR}
)


##### artswavplayobject (shared lib) ############

tde_add_library( artswavplayobject SHARED
  SOURCES wavplayobject_impl.cc
  VERSION 0.0.0
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${LIB_INSTALL_DIR}
)


##### kmedia2 (shared lib) ######################

tde_add_library( kmedia2 SHARED
  SOURCES fileinputstream_impl.cc stdoutwriter_impl.cc
  VERSION 1.0.0
  LINK kmedia2_idl-shared artsflow-shared
  DESTINATION ${LIB_INSTALL_DIR}
)


##### artsd #####################################

set( target artsd )

set( ${target}_SRCS
  soundserverv2_impl.cc soundserver_impl.cc simplesoundserver_impl.cc
  artsd.cc cpuusage.cc samplestorage_impl.cc crashhandler.cc
  soundserverstartup_impl.cc
)

tde_add_executable( ${target}
  SOURCES ${${target}_SRCS}
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### artsplay ##################################

tde_add_executable( artsplay
  SOURCES artsplay.cc
  LINK soundserver_idl-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### artscat ###################################

tde_add_executable( artscat
  SOURCES artscat.cc
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
SET( artsdpath  ${BIN_INSTALL_DIR}/artsd )
set_target_properties( artswrapper PROPERTIES COMPILE_FLAGS -DEXECUTE=\\"${artsdpath}\\" )


##### artsshell #################################

tde_add_executable( artsshell
  SOURCES artsshell.cc tradercheck.cc
  LINK soundserver_idl-shared
  DESTINATION ${BIN_INSTALL_DIR}
)


##### artsrec ###################################

tde_add_executable( artsrec
  SOURCES artsrec.cc
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${BIN_INSTALL_DIR}
)
