
noinst_LIBRARIES = libkdb-subarch.a

libkdb_subarch_a_SOURCES = x86.cc \
	${top_srcdir}/kdb/generic/linear_ptab_dump.cc

if CONFIG_KDB_DISAS
libkdb_subarch_a_SOURCES += disas.cc ia32-dis.cc sprintf.cc
endif

if CONFIG_NEW_MDB
libkdb_subarch_a_SOURCES += ${top_srcdir}/kdb/generic/mdb.cc
else
libkdb_subarch_a_SOURCES += ${top_srcdir}/kdb/generic/mapping.cc
endif
