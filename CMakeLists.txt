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

add_library(webserver-plugins-fileserverplugin SHARED ${HEADERS} ${SOURCES} ${RESOURCES} ${OTHER_FILES})

set_target_properties(webserver-plugins-fileserverplugin PROPERTIES OUTPUT_NAME fileserverplugin PREFIX "")

target_link_libraries(webserver-plugins-fileserverplugin Qt5::Core Qt5::Network dbcorelib webserverlib)

add_dependencies(webserver-plugins webserver-plugins-fileserverplugin)
