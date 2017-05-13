macro(add_libtc_qtest foo)
    add_executable(${foo} ${foo}/test.cpp fakeit.hpp)
    target_link_libraries(${foo} libtc Qt5::Test)
endmacro()

add_libtc_qtest(test-project)
