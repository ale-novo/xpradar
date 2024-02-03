# Program name
bin_PROGRAMS = xpradar

# Include paths
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

# Libraries to link with
xpradar_LDADD = ../gauges/libRadarGauges.a ../datasources/libRadarDataSources.a ../base/libRadarBase.a

# Ensure the linker can find the libraries and resolve all symbols
LDFLAGS += -L$(top_srcdir)/libxpcockpit -lpthread

