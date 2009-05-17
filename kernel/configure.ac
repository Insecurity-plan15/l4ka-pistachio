AC_INIT([Pistachio],[0.4.0])
JL_CONFIG_INIT
JL_DELTA_BUNDLES([system])
AC_CANONICAL_BUILD

if test x"${host_alias}" = x"" ; then
	host_alias="${build_cpu}-elf"
	cross_compiling=yes
	ac_tool_prefix="${host_alias}-"
fi
AC_CANONICAL_HOST
JL_CHECK_BUNDLES
JL_CHECK_INPLACE([System.app/Sources/kernel],[../..])
JL_BUNDLE_PATHS([system],[${librarydir}/CoreServices/System.app],[System.app],[.])
AC_ARG_WITH([cpu],[AS_HELP_STRING([--enable-cpu=CPU],[override CPU type selection (one of i486, i586, i686, p4, k8, c3, simics, power3, power3p, power4, power4p, ppc970, ibm750, ppc604)])],[host_cpu=$withval])

AC_MSG_CHECKING([host CPU type])
case "${host_cpu}" in
	i486)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_X86_I486],[1],[Target i486 CPUs])
		default_arch="x86"
		default_subarch="x32"
		CFLAGS="$CFLAGS -march=i486"
		;;
	i586)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_X86_I586],[1],[Target i586 (Pentium) CPUs])
		default_arch="x86"
		default_subarch="x32"
		CFLAGS="$CFLAGS -march=i586"
		;;
	i686)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_X86_I686],[1],[Target i686 (Pentium II/Pentium III) CPUs])
		default_arch="x86"
		default_subarch="x32"
		;;
	p4|x86|ix86)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_X86_P4],[1],[Target P4/Core/Core 2 CPUs])
		host_cpu="p4"
		default_arch="x86"
		default_subarch="x32"
		;;
	k8)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_X86_K8],[1],[Target AMD K8 CPUs])
		default_arch="x86"
		default_subarch="x32"
		;;
	c3)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_X86_C3],[1],[Target VIA C3 CPUs])
		default_arch="x86"
		default_subarch="x32"
		;;
	simics)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_X86_SIMICS],[1],[Target a Virtutech Simics simulation platform])
		default_arch="x86"
		default_subarch="x32"
		;;
	ibm750)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_POWERPC_IBM750],[1],[Target an IBM 750 PowerPC CPU])
		default_arch="ppc"
		;;
	ppc604|ppc)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_POWERPC_PPC604],[1],[Target a PowerPC 604 CPU])
		host_cpu="ppc604"
		default_arch="ppc"
		;;
	power3)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_POWERPC64_POWER3],[1],[Target a POWER3 CPU])
		default_arch="ppc64"
		CPPFLAGS="$CPPFLAGS -Upower3 -D__CPU__=power3"
		;;
	power3p|power3+)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_POWERPC64_POWER3p],[1],[Target a POWER3+ CPU])
		host_cpu="power3p"
		default_arch="ppc64"
		;;
	power4)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_POWERPC64_POWER4],[1],[Target a POWER4 CPU])
		default_arch="ppc64"
		;;
	power3p|power3+)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_POWERPC64_POWER4p],[1],[Target a POWER4+ CPU])
		host_cpu="power3p"
		default_arch="ppc64"
		;;
	ppc970|ppc64)
		AC_DEFINE_UNQUOTED([CONFIG_CPU_POWERPC64_PPC970],[1],[Target a PowerPC 970 CPU])
		default_cpu="ppc970"
		default_arch="ppc64"
		;;
    *)
		AC_MSG_ERROR([host CPU type ${host_cpu} is not supported])
esac
AC_MSG_RESULT([${host_cpu}])

with_arch=${default_arch}

AC_ARG_WITH([subarch],[AS_HELP_STRING([--with-subarch=SUBARCH],[specify sub-architecture (for x86 systems - either x32 or x64, default is x32)])],,[with_subarch="$default_subarch"])

