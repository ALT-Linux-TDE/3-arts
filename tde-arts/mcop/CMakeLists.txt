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

configure_file( arts_export.h.in arts_export.h )

if( NOT EXTENSION_DIR )
  set( EXTENSION_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX} )
endif( NOT EXTENSION_DIR )

if( NOT DTRADER_DIR )
  set( DTRADER_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/mcop )
endif( NOT DTRADER_DIR )

add_definitions(
  -DEXTENSION_DIR=\"${EXTENSION_DIR}\"
  -DTRADER_DIR=\"${DTRADER_DIR}\"
)

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/libltdl
)


##### headers ###################################

install( FILES
    buffer.h common.h connection.h core.h dispatcher.h
    factory.h flowsystem.h idlfilereg.h ifacerepo_impl.h
    iomanager.h mcoputils.h object.h objectmanager.h
    pool.h socketconnection.h startupmanager.h tcpconnection.h
    tcpserver.h type.h unixconnection.h unixserver.h
    referenceclean.h datapacket.h asyncstream.h notification.h
    extensionloader.h mcopconfig.h connect.h reference.h
    weakreference.h dynamicrequest.h anyref.h debug.h
    delayedreturn.h dynamicskeleton.h thread.h core.idl
    ${CMAKE_CURRENT_BINARY_DIR}/arts_export.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/${CMAKE_PROJECT_NAME} )


##### mcop (shared library) #####################

set( target mcop )

set( ${target}_SRCS
  buffer.cpp connection.cpp core.cpp debug.cpp dispatcher.cpp
  iomanager.cpp object.cpp socketconnection.cpp tcpconnection.cpp
  unixconnection.cpp tcpserver.cpp unixserver.cpp objectmanager.cpp
  factory.cpp idlfilereg.cpp ifacerepo_impl.cpp mcoputils.cpp
  startupmanager.cpp md5.c md5auth.cpp referenceclean.cpp
  datapacket.cpp asyncstream.cpp notification.cpp
  flowsystem.cpp extensionloader.cpp tmpglobalcomm.cpp
  mcopconfig.cpp connect.cpp reference.cpp type.cpp
  trader_impl.cpp dynamicrequest.cpp anyref.cpp
  loopback.cpp delayedreturn.cpp thread.cpp dynamicskeleton.cpp
)

tde_add_library( ${target} SHARED
  SOURCES ${${target}_SRCS}
  VERSION 1.0.0
  LINK ltdlc-static ${ESOUND_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)
