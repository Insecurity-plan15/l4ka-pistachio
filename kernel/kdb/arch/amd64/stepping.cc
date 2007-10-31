/*********************************************************************
 *                
 * Copyright (C) 2002-2003, 2007,  Karlsruhe University
 *                
 * File path:     kdb/arch/amd64/stepping.cc
 * Description:   Single stepping for IA-32
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
 * $Id: stepping.cc,v 1.2 2003/09/24 19:05:04 skoglund Exp $
 *                
 ********************************************************************/


#include <debug.h>	
#include <kdb/kdb.h>
#include INC_ARCH(trapgate.h)	/* for ia32_exceptionframe_t */

DECLARE_CMD (cmd_singlestep, root, 's', "singlestep", "Single step");

CMD(cmd_singlestep, cg)
{
    debug_param_t * param = (debug_param_t*)kdb.kdb_param;
    x86_exceptionframe_t* f = param->frame;
    
    /* set RF and TF in eflags */
    f->rflags |= ((1 << 8) | (1 << 16));
    return CMD_QUIT;
}