is_32bit=no
is_64bit=no
multi_architecture=no
		x86_small_spaces=no
		x86_io_flexpages=no
		x86_compatibility_mode=no
		x86_keep_last_branches=no
		k8_flushfilter=no
		x86_sysenter=no
		x86_fxsr=no
		x86_pge=no
		x86_htt=no
		x86_small_spaces_global=no
		x86_pse=no
		x86_tsc=no
		x86_pat=no
		x86_em64t=no

kdb_disas=no
kdb_no_asserts=no
verbose_init=no
tracepoints=no
kmem_trace=no
cpu_x86_simics_speed=100
sysv_abi=yes

dnl JL_ENABLE_OPTION(define, var, arg, description, default, cmds-if-true, cmds-if-false)
AC_DEFUN([JL_ENABLE_OPTION],[
AC_MSG_CHECKING([whether to] $4)
AC_ARG_ENABLE($2,[AS_HELP_STRING($3,$4)],[$2=$enableval],[$2=$5])
$1="$$2"
if test x"$$2" = x"yes" ; then
	AC_DEFINE_UNQUOTED($1,[1],[$4])
	$6
else
	$7
	true
fi
AC_SUBST($1)
AC_MSG_RESULT($$2)
])
AC_DEFUN([JL_ENABLE_OPTION_COND],[
AC_MSG_CHECKING([whether to] $4)
AC_ARG_ENABLE($2,[AS_HELP_STRING($3,$4)],[$2=$enableval],[$2=$5])
$1="$$2"
if test x"$$2" = x"yes" ; then
	AC_DEFINE_UNQUOTED($1,[1],[$4])
	$6
else
	$7
	true
fi
AC_SUBST($1)
AM_CONDITIONAL($1,test x"$$2" = x"yes")
AC_MSG_RESULT($$2)
])
AC_DEFUN([JL_WITH_OPTION],[
AC_MSG_CHECKING($4)
AC_ARG_WITH($2,[AS_HELP_STRING($3,$4)],[$2=$enableval],[$2=$5])
$1="$$2"
if test x"$$2" = x"yes" ; then
	AC_DEFINE_UNQUOTED($1,[1],[$4])
	$6
else
	$7
	true
fi
AC_SUBST($1)
AC_MSG_RESULT($$2)
])
AC_DEFUN([JL_DEFINE_IF],[
if $4 ; then
   AC_DEFINE_UNQUOTED($1,$2,$3)
fi
])

JL_ENABLE_OPTION(CONFIG_SMP,smp,[--enable-smp],[enable symmetric multiprocessing support],no,[
	JL_WITH_OPTION(CONFIG_SMP_MAXPROCS,smp_maxprocs,[--with-smp-maxprocs=NPROCS],[  maximum number of CPUs (default=4)],4)
	JL_ENABLE_OPTION(CONFIG_SMP_IDLE_POLL,smp_idle_poll,[--enable-idle-poll],[ poll XCPU queue on idle],no)
])
JL_ENABLE_OPTION_COND(CONFIG_IOAPIC,ioapic,[--enable-iopic],[use APIC+IOAPIC],no,[
	JL_WITH_OPTION(CONFIG_MAX_IOAPICS,max_ioapics,[--with-max-ioapics=NUM],[maximum number of IOAPICs (default=2)],2)
	JL_WITH_OPTION(CONFIG_APIC_TIMER_TICK,apic_timer_tick,[--apic-timer-tick=NUM],[timer tick length for APIC (default=1000)],1000)
])
JL_WITH_OPTION(CONFIG_BOOTMEM_PAGES,bootmem_pages,[--with-bootmem-pages=NUM],[number of pages to reserve for the kernel (default=1024)],1024)
JL_ENABLE_OPTION(CONFIG_IPC_FASTPATH,ipc_fastpath,[--enable-ipc-fastpath],[enable the fast IPC path],no)
JL_ENABLE_OPTION_COND(CONFIG_DEBUG,debug,[--enable-debug],[enable debugging mode],yes)
JL_ENABLE_OPTION(CONFIG_PERFMON,perfmon,[--enable-perfmon],[enable performance monitoring],no)
JL_ENABLE_OPTION(CONFIG_SPINWHEELS,spinwheels,[--enable-spinwheels],[enable spin wheels],no)
JL_ENABLE_OPTION_COND(CONFIG_NEW_MDB,new_mdb,[--enable-new-mdb],[enable new mapping database],no)
JL_ENABLE_OPTION(CONFIG_EXPERIMENTAL,experimental,[--enable-experimental],[enable experimental features],no,[
	JL_ENABLE_OPTION(CONFIG_X_PAGER_EXREGS,x_pager_exregs,[--enable-x-pager-exregs],[enable the pager ExchangeRegisters feature])
])
JL_ENABLE_OPTION(CONFIG_KDB,kdb,[--enable-kdb],[enable the kernel debugger],no,[
	JL_ENABLE_OPTION(CONFIG_KDB_CONS_COM,kdb_cons_com,[--enable-kdb-cons-com],[enable the KDB serial port],no,[
		kdb_console=yes
		JL_WITH_OPTION(CONFIG_KDB_CONS_COMPORT,kdb_cons_comport,[--with-kdb-cons-comport=IOPORT],[the port to use for the KDB serial console (default=0x3f8)],0x3f8)
		JL_WITH_OPTION(CONFIG_KDB_CONS_COMSPEED,kdb_cons_comspeed,[--with-kdb-cons-comspeed=BAUD],[the KDB serial port speed (default=115200)],115200)
	])
	JL_ENABLE_OPTION(CONFIG_KDB_CONS_KBD,kdb_cons_kbd,[--enable-kdb-cons-kbd],[enable the KDB keyboard],no,[kdb_console=yes])
	JL_ENABLE_OPTION(CONFIG_KDB_DISAS,kdb_disas,[--enable-kdb-disas],[enable the KDB disassembler],no,[kdb_disas=yes])
])
AM_CONDITIONAL([CONFIG_KDB_DISAS],[test x"$kdb_disas" = x"yes"])

