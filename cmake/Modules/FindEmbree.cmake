
find_path(Embree_INCLUDE_DIRS embree3/rtcore.h
  /usr/include
  /usr/local/include
  /opt/local/include
  C:/Programs/embree3/include
  D:/Programs/embree3/include)

find_library(Embree_LIBRARY NAMES embree3 PATHS 
  /usr/lib 
  /usr/local/lib 
  /opt/local/lib
  C:/Programs/embree3/lib
  D:/Programs/embree3/lib)

find_library(Embree_LIBRARY_MIC NAMES embree_xeonphi PATHS 
  /usr/lib 
  /usr/local/lib 
  /opt/local/lib
  C:/Programs/embree3/lib
  D:/Programs/embree3/lib)

set(Embree_FOUND false)
if (Embree_INCLUDE_DIRS AND Embree_LIBRARY)
  set(Embree_FOUND true)
endif ()
