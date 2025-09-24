SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR}/src)

FOREACH(subdir ${SUBDIRS})
  set(AppName ${subdir})

  FILE(GLOB SRC_FILES src/${subdir}/*.cpp)
  
  if(SRC_FILES)
    add_executable(${AppName})  

    TARGET_SOURCES(${AppName} PRIVATE ${SRC_FILES})
  endif()
ENDFOREACH(subdir ${SUBDIRS})

TARGET_LINK_LIBRARIES(Main PRIVATE 
  core::Main
)

TARGET_LINK_LIBRARIES(Creator PRIVATE 
  core::Creator
)

TARGET_LINK_LIBRARIES(Reporter PRIVATE 
  core::Reporter
)