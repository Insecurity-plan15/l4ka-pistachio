
noinst_PROGRAMS = kinit.o

kinit_o_LDFLAGS = -Wl,-r

kinit_o_SOURCES = \
	init.cc exception.cc space.cc user.cc trap.S trampoline.S \
	memcontrol.cc \
	${top_srcdir}/src/generic/linear_ptab_walker.cc \
	${top_srcdir}/src/generic/mapping_alloc.cc \
	${top_srcdir}/src/generic/mapping.cc
