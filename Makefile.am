AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = zack
zack_SOURCES = \
	ZackConfig.cc \
	Parser.cc \
	MData.cc \
	ZackServer.cc \
	Main.cc

zack_CPPFLAGS =  \
		$(BOOST_CPPFLAGS) \
		$(SOIL_CPPFLAGS)


zack_LDADD = \
	$(BOOST_LIBADD) \
	$(SOIL_LIBADD)


zack_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = ZackConfig.hh \
	FtdcDef.hh \
	Parser.hh \
	ZackServer.hh \
	ZackLog.hh \
	MData.hh

EXTRA_DIST = env.sh.example zack.cfg.example log.cfg.example

zackdir = $(bindir)
zack_DATA = $(EXTRA_DIST)
