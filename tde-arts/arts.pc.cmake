prefix=@CMAKE_INSTALL_PREFIX@
exec_prefix=@PC_EXEC_PREFIX@
libdir=@PC_LIB_DIR@
includedir=@PC_INCLUDE_DIR@

Name: aRts
Description: Soundserver for the Trinity Desktop Environment (TDE)
Version: @ARTS_VERSION@
Libs: -L${libdir}
Cflags: -I${includedir}
@PC_LIB_REQUIRES@
