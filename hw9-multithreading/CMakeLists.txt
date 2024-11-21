cmake_minimum_required(VERSION 3.15)

set(CMAKE_INSTALL_PREFIX /usr/)

find_package(Git)

if(GIT_EXECUTABLE)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE PROJECT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

if(PROJECT_VERSION STREQUAL "")
  set(PROJECT_VERSION 0.0.0)
  message(WARNING "Failed to determine version from Git tags. Using default version \"${PROJECT_VERSION}\".")
endif()

message("Version is \"${PROJECT_VERSION}\".")

project(async VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 17)

add_library(async SHARED
    async.cpp
    async.h
    parser.cpp
    parser.h
    consolelogger.cpp
    consolelogger.h
    waitingqueue.h
    asyncdefs.h
    filelogger.cpp
    filelogger.h
)

set_target_properties(async PROPERTIES PUBLIC_HEADER async.h)

add_executable(async_cli main.cpp)

target_link_libraries(async_cli async)

if(LINUX)
# -- install command was introduced in 3.15 version of cmake
# -- we can use sudo make install from build folder
    install(TARGETS async_cli DESTINATION bin)
    install(TARGETS async LIBRARY DESTINATION lib
            PUBLIC_HEADER DESTINATION include)
    set(CPACK_GENERATOR DEB)
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    set(CPACK_PACKAGE_CONTACT dmitriirylko@gmail.com)
    include(CPack)
endif()