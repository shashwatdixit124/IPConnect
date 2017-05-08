set(IPConnectLibrary_SRS
    interfaces/icontrolcenter.cpp
)

set(IPConnectLibrary_Interafces_HEADERS
    interfaces/icontrolcenter.h
)

add_library(IPConnect SHARED ${IPConnectLibrary_SRS})
target_link_libraries(IPConnect Qt5::Core)
install(TARGETS IPConnect LIBRARY DESTINATION lib)
install(FILES ${IPConnectLibrary_Interfaces_HEADERS} DESTINATION include/ipconnect/interfaces)
