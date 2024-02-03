# Library name
noinst_LIBRARIES = libRadarGauges.a
AM_CPPFLAGS += -I$(top_srcdir)/libxpcockpit

# Source file names
libRadarGauges_a_SOURCES = \
WXR/WXR.cpp \
WXR/DrawWXR.cpp \
WXR/DrawWXRInfo.cpp \
Dummy/DummyGauge.cpp \
$(top_srcdir)/libxpcockpit/check_aircraft.c \
$(top_srcdir)/libxpcockpit/dictionary.c \
$(top_srcdir)/libxpcockpit/handleserver.c \
$(top_srcdir)/libxpcockpit/iniparser.c \
$(top_srcdir)/libxpcockpit/serverdata.c \
$(top_srcdir)/libxpcockpit/wxrdata.c \
$(top_srcdir)/libxpcockpit/xplanebeacon.c \
$(top_srcdir)/libxpcockpit/xplanewxr.c

noinst_HEADERS = \
WXR/WXR.h \
WXR/DrawWXR.h \
WXR/DrawWXRInfo.h \
Dummy/DummyGauge.h
