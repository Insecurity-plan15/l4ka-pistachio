DIST_SUBDIRS = x32

SUBDIRS = . @SUBARCH@

BUILT_SOURCES = ${top_builddir}/asmsyms.h

noinst_LIBRARIES = libglue.a libasmsyms.a

libglue_a_SOURCES = \
	ctors.cc exception.cc space.cc init.cc resources.cc idt.cc \
	debug.cc cpu.cc thread.cc

if CONFIG_IOAPIC
libglue_a_SOURCES += timer-apic.cc
else
libglue_a_SOURCES += timer.cc
endif

if CONFIG_X86_IO_FLEXPAGES
libglue_a_SOURCES += mdb_io.cc vrt_io.cc io_space.cc
endif

libasmsyms_a_SOURCES = asmsyms.cc

${top_builddir}/asmsyms.h: asmsyms.${OBJEXT}
	NM='${NM}' PERL='${PERL}' ${SHELL} ${top_srcdir}/genasmsyms.sh ${top_builddir}/asmsyms.h ${srcdir}/asmsyms.${OBJEXT}
