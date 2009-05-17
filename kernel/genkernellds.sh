#! /bin/sh

${CPP} -DASSEMBLY ${CPPFLAGS} -P -C - < $2 > $1.tmp
cmp -s $1.tmp $1 && rm -f $1.tmp || mv $1.tmp $1
