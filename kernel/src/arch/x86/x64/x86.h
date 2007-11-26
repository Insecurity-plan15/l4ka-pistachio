/*********************************************************************
 *                
 * Copyright (C) 2003-2004, 2006-2007,  Karlsruhe University
 *                
 * File path:     arch/x86/x64/x86.h
 * Description:   X86-64 CPU Specific constants
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
 * $Id: amd64.h,v 1.6 2006/09/28 08:03:20 stoess Exp $
 *                
 ********************************************************************/
#ifndef __ARCH__X86__X64__X86_H__
#define __ARCH__X86__X64__X86_H__

#include INC_ARCH(x86.h)


/**********************************************************************
 *    MMU
 **********************************************************************/

/* Sign extend 63..48 */
#define AMD64_SIGN_EXTEND_BITS   48
#define AMD64_SIGN_EXTEND_SIZE   (X86_64BIT_ONE << AMD64_SIGN_EXTEND_BITS)
#define AMD64_SIGN_EXTEND_MASK   (~(AMD64_SIGN_EXTEND_SIZE - 1))
#define AMD64_SIGN_EXTENSION	 (~(AMD64_SIGN_EXTEND_SIZE - 1))

/* Page map 47.. 39 */
#define AMD64_PML4_BITS		39
#define AMD64_PML4_SIZE         (X86_64BIT_ONE << AMD64_PML4_BITS)
#define AMD64_PML4_MASK         ((~(AMD64_PML4_SIZE - 1)) ^ (~(AMD64_SIGN_EXTEND_SIZE - 1)))
#define AMD64_PML4_IDX(x)	((x & AMD64_PML4_MASK) >> AMD64_PML4_BITS)

/* Page directory pointer 38..30 */
#define AMD64_PDP_BITS           30
#define AMD64_PDP_SIZE           (X86_64BIT_ONE << AMD64_PDP_BITS)
#define AMD64_PDP_MASK           ((~(AMD64_PDP_SIZE - 1))  ^ (~(AMD64_PML4_SIZE - 1)))
#define AMD64_PDP_IDX(x)	 ((x & AMD64_PDP_MASK) >> AMD64_PDP_BITS)

/* Page directory 29..21 */
#define AMD64_PDIR_BITS         21
#define AMD64_PDIR_SIZE         (X86_64BIT_ONE << AMD64_PDIR_BITS)
#define AMD64_PDIR_MASK         ((~(AMD64_PDIR_SIZE - 1))  ^ (~(AMD64_PDP_SIZE - 1)))
#define AMD64_PDIR_IDX(x)	((x & AMD64_PDIR_MASK) >> AMD64_PDIR_BITS)

/* Pagetable 20..12  */
#define AMD64_PTAB_BITS          12
#define AMD64_PTAB_SIZE          (X86_64BIT_ONE << AMD64_PTAB_BITS)
#define AMD64_PTAB_MASK          ((~(AMD64_PTAB_SIZE - 1))  ^ (~(AMD64_PTAB_SIZE - 1)))
#define AMD64_PTAB_IDX(x)        ((x & AMD64_PTAB_MASK) >> AMD64_PTAB_BITS)


#define X86_PAGE_CPULOCAL       (1<<9)
#define X86_PAGE_NX		(1<<63)

/**
 * 
 * Bits to zero out invalid parts of pagetable entries 
 */

/* Normal pagetable entry 11..0  */
#define AMD64_PTE_BITS			12
#define AMD64_PTE_SIZE			(X86_64BIT_ONE << AMD64_PTE_BITS) 
#define AMD64_PTE_MASK			(~(AMD64_PTE_SIZE - 1))
#define AMD64_PTE_FLAGS_MASK		(0x0e3f)

/* pagefault error code bits */
#define AMD64_PF_RW	(1 << 1)	/* Pagefault on read/write	*/
#define AMD64_PF_US	(1 << 2)	/* Pagefault in user/kernel	*/
#define AMD64_PF_ID	(1 << 4)	/* Pagefault on insn./data	*/

