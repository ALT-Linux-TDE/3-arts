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
  DESTINATION ${INCLUDE_INSTALL_DIR} )


##### other data ################################

install( FILES
    ${CMAKE_CURRENT_BINARY_DIR}/artsflow.mcoptype
    ${CMAKE_CURRENT_BINARY_DIR}/artsflow.mcopclass
  DESTINATION ${LIB_INSTALL_DIR}/mcop )


##### artsflow (shared lib) #####################

set( target artsflow )

set( ${target}_SRCS
  synth_play_impl.cc gslschedule.cc audiosubsys.cc
  pipebuffer.cc convert.cc synth_wave_sin_impl.cc
  synth_frequency_impl.cc synth_multi_add_impl.cc
  synth_add_impl.cc synth_mul_impl.cc synth_play_wav_impl.cc
  stdsynthmodule.cc cache.cc asyncschedule.cc bytestreamtoaudio_impl.cc
  stereovolumecontrol_impl.cc stereoeffectstack_impl.cc
  fft.c stereofftscope_impl.cc virtualports.cc bus.cc
  audiomanager_impl.cc synth_record_impl.cc resample.cc
  audioio.cc audioiooss.cc audioioalsa.cc audioioalsa9.cc
  audioionull.cc audioiolibaudioio.cc audioioesd.cc audioiosndio.cc
  audioiojack.cc audioiosun.cc audioioaix.cc audioionas.cc
  cpuinfo.cc audioioossthreaded.cc audiotobytestream_impl.cc
  audioiosgi.cc audioiocsl.cc audioiomas.cc datahandle_impl.cc
)

tde_add_library( ${target} SHARED
  SOURCES ${${target}_SRCS}
  VERSION 1.0.0
  LINK artsgsl-static artsgslpp-static artsflow_idl-shared ${AUDIOFILE_LIBRARIES} ${LIBJACK_LIBRARIES} ${LIBSNDIO_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
)

##### artsflow_idl (shared lib) #################

add_custom_command(
  OUTPUT artsflow.cc
  COMMAND ../mcopidl/mcopidl
  ARGS -t ${CMAKE_CURRENT_SOURCE_DIR}/artsflow.idl DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/artsflow.idl
)

tde_add_library( artsflow_idl SHARED
  SOURCES artsflow.cc
  VERSION 1.0.0
  LINK mcop-shared ${ALSA_LIBRARIES}
  DESTINATION ${LIB_INSTALL_DIR}
  DEPENDENCIES mcopidl
)
