set(IPConnectLibrary_SRS
    interfaces/icontrolcenter.cpp
    interfaces/iserver.cpp
    controlcenter.cpp
    controlcenter_p.cpp
    debug.cpp
    server.cpp
)

set(IPConnectLibrary_Interfaces_HEADERS
    interfaces/icontrolcenter.h
    interfaces/iserver.h
)

set(IPConnectLibrary_HEADERS
    controlcenter.h
    controlcenter_p.h
    debug.h
    server.h
)

add_library(IPConnect SHARED ${IPConnectLibrary_SRS})
target_link_libraries(IPConnect Qt5::Core Qt5::Network)
install(TARGETS IPConnect LIBRARY DESTINATION lib)
install(FILES ${IPConnectLibrary_Interfaces_HEADERS} DESTINATION include/ipconnect/interfaces)
install(FILES ${IPConnectLibrary_HEADERS} DESTINATION include/ipconnect)
target_include_directories(IPConnect SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src/lib)
