project(ipconnect)
add_executable(ipconnect main.cpp)
target_link_libraries(ipconnect Qt5::Widgets IPConnect IPConnectUI)
install(TARGETS ipconnect DESTINATION bin)
