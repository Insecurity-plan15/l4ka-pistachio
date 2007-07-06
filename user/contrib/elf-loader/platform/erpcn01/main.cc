/*********************************************************************
 *                
 * Copyright (C) 2002,  University of New South Wales
 *                
 * File path:     elf-loader/src/platform/u4600/main.cc
 * Description:   Main file for elf loader 
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
 * $Id: main.cc,v 1.3 2004/05/14 05:16:40 cvansch Exp $
 *                
 ********************************************************************/

#include "elf-loader.h"

#define PHYS_OFFSET 0xffffffff80000000

extern L4_KernelConfigurationPage_t *kip;

volatile unsigned int *propane_uart = (unsigned int *)0x900000001f100000;

extern "C" void putc(char c)
{
    while ((propane_uart[2] & 0x0F) >= 0xD); /* fifo count */
    propane_uart[1] = (unsigned char)c;
}

extern "C" void memset (char * p, char c, int size)
{
    for (;size--;)
	*(p++)=c;
}

extern "C" __attribute__ ((weak)) void *
memcpy (void * dst, const void * src, unsigned int len)
{
    unsigned char *d = (unsigned char *) dst;
    unsigned char *s = (unsigned char *) src;

    while (len-- > 0)
	*d++ = *s++;

    return dst;
}


void start_kernel(L4_Word_t bootaddr)
{
    void (*func)(unsigned long) = (void (*)(unsigned long)) (bootaddr | PHYS_OFFSET);

    /* XXX - Get this from boot loader */
    kip->MainMem.high = 32UL * 1024 * 1024;
    kip->MemoryInfo.n = 0;
    
    func(0);
}

int main(void)
{
    unsigned int temp;
    /* Disable caches */
    __asm__ __volatile__ (
	"mfc0    %0,$16;    \n\t"
	"li      $31,-8;    \n\t"
	"and     %0,%0,$31;  \n\t"
	"ori     %0,%0,0x2; \n\t"
	"mtc0    %0,$16;    \n\t"
	: "=r" (temp) : : "$31"
    );

    L4_Word_t entry;
    
    if(load_modules(&entry, PHYS_OFFSET)) {
	asm ("break\n\t");
    }
    
    start_kernel(entry);
    
    asm ("break\n\t");
}
