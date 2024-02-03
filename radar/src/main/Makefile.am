# Program name
bin_PROGRAMS = xpradar

# Include paths
AM_CPPFLAGS += -I$(top_srcdir)/lib

# Source file names
xpradar_SOURCES = \
    $(top_srcdir)/lib/check_aircraft.c \
    $(top_srcdir)/lib/dictionary.c \
    $(top_srcdir)/lib/handleserver.c \
    $(top_srcdir)/lib/iniparser.c \
    $(top_srcdir)/lib/serverdata.c \
    $(top_srcdir)/lib/wxrdata.c \
    $(top_srcdir)/lib/xplanebeacon.c \
    $(top_srcdir)/lib/xplanewxr.c \
    AppObject.cpp \
    main.cpp

noinst_HEADERS = \
    AppObject.h

# Libraries to link with
xpradar_LDADD = ../gauges/libRadarGauges.a ../datasources/libRadarDataSources.a ../base/libRadarBase.a

# Ensure the linker can find the libraries and resolve all symbols
LDFLAGS += -L$(top_srcdir)/lib -lpthread

