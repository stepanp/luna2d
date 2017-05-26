if(WIN32 AND NOT CYGWIN)
  set(TWO_FILE_COMMANDLINE true)
endif()

configure_file(${LIBJPEG_PATH}/jconfig.h.cmake ${LIBJPEG_PATH}/jconfig.h)

include_directories(${LIBJPEG_PATH})

set(LIBJPEG_SOURCES 
  ${LIBJPEG_PATH}/jmemnobs.c
  ${LIBJPEG_PATH}/jaricom.c
  ${LIBJPEG_PATH}/jcapimin.c
  ${LIBJPEG_PATH}/jcapistd.c
  ${LIBJPEG_PATH}/jcarith.c
  ${LIBJPEG_PATH}/jccoefct.c
  ${LIBJPEG_PATH}/jccolor.c 
  ${LIBJPEG_PATH}/jcdctmgr.c
  ${LIBJPEG_PATH}/jchuff.c
  ${LIBJPEG_PATH}/jcinit.c
  ${LIBJPEG_PATH}/jcmainct.c
  ${LIBJPEG_PATH}/jcmarker.c
  ${LIBJPEG_PATH}/jcmaster.c
  ${LIBJPEG_PATH}/jcomapi.c
  ${LIBJPEG_PATH}/jcparam.c
  ${LIBJPEG_PATH}/jcprepct.c
  ${LIBJPEG_PATH}/jcsample.c
  ${LIBJPEG_PATH}/jctrans.c
  ${LIBJPEG_PATH}/jdapimin.c
  ${LIBJPEG_PATH}/jdapistd.c
  ${LIBJPEG_PATH}/jdarith.c
  ${LIBJPEG_PATH}/jdatadst.c
  ${LIBJPEG_PATH}/jdatasrc.c
  ${LIBJPEG_PATH}/jdcoefct.c
  ${LIBJPEG_PATH}/jdcolor.c
  ${LIBJPEG_PATH}/jddctmgr.c
  ${LIBJPEG_PATH}/jdhuff.c
  ${LIBJPEG_PATH}/jdinput.c
  ${LIBJPEG_PATH}/jdmainct.c
  ${LIBJPEG_PATH}/jdmarker.c
  ${LIBJPEG_PATH}/jdmaster.c
  ${LIBJPEG_PATH}/jdmerge.c
  ${LIBJPEG_PATH}/jdpostct.c
  ${LIBJPEG_PATH}/jdsample.c
  ${LIBJPEG_PATH}/jdtrans.c
  ${LIBJPEG_PATH}/jerror.c
  ${LIBJPEG_PATH}/jfdctflt.c
  ${LIBJPEG_PATH}/jfdctfst.c
  ${LIBJPEG_PATH}/jfdctint.c
  ${LIBJPEG_PATH}/jidctflt.c
  ${LIBJPEG_PATH}/jidctfst.c
  ${LIBJPEG_PATH}/jidctint.c
  ${LIBJPEG_PATH}/jquant1.c
  ${LIBJPEG_PATH}/jquant2.c
  ${LIBJPEG_PATH}/jutils.c
  ${LIBJPEG_PATH}/jmemmgr.c
  ${LIBJPEG_PATH}/cderror.h 
  ${LIBJPEG_PATH}/cdjpeg.h 
  ${LIBJPEG_PATH}/jdct.h 
  ${LIBJPEG_PATH}/jinclude.h
  ${LIBJPEG_PATH}/jmemsys.h
  ${LIBJPEG_PATH}/jpegint.h
  ${LIBJPEG_PATH}/jversion.h
  ${LIBJPEG_PATH}/transupp.h
  ${LIBJPEG_PATH}/jerror.h
  ${LIBJPEG_PATH}/jmorecfg.h
  ${LIBJPEG_PATH}/jpeglib.h
  ${LIBJPEG_PATH}/jconfig.h
)
