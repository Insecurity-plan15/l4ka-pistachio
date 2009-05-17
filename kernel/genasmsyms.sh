#! /bin/sh

output="$1"
obj="$2"
printf "/* machine-generated file - do NOT edit */\n"	> ${output}.new
printf "#ifndef __ASMSYMS_H__\n"				>> ${output}.new
printf "#define __ASMSYMS_H__\n\n"				>> ${output}.new
printf '($val, $name, $type, $num) ='			> mkasmsyms.pl
printf '(/^\d+ (\d*) ?. (.*)_(sign|b(\d))$/);'		>> mkasmsyms.pl
printf '$val /= 32;'					>> mkasmsyms.pl
printf 'if ($type eq "sign") {'				>> mkasmsyms.pl
printf '	printf ("#define %%-25s (%%s(0x%%x%%x))\\n",'	>> mkasmsyms.pl
printf '	    $name, $val == 2 ? "" : "-",'		>> mkasmsyms.pl
printf '	    $val_high, $val_low);'			>> mkasmsyms.pl
printf '	    $val_low = $val_high = 0;'		>> mkasmsyms.pl
printf '	} else {'					>> mkasmsyms.pl
printf '	    if ($num <= 3) {'				>> mkasmsyms.pl
printf '		$val_low += $val << ($num * 8);'	>> mkasmsyms.pl
printf '	    } elsif ($num <= 7) {'			>> mkasmsyms.pl
printf '		$val_high += $val << (($num-4) * 8);'>> mkasmsyms.pl
printf ' }'							>> mkasmsyms.pl
printf ' }'							>> mkasmsyms.pl
($NM --radix=d -S $obj | $PERL -n mkasmsyms.pl || exit $?)	>> ${output}.new
printf "\n\n#endif /* __ASMSYMS_H__ */\n"			>> ${output}.new
rm -f mkasmsyms.pl
cmp -s ${output} ${output}.new && rm -f ${output}.new || mv ${output}.new ${output}
