set(IPConnectLibrary_SRS
    interfaces/iclient.cpp
    interfaces/iclientmanager.cpp
    interfaces/iconnection.cpp
    interfaces/icontrolcenter.cpp
    interfaces/idownloader.cpp
    interfaces/iserver.cpp
    interfaces/iusersettings.cpp
    
    client.cpp
    clientinformation.cpp
    clientmanager.cpp
    clientthreadmanager.cpp
    connection.cpp
    controlcenter.cpp
    controlcenter_p.cpp
    debug.cpp
    file.cpp
    messageinformation.cpp
    server.cpp
    usersettings.cpp
)

set(IPConnectLibrary_Interfaces_HEADERS
    interfaces/iclient.h
    interfaces/iclientmanager.h
    interfaces/iconnection.h
    interfaces/icontrolcenter.h
    interfaces/idownloader.h
    interfaces/iserver.h
    interfaces/iusersettings.h
)

set(IPConnectLibrary_HEADERS
    client.h
    clientinformation.h
    clientmanager.h
    clientthreadmanager.h
    connection.h
    controlcenter.h
    controlcenter_p.h
    debug.h
    file.h
    messageinformation.h
    server.h
    usersettings.h
)

add_library(IPConnect SHARED ${IPConnectLibrary_SRS})
target_link_libraries(IPConnect Qt5::Core Qt5::Network)
install(TARGETS IPConnect LIBRARY DESTINATION lib)
install(FILES ${IPConnectLibrary_Interfaces_HEADERS} DESTINATION include/ipconnect/interfaces)
install(FILES ${IPConnectLibrary_HEADERS} DESTINATION include/ipconnect)
target_include_directories(IPConnect SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/src/lib)
