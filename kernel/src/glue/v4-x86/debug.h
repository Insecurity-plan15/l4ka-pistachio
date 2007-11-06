/*********************************************************************
 *                
 * Copyright (C) 2002-2004, 2007,  Karlsruhe University
 *                
 * File path:     glue/v4-x86/debug.h
 * Description:   x86 debugging support
 *                
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *                
 * $Id: debug.h,v 1.8.4.3 2006/12/05 15:53:45 skoglund Exp $
 *                
 ********************************************************************/
#ifndef __GLUE__X86__DEBUG_H__
#define __GLUE__X86__DEBUG_H__

#include INC_ARCH(trapgate.h)
#include INC_GLUE(config.h)

#if defined(CONFIG_DEBUG)

extern "C" int printf(const char* format, ...);
INLINE void dump_flags(const word_t flags)
{
    printf("%c%c%c%c%c%c%c%c%c%c%c",
	   flags & (1 <<  0) ? 'C' : 'c',
	   flags & (1 <<  2) ? 'P' : 'p',
	   flags & (1 <<  4) ? 'A' : 'a',
	   flags & (1 <<  6) ? 'Z' : 'z',
	   flags & (1 <<  7) ? 'S' : 's',
	   flags & (1 << 11) ? 'O' : 'o',
	   flags & (1 << 10) ? 'D' : 'd',
	   flags & (1 <<  9) ? 'I' : 'i',
	   flags & (1 <<  8) ? 'T' : 't',
	   flags & (1 << 16) ? 'R' : 'r',
	   ((flags >> 12) & 3) + '0'
	);
}

#define DEBUG_SCREEN (KERNEL_OFFSET + 0xb8000)


INLINE void spin_forever(int pos = 0)
{
#if defined(CONFIG_SPIN_WHEELS)
    while(1)
	    ((u16_t*)(DEBUG_SCREEN))[pos] += 1;
#else /* defined(CONFIG_SPIN_WHEELS) */
    int dummy = 0;
    while(1)
	    dummy = (dummy + 1) % 32;
#endif /* defined(CONFIG_SPIN_WHEELS) */
}

class space_t;
class tcb_t;

class debug_param_t 
{
   
public:
    word_t exception;
    space_t * space;
    tcb_t * tcb;
    x86_exceptionframe_t * frame;
};

INLINE void spin(int pos, int cpu = 0)
{
#if defined(CONFIG_SPIN_WHEELS)
    ((u8_t*)(DEBUG_SCREEN))[(cpu * 160) + pos * 2] += 1;
    ((u8_t*)(DEBUG_SCREEN))[(cpu * 160) + pos * 2 + 1] = 7;
#endif /* defined(CONFIG_SPIN_WHEELS) */
}

#define enter_kdebug(arg...)                    \
    __asm__ __volatile__ (                      \
            "   int $3                  \n"     \
            "   jmp 1f                  \n"     \
            "   mov $2f, %0             \n"     \
            ".section .rodata           \n"     \
            "2:.ascii \"KD# " arg "\"   \n"     \
            ".byte 0                    \n"     \
            ".previous                  \n"     \
            "1:                         \n"     \
            :                                   \
            : "a" (0UL))

#else
#define dump_flags(x...)	do { } while (true)

#endif /* defined(CONFIG_DEBUG) */

#endif /* !__GLUE__X86__DEBUG_H__ */
