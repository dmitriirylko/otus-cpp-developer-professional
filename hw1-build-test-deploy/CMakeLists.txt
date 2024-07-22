cmake_minimum_required(VERSION 3.15)

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

project(helloworld VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

configure_file(version.h.in version.h)

add_executable(helloworld_cli main.cpp lib.cpp)

add_library(helloworld lib.cpp)

set_target_properties(helloworld_cli PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

include_directories(${CMAKE_BINARY_DIR}) 

target_link_libraries(helloworld_cli PRIVATE helloworld)

if(WITH_BOOST_TEST)
    if(WIN32)
        set(Boost_ROOT "C:/boost_1_85_0/")
        set(Boost_USE_STATIC_LIBS        ON)
        set(Boost_USE_MULTITHREADED      ON)
        set(Boost_USE_STATIC_RUNTIME    OFF)
    endif()
    
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)

    add_executable(test_version test_version.cpp)

    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
    )

    include_directories(${Boost_INCLUDE_DIRS}) 

    target_link_libraries(test_version ${Boost_LIBRARIES} helloworld)
endif()

if(LINUX)
# -- install command was introduced in 3.15 version of cmake
# -- we can use sudo make install from build folder
    install(TARGETS helloworld_cli RUNTIME DESTINATION bin)
    set(CPACK_GENERATOR DEB)
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
    set(CPACK_PACKAGE_CONTACT dmitriirylko@gmail.com)
    include(CPack)
endif()

if(WITH_BOOST_TEST)
    enable_testing()
    add_test(test_version test_version)
endif()