
FIND_PATH(EMBREE_INCLUDE_DIRS embree3/rtcore.h
  /usr/include
  /usr/local/include
  /opt/local/include
  C:/Programs/embree3/include)

FIND_LIBRARY(EMBREE_LIBRARY NAMES embree3 PATHS 
  /usr/lib 
  /usr/local/lib 
  /opt/local/lib
  C:/Programs/embree3/lib)

FIND_LIBRARY(EMBREE_LIBRARY_MIC NAMES embree_xeonphi PATHS 
  /usr/lib 
  /usr/local/lib 
  /opt/local/lib
  C:/Programs/embree3/lib)


IF (EMBREE_INCLUDE_DIRS AND EMBREE_LIBRARY)
  SET(EMBREE_FOUND TRUE)
ENDIF ()