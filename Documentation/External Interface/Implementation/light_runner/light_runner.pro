#                                         #
#   ERTMS/ETCS "light" TESTRUNNER         #
#   (C) Copyright ERSA                    #
#   All Rights Reserved.                  #
#                                         #
#       qmake configuration file          #
#                                         #
###########################################

# Suffix definition
CONFIG(debug, debug|release) {
    DEFINES -= NDEBUG
    DEFINES *= DEBUG
    DEFINES *= _DEBUG
    DEFINES *= __DEBUG__

    SUFFIX_STR = d
}

CONFIG(release, debug|release) {
    DEFINES *= NDEBUG
    DEFINES -= DEBUG
    DEFINES -= _DEBUG
    DEFINES -= __DEBUG__
}

# Intermediate output dir
OBJECTS_DIR         =   .out$${SUFFIX_STR}
MOC_DIR             =   $${OBJECTS_DIR}/moc
RCC_DIR             =   $${OBJECTS_DIR}/rcc
UI_DIR              =   $${OBJECTS_DIR}/ui

TARGET              =   light_runner$${SUFFIX_STR}

# Project configuration
TEMPLATE            =   app
DESTDIR             =   bin

CONFIG              *=  qt thread
QT                  *=  core gui sql

greaterThan(QT_MAJOR_VERSION, 4) {
    QT              *=  widgets testlib
} else {
    CONFIG          *=  qtestlib
}

INCLUDEPATH         *=  include                                                 \
                        ../evc/evc_com/include                                  \
                        ../evc/eurocab/include

HEADERS             =   include/light_runner.h


SOURCES             =   src/light_runner.cpp                                    

FORMS               =   ui/light_runner.ui                                        


LIBS                *=  -L../lib -levc_com$${SUFFIX_STR}


# rpath should point to the shared lib directory (relative to the binary)
QMAKE_LFLAGS    *=  -Wl,-rpath,../lib                                       \
                    -Wl,-rpath,\'\$$ORIGIN/../../lib\'

PRE_TARGETDEPS  *=  ../lib/libevc_com$${SUFFIX_STR}.so

