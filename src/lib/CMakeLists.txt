set(IPConnectLibrary_SRS
    interfaces/iclient.cpp
    interfaces/iclientmanager.cpp
    interfaces/iconnection.cpp
    interfaces/icontrolcenter.cpp
    interfaces/iserver.cpp
    
    clientmanager.cpp
    controlcenter.cpp
    controlcenter_p.cpp
    debug.cpp
    server.cpp
)

set(IPConnectLibrary_Interfaces_HEADERS
    interfaces/iclient.h
    interfaces/iclientmanager.h
    interfaces/iconnection.h
    interfaces/icontrolcenter.h
    interfaces/iserver.h
)

set(IPConnectLibrary_HEADERS
    clientmanager.h
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
