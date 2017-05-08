set(IPConnectLibrary_SRS
    interfaces/icontrolcenter.cpp
    controlcenter.cpp
    interfaces/iserver.cpp
    server.cpp
)

set(IPConnectLibrary_Interafces_HEADERS
    interfaces/icontrolcenter.h
    interfaces/iserver.h
)

set(IPConnectLibrary_HEADERS
    controlcenter.h
    server.h
)

add_library(IPConnect SHARED ${IPConnectLibrary_SRS})
target_link_libraries(IPConnect Qt5::Core Qt5::Network)
install(TARGETS IPConnect LIBRARY DESTINATION lib)
install(FILES ${IPConnectLibrary_Interfaces_HEADERS} DESTINATION include/ipconnect/interfaces)
install(FILES ${IPConnectLibrary_HEADERS} DESTINATION include/ipconnect)
