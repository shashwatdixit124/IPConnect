set(IPConnectUILibrary_SRS
    ipconnectwindow.cpp
)

set(IPConnectUILibrary_HEADERS
    ipconnectwindow.h
)

set(IPConnectUILibrary_RCC
    window.qrc
)

add_library(IPConnectUI SHARED ${IPConnectUILibrary_SRS} ${IPConnectUILibrary_RCC})
target_link_libraries(IPConnectUI Qt5::Core Qt5::Widgets Qt5::Quick)
install(TARGETS IPConnectUI LIBRARY DESTINATION lib)
install(FILES ${IPConnectUILibrary_HEADERS} DESTINATION include/ipconnect/ui)
target_include_directories(IPConnectUI SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src/ui)
