# Program name
bin_PROGRAMS = xpradar

# Source file names
xpradar_SOURCES = \
AppObject.cpp \
main.cpp \
../libxpcockpit/check_aircraft.c \
../libxpcockpit/dictionary.c \
../libxpcockpit/handleserver.c \
../libxpcockpit/iniparser.c \
../libxpcockpit/serverdata.c \
../libxpcockpit/wxrdata.c \
../libxpcockpit/xplanebeacon.c \
../libxpcockpit/xplanewxr.c

noinst_HEADERS = \
AppObject.h

xpradar_LDADD = ../gauges/libRadarGauges.a ../datasources/libRadarDataSources.a ../base/libRadarBase.a