case "${host_cpu}" in
	i486|c3)
		tracebuffer=no
		;;
	*)
		JL_ENABLE_OPTION(CONFIG_TRACEBUFFER,tracebuffer,[--enable-tracebuffer],[enable the trace buffer],no,[
			JL_ENABLE_OPTION(CONFIG_TBUF_LIGHT,tbuf_light,--enable-tbuf-light,[  enable the lightweight trace buffer],no)
			JL_ENABLE_OPTION(CONFIG_TBUF_PERFMON,tbuf_perfmon,--enable-tbuf-perfmon,[  enable performance counters],no)
		])
		;;
esac
AM_CONDITIONAL([CONFIG_TRACEBUFFER],[test x"$tracebuffer" = x"yes"])

AC_MSG_CHECKING([host CPU architecture])
if test x"${with_subarch}" = x"" ; then
	AC_MSG_RESULT([${with_arch}])
else
	AC_MSG_RESULT([${with_arch}/${with_subarch}])
fi
case "${with_arch}" in
	x86)
		AC_DEFINE_UNQUOTED([CONFIG_ARCH_X86],[1],[Target x86 or x86_64 systems])
		default_platform="pc99"
		case "${with_subarch}" in
			x32)
				AC_DEFINE_UNQUOTED([CONFIG_SUBARCH_X32],[1],[Target 32-bit x86 systems])
				CPPFLAGS="$CPPFLAGS -m32"
				CFLAGS="$CFLAGS -O2 -mpreferred-stack-boundary=2"
				LDFLAGS="$LDFLAGS -Wl,-melf_i386"
				LIBS="$LIBS -lgcc"
				test x"${host_cpu}" = x"i686" && CFLAGS="$CFLAGS -march=i686"
				is_32bit=yes
				JL_ENABLE_OPTION_COND([CONFIG_X86_SMALL_SPACES],[small_spaces],[--enable-small-spaces],[enable small address spaces],[no])
				;;
			x64)
				case "$host_cpu" in
					i686|p4|k8|simics)
						;;
					*)
						AC_MSG_ERROR([sub-architecture x64 is not supported with CPU ${host_cpu}])
						;;
				esac
				AC_DEFINE_UNQUOTED([CONFIG_SUBARCH_X64],[1],[Target 64-bit x64 systems])
				CPPFLAGS="$CPPFLAGS -m64"
				CFLAGS="$CFLAGS -O2 -mcmodel=kernel -mno-red-zone"
				LDFLAGS="$LDFLAGS -Wl,-melf_x86_64 -Wl,-n -Wl,-z -Wl,-max-page-size,4096"
				is_64bit=yes
				JL_ENABLE_OPTION([CONFIG_X86_COMPATIBILITY_MODE],[32bit_compat],[--enable-32bit-compat],[enable 32-bit compatibility mode],[no])
				;;
			*)
				AC_MSG_ERROR([sub-architecture ${with_subarch} is not supported on ${with_arch}])
				;;
		esac
		case "${host_cpu}" in
	 		i686|p4|k8)
	 			x86_sysenter=yes
				x86_fxsr=yes
				x86_pge=yes
				x86_pse=yes
				x86_pat=yes
				;;
		esac
		case "${host_cpu}" in
			p4)
				x86_htt=yes
				;;
		esac
		case "${host_cpu}" in
			i686|p4|k8|c3)
				x86_tsc=yes
				;;
		esac
		if test x"$CONFIG_X86_SMALL_SPACES" = x"yes" && test x"$x86_pge" = x"yes" ; then
			CONFIG_X86_SMALL_SPACES_GLOBAL=yes
			AC_DEFINE_UNQUOTED([CONFIG_X86_SMALL_SPACES_GLOBAL],[1],[small spaces is enabled and PGE is available])
			AC_SUBST([CONFIG_X86_SMALL_SPACES_GLOBAL])
		fi
		JL_DEFINE_IF([CONFIG_X86_SYSENTER],[1],[SYSENTER is available],[test x$x86_sysenter = xyes])
		JL_DEFINE_IF([CONFIG_X86_FXSR],[1],[the FXSR is available],[test x$x86_fxsr = xyes])
		JL_DEFINE_IF([CONFIG_X86_PGE],[1],[PGE is available],[test x$x86_pge = xyes])
		JL_DEFINE_IF([CONFIG_X86_PSE],[1],[PSE is available],[test x$x86_pse = xyes])
		JL_DEFINE_IF([CONFIG_X86_PAT],[1],[PAT is available],[test x$x86_pat = xyes])
		JL_DEFINE_IF([CONFIG_X86_HTT],[1],[HyperThreading is available],[test x$x86_htt = xyes])
		JL_DEFINE_IF([CONFIG_X86_TSC],[1],[TSC is available],[test x$x86_tsc = xyes])
		;;
	ppc)
		AC_DEFINE_UNQUOTED([CONFIG_ARCH_POWERPC],[1],[Target 32-bit PowerPC systems])
		default_platform="ofppc"
		is_32bit=yes
		;;
	ppc64)
		AC_DEFINE_UNQUOTED([CONFIG_ARCH_POWERPC64],[1],[Target 64-bit PowerPC systems])
		is_64bit=yes
		case "$host_cpu" in
			power3|power3p)
				default_platform="ofpower3"
				;;
			power4|power4p)
				default_platform="ofpower4"
				;;
			ppc970)
				default_platform="ofg5"
				;;
		esac
		;;
	*)
		AC_MSG_ERROR([architecture ${with_arch} is not supported])
		;;
