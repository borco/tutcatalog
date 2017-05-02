add_subdirectory(3rd-party)
add_subdirectory(libtc)
add_subdirectory(tut-catalog)

if (${WITH_TESTS})
    add_subdirectory(tests)
endif()
