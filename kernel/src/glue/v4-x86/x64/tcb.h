/*********************************************************************
 *                
 * Copyright (C) 2002-2007,  Karlsruhe University
 *                
 * File path:     glue/v4-x86/x64/tcb.h
 * Description:   TCB related functions for Version 4, AMD64
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
 * $Id: tcb.h,v 1.40 2007/02/21 07:09:57 stoess Exp $
 *                
 ********************************************************************/
#ifndef __GLUE_V4_X86__X64__TCB_H__
#define __GLUE_V4_X86__X64__TCB_H__

#include INC_ARCH_SA(tss.h)			/* for amd64_tss_t */

#if defined(CONFIG_X86_COMPATIBILITY_MODE)
#include INC_GLUE_SA(x32comp/tcb.h)
#else 


/**
 * copies a set of message registers from one UTCB to another
 * @param dest destination TCB
 * @param start MR start index
 * @param count number of MRs to be copied
 */
INLINE void tcb_t::copy_mrs(tcb_t * dest, word_t start, word_t count)
{
    ASSERT(start + count <= IPC_NUM_MR);
    ASSERT(count > 0);
    word_t dummy;

    /* use optimized IA32 copy loop -- uses complete cacheline
       transfers */
    __asm__ __volatile__ (
	"cld\n"
	"rep  movsq (%0), (%1)\n"
	: /* output */
	"=S"(dummy), "=D"(dummy), "=c"(dummy)
	: /* input */
	"c"(count), "S"(&get_utcb()->mr[start]), 
	"D"(&dest->get_utcb()->mr[start]));

}


#endif /* !defined(CONFIG_X86_COMPATIBILITY_MODE) */



/********************************************************************** 
 *
 *                      thread switch routines
 *
 **********************************************************************/

#ifndef BUILD_TCB_LAYOUT
#include <tcb_layout.h>
#include <kdb/tracebuffer.h>

/**
 * switch to initial thread
 * @param tcb TCB of initial thread
 *
 * Initializes context of initial thread and switches to it.  The
 * context (e.g., instruction pointer) has been generated by inserting
 * a notify procedure context on the stack.  We simply restore this
 * context.
 */
INLINE void NORETURN initial_switch_to (tcb_t * tcb)
{
    /*
     * jsXXX: actually one should change tss->rsp0 before switching. However,
     * since we currently switch to idle and don't change privilege levels,
     * there's no real problem here.    
     *	      
     */ 
    asm("movq %0, %%rsp\n"
         "retq\n"
         :
	: "r"(tcb->stack));
    
     while (true)
         /* do nothing */;
}
/**
 * switches to another tcb thereby switching address spaces if needed
 * @param dest tcb to switch to
 */

INLINE void tcb_t::switch_to(tcb_t * dest)
{
    word_t dummy;
    
    ASSERT(dest->stack);
    ASSERT(dest != this);
    ASSERT(get_cpu() == dest->get_cpu());

    if ( EXPECT_FALSE(this->resource_bits))
	resources.save(this);
	
    /* modify stack in tss */
    tss.set_rsp0((u64_t)dest->get_stack_top());

#if 0
    TRACEF("\ncurr=%t (sp=%p, pdc=%p, spc=%p)\ndest=%t (sp=%p, pdc=%p, spc=%p)\n",
	   this, this->stack, this->pdir_cache, this->space,
	   dest, dest->stack, dest->pdir_cache, dest->space);
    //enter_kdebug("hmm");
#endif
    tbuf_record_event (1, 0, "switch %t => %t", (word_t)this, (word_t)dest);

#ifdef CONFIG_SMP
    active_cpu_space.set(get_cpu(), dest->space);
#endif
    __asm__ __volatile__ (
	"/* switch_to_thread */			\n\t"
	"movq	%[dtcb], %%r12			\n\t"	/* save dest			*/
	"pushq	%%rbp				\n\t"	/* save rbp			*/
	
	"pushq	$3f				\n\t"	/* store return address		*/
	
	"movq	%%rsp, %c[stack](%[stcb])	\n\t"	/* switch stacks		*/
	"movq	%c[stack](%[dtcb]), %%rsp	\n\t"
	
	"cmpq	%[spdir], %[dpdir]		\n\t"	/* same pdir_cache?		*/
	"je	2f				\n\t"

	"cmpq	$0, %c[space](%[dtcb])		\n\t"	/* kernel thread (space==NULL)?	*/
	"jnz	1f				\n\t"	
	"movq	%[spdir], %c[pdir](%[dtcb])	\n\t"	/* yes: update dest->pdir_cache */
	"jmp	2f				\n\t"

	"1:					\n\t"
	"movq	%[dpdir], %%cr3			\n\t"	/* no:  reload pagedir		*/
	"2:					\n\t"
	/* srXXX: Replace popq and jmpq with retq? */
	"popq	%%rdx				\n\t"	/* load (new) return address	*/
	"movq   %[utcb], %%gs:0		        \n\t"   /* update current UTCB		*/
	"jmpq	*%%rdx				\n\t"	/* jump to new return address 	*/

	"3:					\n\t"
	"movq   %%r12, %[stcb]			\n\t"   /* restore this			*/
	"popq	%%rbp				\n\t"	/* restore rbp			*/
	"/* switch_to_thread */			\n\t"
	: /* output */
	  "=a" (dummy)						/* %0 RAX */
	: /* input */
	  [dtcb]	"D" (dest),				/* %1 RDI */
	  [stcb]	"S" (this),				/* %2 RSI */
	  [stack]	"i" (OFS_TCB_STACK),			/* %3 IMM */
	  [space]	"i" (OFS_TCB_SPACE),			/* %4 IMM */
	  [pdir]	"i" (OFS_TCB_PDIR_CACHE),		/* %5 IMM */
	  [dpdir]	"0" (dest->pdir_cache),			/* %6 RAX */
	  [spdir]	"c" (this->pdir_cache),			/* %7 RCX */
	  [utcb]	"b" (dest->get_local_id().get_raw())	/* %8 RBX */

	: /* clobber - trash global registers */ 
	  "memory", "rdx", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"
	);
	
    if ( EXPECT_FALSE(this->resource_bits) )
	resources.load(this);
}