esac

AM_CONDITIONAL([CONFIG_ARCH_X86],[test x$with_arch = xx86])
AM_CONDITIONAL([CONFIG_X86_IO_FLEXPAGES],[test x$x86_io_flexpages = xyes])

if test x"${is_32bit}" = x"yes" ; then
	AC_DEFINE_UNQUOTED([CONFIG_IS_32BIT],[1],[Indicate that this is a 32-bit platform])
fi

if test x"${is_64bit}" = x"yes" ; then
	AC_DEFINE_UNQUOTED([CONFIG_IS_64BIT],[1],[Indicate that this is a 64-bit platform])
fi

host_triplet="${host_cpu}-${host_vendor}-${host_os}"
host="${host_triplet}"

AC_MSG_CHECKING([final host system type])
AC_MSG_RESULT([${host_triplet}])

with_api=v4
AC_MSG_CHECKING([API version])
AC_MSG_RESULT([$with_api])

AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE

CFLAGS="$CFLAGS -ffreestanding"

AC_ARG_WITH([platform],[AS_HELP_STRING([--with-platform=TYPE],[specify host platform type (for x86: pc99 (default); for ppc: ofppc; for ppc64: ofpower4, ofpower3, ofg5])],,[with_platform=$default_platform])
AC_MSG_CHECKING([host platform type])
supported=yes
case "${with_arch}" in
	x86)
		LDSCRIPT="\${top_srcdir}/src/glue/${with_api}-${with_arch}/${with_subarch}/linker-${with_platform}.lds"
		case "${with_platform}" in
			pc99)
				AC_DEFINE_UNQUOTED([CONFIG_PLAT_PC99],[1],[Target PC99 systems])
				;;
			none) true ;;
			*) supported=no ;;
		esac
		;;
	ppc)
		case "${with_platform}" in
			ofppc)
				AC_DEFINE_UNQUOTED([CONFIG_PLAT_OFPPC],[1],[IEEE 1275 Open Firmware for PowerPC])
				;;
			none) true ;;
			*) supported=no ;;
		esac
		;;
	ppc64)
		case "${host_cpu}" in
			power3|power3p)
				case "${with_platform}" in
					ofpower3)
						AC_DEFINE_UNQUOTED([CONFIG_PLAT_OFPOWER3],[1],[OpenFirmware POWER3])
						;;
					none) true ;;
					*) supported=no ;;
				esac
				;;
			power4|power4p)
				case "${with_platform}" in
					ofpower4)
						AC_DEFINE_UNQUOTED([CONFIG_PLAT_OFPOWER4],[1],[OpenFirmware POWER4])
						;;
					none) true ;;
					*) supported=no ;;
				esac
				;;
			ppc970)
				case "${with_platform}" in
					ofg5)
						AC_DEFINE_UNQUOTED([CONFIG_PLAT_OFG5],[1],[OpenFirmware G5])
						;;
					none) true ;;
					*) supported=no ;;
				esac
				;;
		esac
		;;
