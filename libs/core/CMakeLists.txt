SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)

FOREACH(subdir ${SUBDIRS})
  set(LibName ${subdir}Core)

  FILE(GLOB SRC_FILES src/${subdir}/*.cpp)
  FILE(GLOB HDR_FILES include/core/${subdir}/*.h)
  
  if(SRC_FILES)
    ADD_LIBRARY(${LibName} STATIC)  
    ADD_LIBRARY(core::${subdir} ALIAS ${LibName})

    TARGET_SOURCES(${LibName} PRIVATE ${SRC_FILES})
    TARGET_SOURCES(${LibName} PUBLIC FILE_SET HEADERS FILES ${HDR_FILES})

    TARGET_INCLUDE_DIRECTORIES(${LibName} PUBLIC include)
  endif()
ENDFOREACH(subdir ${SUBDIRS})


TARGET_LINK_LIBRARIES(MainCore PUBLIC
  GeneralCore
)