make_tests_from_folder(${CMAKE_CURRENT_SOURCE_DIR}/units
    NAME sc-kpm-utils-tests
    DEPENDS sc-memory sc-kpm-utils
    INCLUDES ${SC_KPM_UTILS_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sc-kpm-utils-tests)
endif()
