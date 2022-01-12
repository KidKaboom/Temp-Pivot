set(SOURCE_FILES
"main.cpp"
"TempPivotContextCmd.h"
"TempPivotContextCmd.cpp"
"TempPivotContext.h" 
"TempPivotContext.cpp" 
"TempPivotManip.h"
"TempPivotManip.cpp" 
"TempPivotToolCmd.h" 
"TempPivotToolCmd.cpp"
)

find_package(Maya REQUIRED)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES}
"TempPivotContextCmd.h"
"TempPivotContextCmd.cpp"
"TempPivotContext.h" 
"TempPivotContext.cpp" 
"TempPivotManip.h"
"TempPivotManip.cpp" 
"TempPivotToolCmd.h" 
"TempPivotToolCmd.cpp"
)

target_link_libraries(${PROJECT_NAME} PRIVATE Maya::Maya)
target_include_directories(${PROJECT_NAME} 
    PRIVATE Maya::Maya
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)

MAYA_PLUGIN(${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION plug-ins/${MAYA_VERSION})
