set(SC_AGENTS_COMMON_SRC "${CMAKE_CURRENT_SOURCE_DIR}")

file(GLOB SOURCES CONFIGURE_DEPENDS
    "src/*.hpp" "src/*.cpp"
    "src/*/*.hpp" "src/*/*.cpp"
    "include/sc-agents-common/*.hpp"
    "include/sc-agents-common/*/*.hpp"
)

add_library(sc-agents-common SHARED ${SOURCES})
target_link_libraries(sc-agents-common
    LINK_PUBLIC sc-memory
)
target_include_directories(sc-agents-common
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PUBLIC $<INSTALL_INTERFACE:include>
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-agents-common)
endif()

if(${SC_BUILD_TESTS})
    add_subdirectory(tests)
endif()
