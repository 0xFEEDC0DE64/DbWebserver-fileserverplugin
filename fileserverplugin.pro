QT += core network

DBLIBS += dbcore webserverlib

HEADERS += fileserverplugin.h \
    fileserverapplication.h

SOURCES += fileserverplugin.cpp \
    fileserverapplication.cpp

FORMS +=

RESOURCES += fileserverplugin_resources.qrc

TRANSLATIONS +=

OTHER_FILES += fileserverplugin.json

include(../plugin.pri)
