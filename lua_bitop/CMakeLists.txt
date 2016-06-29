PROJECT(bitop)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} /usr/local/luajit/include/luajit-2.1)
#LINK_DIRECTORIES(/usr/local/luajit/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_C_FLAGS "-std=c99 -O2 -Wall")
SET(SRC bitop.c)
ADD_LIBRARY(bitop SHARED ${SRC})
SET_TARGET_PROPERTIES(bitop PROPERTIES PREFIX "" SUFFIX ".so")
#TARGET_LINK_LIBRARIES(bitop luajit-5.1) #Never use -llua when building Lua modules
INSTALL(TARGETS bitop
        LIBRARY DESTINATION /usr/local/lualib)
