#                                                                  #
#             +++ ERTMS/ETCS "light" TESTRUNNER +++                #
#                                                                  #
# Copyright © 2014 - European Rail Software Applications (ERSA)    #
#                    5 rue Maurice Blin                            #
#                    67500 HAGUENAU                                #
#                    FRANCE                                        #
#                    http://www.ersa-france.com                    #
#                                                                  #
# Author(s): Alexis JULIN (ERSA), Didier WECKMANN (ERSA)           #
#                                                                  #
# Licensed under the EUPL Version 1.1.                             #
#                                                                  #
# You may not use this work except in compliance with the License. #
# You may obtain a copy of the License at:                         #
# http://ec.europa.eu/idabc/eupl.html                              #
#                                                                  #
# Unless required by applicable law or agreed to in writing,       #
# software distributed under the License is distributed on an      #
# "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,     #
# either express or implied. See the License for the specific      #
# language governing permissions and limitations under the License.#
#                                                                  #
#       qmake configuration file                                   #
#                                                                  #
####################################################################

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

