project(ipconnect)
add_executable(ipconnect main.cpp)
target_link_libraries(ipconnect Qt5::Widgets IPConnect)
install(TARGETS ipconnect DESTINATION bin)
