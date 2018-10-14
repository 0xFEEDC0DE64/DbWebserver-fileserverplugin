set(HEADERS
    fileserverapplication.h
    fileserverplugin.h
)

set(SOURCES
    fileserverapplication.cpp
    fileserverplugin.cpp
)

set(RESOURCES
    fileserverplugin_resources.qrc
)

set(OTHER_FILES
    fileserverplugin.json
)

add_library(fileserverplugin SHARED ${HEADERS} ${SOURCES} ${RESOURCES} ${OTHER_FILES})

target_link_libraries(fileserverplugin Qt5::Core Qt5::Network dbcorelib webserverlib)
