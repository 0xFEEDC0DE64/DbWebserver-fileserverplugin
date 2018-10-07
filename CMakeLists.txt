set(HEADERS
    fileserverapplication.h
    fileserverplugin.h
)

set(SOURCES
    fileserverapplication.cpp
    fileserverplugin.cpp
)

add_library(fileserverplugin SHARED ${HEADERS} ${SOURCES})

target_link_libraries(fileserverplugin Qt5::Core Qt5::Network dbcorelib webserverlib)