esac
if test x"$supported" = x"no" ; then
	AC_MSG_ERROR([platform $with_platform is not supported on architecture $with_arch (with CPU $host_cpu)])
fi
AC_MSG_RESULT([$with_platform])

LDFLAGS="$LDFLAGS -O2 -static -nostdlib"

AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AM_PROG_AS
AC_CHECK_TOOL([NM],[nm])
AC_PROG_RANLIB
AC_PROG_AWK
AC_PROG_GREP
AC_PROG_SED
AC_CHECK_PROG([PERL],[perl],[perl])

dnl Reset DEFAULT_INCLUDES
DEFAULT_INCLUDES=''
CPPFLAGS="$CPPFLAGS -U${with_arch} -U${with_subarch} -U${host_cpu} -U${with_platform} -U${with_api}"
CPPFLAGS="$CPPFLAGS -D__ARCH__=${with_arch} -D__SUBARCH__=${with_subarch} -D__CPU__=${host_cpu} -D__PLATFORM__=${with_platform} -D__API__=${with_api}"
CPPFLAGS="$CPPFLAGS -Wall -Wno-format -Wconversion"
CPPFLAGS="$CPPFLAGS -Wno-non-virtual-dtor"
CPPFLAGS="$CPPFLAGS -nostdinc"
CPPFLAGS="$CPPFLAGS -I\${top_builddir} -I\${top_srcdir}"
CPPFLAGS="$CPPFLAGS -I\${top_builddir}/src -I\${top_srcdir}/src"
CPPFLAGS="$CPPFLAGS -I\${top_builddir}/src/generic -I\${top_srcdir}/src/generic"
CPPFLAGS="$CPPFLAGS -I\${top_builddir}/include -I\${top_srcdir}/include"
CPPFLAGS="$CPPFLAGS -imacros \${top_builddir}/config.h"
CPPFLAGS="$CPPFLAGS -imacros \${top_srcdir}/src/generic/macros.h"
CPPFLAGS="$CPPFLAGS -include \${top_srcdir}/src/generic/config.h"

CCASFLAGS="$CCASFLAGS -DASSEMBLY"

CFLAGS="$CFLAGS -include \${top_srcdir}/src/generic/types.h"

