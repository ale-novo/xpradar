# Program name
bin_PROGRAMS = xpradar
AM_CPPFLAGS += -I$(top_srcdir)/libxpcockpit

# Source file names
xpradar_SOURCES = \
$(top_srcdir)/libxpcockpit/check_aircraft.c \
$(top_srcdir)/libxpcockpit/dictionary.c \
$(top_srcdir)/libxpcockpit/handleserver.c \
$(top_srcdir)/libxpcockpit/iniparser.c \
$(top_srcdir)/libxpcockpit/serverdata.c \
$(top_srcdir)/libxpcockpit/wxrdata.c \
$(top_srcdir)/libxpcockpit/xplanebeacon.c \
$(top_srcdir)/libxpcockpit/xplanewxr.c \
AppObject.cpp \
main.cpp

noinst_HEADERS = \
AppObject.h

xpradar_LDADD = ../gauges/libRadarGauges.a ../datasources/libRadarDataSources.a ../base/libRadarBase.a
