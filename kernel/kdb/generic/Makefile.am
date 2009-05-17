
noinst_LIBRARIES = libkdb-generic.a

libkdb_generic_a_SOURCES = \
	bootinfo.cc cmd.cc console.cc entry.cc init.cc input.cc kmemory.cc \
	linker_set.cc memdump.cc print.cc tid_format.cc tracepoints.cc

if CONFIG_X86_IO_FLEXPAGES
libkdb_generic_a_SOURCES += vrt.cc
endif

#if CONFIG_ACPI
#libkdb_generic_a_SOURCES += acpi.cc
#endif
