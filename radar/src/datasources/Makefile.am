noinst_LIBRARIES = libRadarDataSources.a
AM_CPPFLAGS += -I$(top_srcdir)/libxpcockpit/src

libRadarDataSources_a_SOURCES = \
DataSource.cpp \
XPlaneDataSource.cpp \
$(top_srcdir)/libxpcockpit/check_aircraft.c \
$(top_srcdir)/libxpcockpit/dictionary.c \
$(top_srcdir)/libxpcockpit/handleserver.c \
$(top_srcdir)/libxpcockpit/iniparser.c \
$(top_srcdir)/libxpcockpit/serverdata.c \
$(top_srcdir)/libxpcockpit/wxrdata.c \
$(top_srcdir)/libxpcockpit/xplanebeacon.c \
$(top_srcdir)/libxpcockpit/xplanewxr.c

noinst_HEADERS = \
DataSource.h \
XPlaneDataSource.h
