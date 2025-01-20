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

add_subdirectory( mcopclass )
add_subdirectory( gsl )
add_subdirectory( gslpp )

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/artsc
  ${CMAKE_BINARY_DIR}/mcop
  ${CMAKE_SOURCE_DIR}/mcop
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${GLIB2_INCLUDE_DIRS}
)


##### headers ###################################

install( FILES
    audiosubsys.h cache.h cachedwav.h convert.h
    pipebuffer.h stdsynthmodule.h synthschedule.h
    fft.h artsflow.idl audioio.h resample.h cpuinfo.h
    bufferqueue.h gslschedule.h
    ${CMAKE_CURRENT_BINARY_DIR}/artsflow.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/${CMAKE_PROJECT_NAME} )


##### other data ################################

install( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/artsflow.mcoptype
    ${CMAKE_CURRENT_BINARY_DIR}/artsflow.mcopclass
  DESTINATION ${LIB_INSTALL_DIR}/mcop )


##### artsflow (shared lib) #####################

set( target artsflow )

set( ${target}_SRCS
  synth_play_impl.cpp gslschedule.cpp audiosubsys.cpp
  pipebuffer.cpp convert.cpp synth_wave_sin_impl.cpp
  synth_frequency_impl.cpp synth_multi_add_impl.cpp
  synth_add_impl.cpp synth_mul_impl.cpp synth_play_wav_impl.cpp
  stdsynthmodule.cpp cache.cpp asyncschedule.cpp bytestreamtoaudio_impl.cpp
  stereovolumecontrol_impl.cpp stereoeffectstack_impl.cpp
  fft.c stereofftscope_impl.cpp virtualports.cpp bus.cpp
  audiomanager_impl.cpp synth_record_impl.cpp resample.cpp
  audioio.cpp audioiooss.cpp audioioalsa.cpp
  audioionull.cpp audioiolibaudioio.cpp audioioesd.cpp audioiosndio.cpp
  audioiojack.cpp audioiosun.cpp audioioaix.cpp audioionas.cpp
  cpuinfo.cpp audioioossthreaded.cpp audiotobytestream_impl.cpp
  audioiosgi.cpp audioiocsl.cpp audioiomas.cpp datahandle_impl.cpp
)

tde_add_library( ${target} SHARED
  SOURCES ${${target}_SRCS}
  VERSION 1.0.0
  LINK artsgsl-static artsgslpp-static artsflow_idl-shared ${AUDIOFILE_LIBRARIES} ${LIBJACK_LIBRARIES} ${LIBSNDIO_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)

##### artsflow_idl (shared lib) #################

add_custom_command(
  OUTPUT artsflow.cpp
  COMMAND ../mcopidl/mcopidl
  ARGS -t ${CMAKE_CURRENT_SOURCE_DIR}/artsflow.idl DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/artsflow.idl
)

tde_add_library( artsflow_idl SHARED
  SOURCES artsflow.cpp
  VERSION 1.0.0
  LINK mcop-shared ${ALSA_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
  DEPENDENCIES mcopidl
)
