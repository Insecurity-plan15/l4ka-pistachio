#! /bin/sh

output="$1"

rm -f "$1"
user="$LOGNAME@`hostname | cut -f1 -d.`"
if test -r .build ; then
	build=`cat .build`
    build=`expr $build + 1`
else
	build=1
fi
echo $build > .build
cat >"$1" <<EOF
/* automatically generated - do NOT edit */

#define KERNEL_VERSION                 ${KERNEL_VERSION}
#define KERNEL_SUBVERSION              ${KERNEL_SUBVERSION}
#define KERNEL_SUBSUBVERSION           ${KERNEL_SUBSUBVERSION}

#define KERNELGENDATE                  SHUFFLE3(day:${KERNEL_DATE_DAY},month:${KERNEL_DATE_MONTH},year:${KERNEL_DATE_YEAR})

#define __USER__                       "${user}"

#define __KERNELBUILDNO__              ${build}
EOF
