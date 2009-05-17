DIST_SUBDIRS = x32

SUBDIRS = @SUBARCH@

noinst_LIBRARIES = libkdb-glue.a

libkdb_glue_a_SOURCES = thread.cc prepost.cc readmem.cc resources.cc

if CONFIG_TRACEBUFFER
libkdb_glue_a_SOURCES += tracebuffer.cc
endif