/* 2 MByte (Super-) Pages 20..0  */
#define X86_SUPERPAGE_BITS		21
#define X86_SUPERPAGE_SIZE		(__UL(1) << X86_SUPERPAGE_BITS) 
#define X86_SUPERPAGE_MASK		(~(X86_SUPERPAGE_SIZE - 1))
#define X86_SUPERPAGE_FLAGS_MASK	(0x1fff)

#define X86_PAGE_FLAGS_MASK		(0x0fff)

#define X86_PAGEFAULT_BITS		(AMD64_PF_RW | AMD64_PF_ID)

#define X86_TOP_PDIR_BITS		AMD64_PML4_BITS
#define X86_TOP_PDIR_SIZE		AMD64_PML4_SIZE
#define X86_TOP_PDIR_IDX(x)		AMD64_PML4_IDX(x)


/**********************************************************************
 *    CPU features (CPUID)  
 **********************************************************************/

/* extended feature register (EFER) bits */
#define AMD64_EFER_SCE  (1 <<  0)       /* system call extensions       */
#define AMD64_EFER_LME  (1 <<  8)       /* long mode enabled            */
#define AMD64_EFER_LMA  (1 << 10)       /* long mode active             */
#define AMD64_EFER_NXE  (1 << 11)       /* nx bit enable                */

/**********************************************************************
 * Model specific register locations.
 **********************************************************************/

#define AMD64_MCG_CAP_MSR               0x0179  /* Machine Check Global Capabilities */
#define AMD64_MCG_STATUS_MSR            0x0179  /* Machine Check Global Status  */
#define AMD64_MCG_CTL_MSR               0x0179  /* Machine Check Global Control  */

#define AMD64_DEBUGCTL_MSR              0x01d9  /* Debug-Control  */

#define AMD64_MC0_MISC_MSR              0x0403  /* Machine Check Error Information */
#define AMD64_MC1_MISC_MSR              0x0407  /* Machine Check Error Information */
#define AMD64_MC2_MISC_MSR              0x040b  /* Machine Check Error Information */
#define AMD64_MC3_MISC_MSR              0x040f  /* Machine Check Error Information */


#define AMD64_EFER_MSR                  0xC0000080      /* Extended Features */
#define AMD64_STAR_MSR                  0xC0000081      /* SYSCALL/RET CS,
							 * SYSCALL EIP (legacy) */
#define AMD64_LSTAR_MSR                 0xC0000082      /* SYSCALL RIP (long) */
#define AMD64_CSTAR_MSR                 0xC0000083      /* SYSCALL RIP (comp) */
#define AMD64_SFMASK_MSR                0xC0000084      /* SYSCALL flag mask */

#define AMD64_FS_MSR                    0xC0000100      /* FS Register */
#define AMD64_GS_MSR                    0xC0000101      /* GS Register */
#define AMD64_KRNL_GS_MSR               0xC0000102      /* Kernel GS Swap  */
#define KERNEL_VERSION_VER              KERNEL_VERSION_CPU_AMD64

#define AMD64_PERFCTR0                  0x0c1
#define AMD64_PERFCTR1                  0x0c2
#define AMD64_EVENTSEL0                 0x186
#define AMD64_EVENTSEL1                 0x187
#define AMD64_LASTBRANCHFROMIP          0x1db
#define AMD64_LASTBRANCHTOIP            0x1dc
#define AMD64_LASTINTFROMIP             0x1dd
#define AMD64_LASTINTTOIP               0x1de
#define AMD64_MTRRBASE(x)               (0x200 + 2*(x) + 0)
#define AMD64_MTRRMASK(x)               (0x200 + 2*(x) + 1)

/**********************************************************************
 *   Cache line configurations
 **********************************************************************/

#define AMD64_CACHE_LINE_SIZE          64


#endif /* !__ARCH__X86__X64__X86_H__ */
