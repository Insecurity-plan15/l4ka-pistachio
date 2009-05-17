#! /bin/sh

output="$1"
srcdir="$2"

extract_members() {
	$AWK 'BEGIN { printme = 0 } \
		/TCB_END_MARKER/ { printme = 0 } \
			printme == 1 { print } \
		/TCB_START_MARKER/ { printme = 1 } ' \
		$1 | \
	$CPP -imacros config.h -P - | \
	$GREP ';$' | \
	$PERL -pe 's/.*\b(\w+)(\[.*\])*;.*/\1/g'
}

tmpfile="tcblayout.tmp.$$.c"
tmpfiles="tcblayout.tmp.$$.s"
rm -f "$tmpfile"
printf '
#include INC_API(tcb.h)\n
\n
tcb_t tcb;                               \n
utcb_t utcb;                             \n
\n
#define O(sym,value) __asm__ __volatile__ ("\\n#define " MKSTR(sym) " %%0 " : : "i" (value)); \n
\n
void make_offsets()                      \n
{                                        \n' >"$tmpfile"
extract_members "$srcdir/src/api/${L4_API}/tcb.h" | $AWK '{ print "   O(OFS_TCB_"toupper($1)",offsetof(tcb_t,"$1"));" } ' >> "$tmpfile"
extract_members "$srcdir/src/glue/${L4_API}-${L4_ARCH}/ktcb.h" | $AWK '{ print "   O(OFS_TCB_ARCH_"toupper($1)",offsetof(tcb_t,arch."$1"));" } ' >> "$tmpfile"
extract_members "$srcdir/src/glue/${L4_API}-${L4_ARCH}/utcb.h" | $AWK '{ print "   O(OFS_UTCB_"toupper($1)",offsetof(utcb_t,"$1"));" } ' >> "$tmpfile"
printf "\n}\n" >> "$tmpfile"
$CC -x c++ -w ${CPPFLAGS} ${COMMON_INC} ${CPP_INC} ${CFLAGS} ${CXXFLAGS} -DBUILD_TCB_LAYOUT -S -o $tmpfiles $tmpfile || ( rm -f $tmpfiles $tmpfile ; exit $? )
printf '/* machine-generated file - do NOT edit */\n' > ${output}.new
printf '#ifndef __TCB_LAYOUT__H__\n' >> ${output}.new
printf '#define __TCB_LAYOUT__H__\n\n' >> ${output}.new
$GREP ^#define $tmpfiles | $SED -e 's/ [\$#]/ /' >> ${output}.new
printf '\n#endif /* __TCB_LAYOUT__H__ */\n' >> ${output}.new
rm $tmpfile
rm $tmpfiles

cmp -s ${output} ${output}.new && rm -f ${output}.new || mv ${output}.new ${output}
