add_executable(NovaCoreTests CoreTests.cpp)
target_link_libraries(NovaCoreTests PRIVATE NovaCore)
nova_set_warnings(NovaCoreTests)
add_test(NAME NovaCoreTests COMMAND NovaCoreTests)
