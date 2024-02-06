# Library name
noinst_LIBRARIES = libRadarGauges.a
AM_CPPFLAGS += -I$(top_srcdir)/lib

# Source file names
libRadarGauges_a_SOURCES = \
$(top_srcdir)/lib/check_aircraft.c \
$(top_srcdir)/lib/dictionary.c \
$(top_srcdir)/lib/handleserver.c \
$(top_srcdir)/lib/iniparser.c \
$(top_srcdir)/lib/serverdata.c \
$(top_srcdir)/lib/wxrdata.c \
$(top_srcdir)/lib/xplanebeacon.c \
$(top_srcdir)/lib/xplanewxr.c \
WXR/WXR.cpp \
WXR/DrawWXRL.cpp \
WXR/DrawWXRR.cpp \
WXR/DrawWXRInfo.cpp \
Dummy/DummyGauge.cpp

noinst_HEADERS = \
WXR/WXR.h \
WXR/DrawWXRL.h \
WXR/DrawWXRR.h \
WXR/DrawWXRInfo.h \
Dummy/DummyGauge.h
