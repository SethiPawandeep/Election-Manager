TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ElectionManager.cpp \
    User.cpp \
    Candidate.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Candidate.h \
    Constituency.h \
    VoterId.h \
    User.h \
    ElectionManager.h

