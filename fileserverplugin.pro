QT += core network

DBLIBS += webserverlib

HEADERS += fileserverplugin.h \
    fileserverapplication.h

SOURCES += fileserverplugin.cpp \
    fileserverapplication.cpp

FORMS +=

RESOURCES +=

TRANSLATIONS +=

OTHER_FILES += fileserverplugin.json

include(../plugin.pri)