CXXFLAGS="$CXXFLAGS -include \${top_srcdir}/src/generic/types.h"
CXXFLAGS="$CXXFLAGS -fno-rtti -fno-builtin -fomit-frame-pointer -fno-exceptions -fpermissive"

AC_SUBST([DEFAULT_INCLUDES])

AC_MSG_CHECKING([for libgcc includes])
gccdir=`$CC -print-libgcc-file-name`
LIBGCCINC=`dirname $gccdir`/include
if test -r $LIBGCCINC/stdarg.h ; then
	true
else
	LIBGCCINC="/usr/include"
fi
AC_MSG_RESULT([$LIBGCCINC])
CPPFLAGS="$CPPFLAGS -I$LIBGCCINC"

AC_DEFINE_UNQUOTED([AUTOCONF_INCLUDED],[1],[Indicate that config.h has been included])

PLATFORM="${with_platform}"
AC_SUBST([PLATFORM])
API="${with_api}"
AC_SUBST([API])
ARCH="${with_arch}"
AC_SUBST([ARCH])
SUBARCH="${with_subarch}"
AC_SUBST([SUBARCH])
CPU="${host_cpu}"
AC_SUBST([CPU])

begin_group="-Wl,-whole-archive"
end_group="-Wl,-no-whole-archive"

AC_SUBST([begin_group])
AC_SUBST([end_group])

AC_SUBST([LDSCRIPT])

AC_CONFIG_HEADER([config.h])

AC_CONFIG_FILES([
Makefile
src/Makefile
src/generic/Makefile
src/platform/Makefile
src/platform/pc99/Makefile
src/api/Makefile
src/api/v4/Makefile
src/arch/Makefile
src/arch/x86/Makefile
src/arch/x86/x32/Makefile
src/glue/Makefile
src/glue/v4-x86/Makefile
src/glue/v4-x86/x32/Makefile
kdb/Makefile
kdb/api/Makefile
kdb/api/v4/Makefile
kdb/arch/Makefile
kdb/arch/x86/Makefile
kdb/arch/x86/x32/Makefile
kdb/glue/Makefile
kdb/glue/v4-x86/Makefile
kdb/glue/v4-x86/x32/Makefile
kdb/platform/Makefile
kdb/platform/pc99/Makefile
kdb/generic/Makefile
])

AC_CONFIG_COMMANDS([include/version.h],[
${SHELL} genversion.sh include/version.h],[
KERNEL_VERSION=`echo $VERSION | cut -f1 -d.`
export KERNEL_VERSION
KERNEL_SUBVERSION=`echo $VERSION | cut -f2 -d.`
export KERNEL_SUBVERSION
KERNEL_SUBSUBVERSION=`echo $VERSION | cut -f3 -d.`
export KERNEL_SUBSUBVERSION
KERNEL_DATE_DAY=`date +%d | sed 's%^0%%'`
export KERNEL_DATE_DAY
KERNEL_DATE_MONTH=`date +%m | sed 's%^0%%'`
export KERNEL_DATE_MONTH
KERNEL_DATE_YEAR=`date +%y | sed 's%^0%%'`
export KERNEL_DATE_YEAR
])

AC_CONFIG_COMMANDS([kernel.lds],[
${SHELL} genkernellds.sh kernel.lds "${LDSCRIPT}"],[
top_builddir=.
export top_builddir
top_srcdir=\$srcdir
export top_srcdir
LDSCRIPT="$LDSCRIPT"
])

AC_CONFIG_COMMANDS([include/tcb_layout.h],[
${SHELL} gentcblayout.sh include/tcb_layout.h "${srcdir}"],[
AWK="$AWK"
export AWK
SED="$SED"
export SED
CC="$CC"
export CC
CPP="$CPP"
export CPP
PERL="$PERL"
export PERL
GREP="$GREP"
export GREP
L4_API="$with_api"
export L4_API
L4_ARCH="$with_arch"
export L4_ARCH
CPPFLAGS="$CPPFLAGS"
export CPPFLAGS
CFLAGS="$CFLAGS"
export CFLAGS
CXXFLAGS="$CXXFLAGS"
export CXXFLAGS
])
AC_OUTPUT