/**********************************************************************
 *
 *                        in-kernel IPC invocation 
 *
 **********************************************************************/

/**
 * invoke an IPC from within the kernel
 *
 * @param to_tid destination thread id
 * @param from_tid from specifier
 * @param timeout IPC timeout
 * @return IPC message tag (MR0)
 */
INLINE msg_tag_t tcb_t::do_ipc (threadid_t to_tid, threadid_t from_tid,
                                timeout_t timeout)
{
    msg_tag_t tag;
   
    sys_ipc (to_tid, from_tid, 0, 0, 0, 0, timeout);
    tag.raw = get_mr (0);

    return tag;
}


/**********************************************************************
 *
 *                        notification functions
 *
 **********************************************************************/

/* notify prologue pops the arguments in their registers */
extern "C" void notify_prologue(void);

/**
 * create stack frame to invoke notify procedure
 * @param func notify procedure to invoke
 *
 * Create a stack frame in TCB so that next thread switch will invoke
 * the indicated notify procedure.
 */
INLINE void tcb_t::notify (void (*func)())
{
   *(--stack) = (word_t)func;   
   stack-=2;
   *(--stack) = (word_t)notify_prologue;
   

}

/**
 * create stack frame to invoke notify procedure
 * @param func notify procedure to invoke
 * @param arg1 1st argument to notify procedure
 *
 * Create a stack frame in TCB so that next thread switch will invoke
 * the indicated notify procedure.
 */
INLINE void tcb_t::notify (void (*func)(word_t), word_t arg1)
{
    *(--stack) = (word_t)func;   
    stack--;
    *(--stack) = arg1;
    *(--stack) = (word_t)notify_prologue;
    
}

/**
 * create stack frame to invoke notify procedure
 * @param func notify procedure to invoke
 * @param arg1 1st argument to notify procedure
 * @param arg2 2st argument to notify procedure
 *
 * Create a stack frame in TCB so that next thread switch will invoke
 * the indicated notify procedure.
 */
INLINE void tcb_t::notify (void (*func)(word_t, word_t), word_t arg1, word_t arg2)
{
    *(--stack) = (word_t)func;   
    *(--stack) = arg2;
    *(--stack) = arg1;
    *(--stack) = (word_t)notify_prologue;
    
}


/**
 * Short circuit a return path from an IPC system call.  The error
 * code TCR and message registers are already set properly.  The
 * function only needs to restore the appropriate user context and
 * return execution to the instruction directly following the IPC
 * system call.
 */
INLINE void tcb_t::return_from_ipc (void)
{

    register word_t utcb asm("r12") = get_local_id ().get_raw ();

    asm("movq %0, %%rsp\n"
	"retq\n"
	:
	:
	"r" (&get_stack_top ()[KSTACK_RET_IPC]),
	"r" (utcb),
	"d" (get_tag ().raw)
	);	
    
}


/**
 * Short circuit a return path from a user-level interruption or
 * exception.  That is, restore the complete exception context and
 * resume execution at user-level.
 */
INLINE void tcb_t::return_from_user_interruption (void)
{
    asm("movq %0, %%rsp\n"
	"retq\n"
	:
	: "r"(&get_stack_top()[- sizeof(x86_exceptionframe_t)/8 - 1]));    
}



/**********************************************************************
 *
 *                  copy-area related functions
 *
 **********************************************************************/


/**
 * Retrieve the real address associated with a copy area address.
 *
 * @param addr		address within copy area
 *
 * @return address translated into a regular user-level address
 */
INLINE addr_t tcb_t::copy_area_real_address (addr_t addr)
{
  word_t copyarea_num = 
      (((word_t) addr - COPY_AREA_START) >> AMD64_PDP_BITS) /
      (COPY_AREA_SIZE >> AMD64_PDP_BITS);
 
     return addr_offset (resources.copy_area_real_address (copyarea_num),
                         (word_t) addr & (COPY_AREA_SIZE-1));
}

#endif /* !defined(BUILD_TCB_LAYOUT) */

/**********************************************************************
 *
 *                        global tcb functions
 *
 **********************************************************************/


INLINE tcb_t * get_current_tcb()
{
    addr_t stack;
    asm ("leaq -8(%%rsp), %0" :"=r" (stack));
    return (tcb_t *) ((word_t) stack & KTCB_MASK);
    
}


/**********************************************************************
 *
 *                  architecture-specific functions
 *
 **********************************************************************/

/**
 * initialize architecture-dependent root server properties based on
 * values passed via KIP
 * @param space the address space this server will run in   
 * @param ip the initial instruction pointer           
 * @param sp the initial stack pointer
 */
INLINE void tcb_t::arch_init_root_server (space_t * space, word_t ip, word_t sp)
{ 
    space->space_control(sp);
}

#endif /* !__GLUE_V4_X86__X64__TCB_H__ */
