SECTIONS
{
        .text : {
	_kickstart_begin = .;
        *(.mbh)
        *(.text)
        *(.*data*)
        _edata = .;
        }
        _end = .;
	_kickstart_end = .;
	/DISCARD/ :
	{
		*(.eh_frame)
		*(.note)
		*(.comment)
        }
}
