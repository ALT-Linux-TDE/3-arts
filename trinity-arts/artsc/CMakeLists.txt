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

add_definitions( -DCOMPILING_ARTSC )


##### variables for artsc-config ################

set( prefix ${CMAKE_INSTALL_PREFIX} )
set( exec_prefix $prefix )
set( libdir ${LIB_INSTALL_DIR} )
set( includedir ${CMAKE_INSTALL_PREFIX}/include/artsc )
if( NOT "${DL_LIBRARIES}" STREQUAL "" )
  set( LIBDL -l${DL_LIBRARIES} )
endif( )
set( USE_THREADS )
set( LIBPTHREAD ${CMAKE_THREAD_LIBS_INIT} )
foreach( LIB ${GTHREAD2_LIBRARIES} )
  set( GLIB_LDFLAGS "${GLIB_LDFLAGS} -l${LIB}" )
endforeach(LIB)
set( GLIB_LIBADD )
foreach( INC ${GTHREAD2_INCLUDE_DIRS} )
  set( GLIB_CFLAGS "${GLIB_CFLAGS} -I${INC}" )
endforeach(INC)

configure_file( artsc.pc.in artsc.pc @ONLY )
configure_file( artsc-config.in artsc-config @ONLY )
configure_file( artsdsp.in artsdsp @ONLY )
configure_file( artsc_export.h.in artsc_export.h @ONLY )

install( FILES ${CMAKE_CURRENT_BINARY_DIR}/artsc.pc
  DESTINATION ${PKGCONFIG_INSTALL_DIR} )

install( PROGRAMS
    ${CMAKE_CURRENT_BINARY_DIR}/artsdsp
    ${CMAKE_CURRENT_BINARY_DIR}/artsc-config
  DESTINATION ${BIN_INSTALL_DIR} )

##### include paths #############################

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/artsc
  ${CMAKE_SOURCE_DIR}/libltdl
  ${CMAKE_BINARY_DIR}/mcop
  ${CMAKE_SOURCE_DIR}/mcop
  ${CMAKE_BINARY_DIR}/flow
  ${CMAKE_SOURCE_DIR}/flow
  ${CMAKE_BINARY_DIR}/soundserver
)


##### headers ###################################

# FIXME: DESTINATION is currently a workaround
install( FILES
    artsc.h ${CMAKE_CURRENT_BINARY_DIR}/artsc_export.h
  DESTINATION ${includedir} )


##### artsdsp (shared lib) ######################

tde_add_library( artsdsp SHARED
  SOURCES artsdsp.c
  VERSION 0.0.0
  LINK artsc-shared ${DL_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)


##### artsdsp_st (shared lib) ###################

tde_add_library( artsdsp_st SHARED
  SOURCES artsc.c artsdsp.c
  VERSION 0.0.0
  LINK ltdlc-static
  DESTINATION ${LIB_INSTALL_DIR}
)

set_target_properties( artsdsp_st-shared PROPERTIES COMPILE_FLAGS -DARTSC_BACKEND=\\"${LIB_INSTALL_DIR}/libartscbackend.la\\" )


##### artsc (shared lib) ########################

tde_add_library( artsc SHARED
  SOURCES artsc.c
  VERSION 0.0.0
  LINK ltdlc-static
  DESTINATION ${LIB_INSTALL_DIR}
)

set_target_properties( artsc-shared PROPERTIES COMPILE_FLAGS -DARTSC_BACKEND=\\"${LIB_INSTALL_DIR}/libartscbackend.la\\" )


##### artscbackend (shared lib) #################

tde_add_library( artscbackend SHARED
  SOURCES artscbackend.cc
  VERSION 0.0.0
  LINK soundserver_idl-shared artsflow-shared
  DESTINATION ${LIB_INSTALL_DIR}
)
