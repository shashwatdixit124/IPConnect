set(IPConnectUILibrary_SRS
    ipconnectwindow.cpp
)

set(IPConnectUILibrary_HEADERS
    ipconnectwindow.h
)

add_library(IPConnectUI SHARED ${IPConnectUILibrary_SRS})
target_link_libraries(IPConnectUI Qt5::Core Qt5::Widgets Qt5::Quick)
install(TARGETS IPConnectUI LIBRARY DESTINATION lib)
install(FILES ${IPConnectUILibrary_HEADERS} DESTINATION include/ipconnect/ui)
target_include_directories(IPConnect SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src/ui)
