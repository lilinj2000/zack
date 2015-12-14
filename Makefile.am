AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = zack
zack_SOURCES = \
	ZackConfig.cc \
	Parser.cc \
	ZackServer.cc \
	Main.cc

zack_CPPFLAGS =  \
		$(BOOST_CPPFLAGS) \
		$(SOIL_CPPFLAGS) \
		$(AIR_CPPFLAGS)


zack_LDADD = \
	$(BOOST_LIBADD) \
	$(SOIL_LIBADD) \
	$(AIR_LIBADD)


zack_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = ZackConfig.hh \
	FtdcDef.hh \
	Parser.hh \
	ZackServer.hh \
	ZackLog.hh


EXTRA_DIST = env.sh.example zack.cfg.example log.cfg.example deploy.sh

zackdir = $(bindir)
zack_DATA = $(EXTRA_DIST)
