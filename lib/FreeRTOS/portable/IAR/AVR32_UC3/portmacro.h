/*This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief FreeRTOS port header for AVR32 UC3.
 *
 * - Compiler:           IAR EWAVR32
 * - Supported devices:  All AVR32 devices can be used.
 * - AppNote:
 *
 * \author               Atmel Corporation: http://www.atmel.com \n
 *                       Support and FAQ: http://support.atmel.no/
 *
 *****************************************************************************/

/*
    FreeRTOS V7.1.1 - Copyright (C) 2012 Real Time Engineers Ltd.
	

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    ***************************************************************************
     *                                                                       *
     *    Having a problem?  Start by reading the FAQ "My application does   *
     *    not run, what could be wrong?                                      *
     *                                                                       *
     *    http://www.FreeRTOS.org/FAQHelp.html                               *
     *                                                                       *
    ***************************************************************************


    http://www.FreeRTOS.org - Documentation, training, latest information,
    license and contact details.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool.

    Real Time Engineers ltd license FreeRTOS to High Integrity Systems, who sell
    the code with commercial support, indemnification, and middleware, under
    the OpenRTOS brand: http://www.OpenRTOS.com.  High Integrity Systems also
    provide a safety engineered and independently SIL3 certified version under
    the SafeRTOS brand: http://www.SafeRTOS.com.
*/



#ifndef PORTMACRO_H
#define PORTMACRO_H

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */
#include <avr32/io.h>
#include "intc.h"
#include "compiler.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Type definitions. */
#define portCHAR        char
#define portFLOAT       float
#define portDOUBLE      double
#define portLONG        long
#define portSHORT       short
#define portSTACK_TYPE  unsigned portLONG
#define portBASE_TYPE   portLONG

#define TASK_DELAY_MS(x)   ( (x)        /portTICK_RATE_MS )
#define TASK_DELAY_S(x)    ( (x)*1000   /portTICK_RATE_MS )
#define TASK_DELAY_MIN(x)  ( (x)*60*1000/portTICK_RATE_MS )

#define configTICK_TC_IRQ             ATPASTE2(AVR32_TC_IRQ, configTICK_TC_CHANNEL)

#if( configUSE_16_BIT_TICKS == 1 )
  typedef unsigned portSHORT portTickType;
	#define portMAX_DELAY ( portTickType ) 0xffff
#else
  typedef unsigned portLONG portTickType;
	#define portMAX_DELAY ( portTickType ) 0xffffffff
#endif
/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH      ( -1 )
#define portTICK_RATE_MS      ( ( portTickType ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT       4
#define portNOP()             {__asm__ __volatile__ ("nop");}
/*-----------------------------------------------------------*/


/*-----------------------------------------------------------*/

/* INTC-specific. */
#define DISABLE_ALL_EXCEPTIONS()    Disable_global_exception()
#define ENABLE_ALL_EXCEPTIONS()     Enable_global_exception()

#define DISABLE_ALL_INTERRUPTS()    Disable_global_interrupt()
#define ENABLE_ALL_INTERRUPTS()     Enable_global_interrupt()

#define DISABLE_INT_LEVEL(int_lev)  Disable_interrupt_level(int_lev)
#define ENABLE_INT_LEVEL(int_lev)   Enable_interrupt_level(int_lev)


/*
 * Debug trace.
 * Activated if and only if configDBG is nonzero.
 * Prints a formatted string to stdout.
 * The current source file name and line number are output with a colon before
 * the formatted string.
 * A carriage return and a linefeed are appended to the output.
 * stdout is redirected to the USART configured by configDBG_USART.
 * The parameters are the same as for the standard printf function.
 * There is no return value.
 * SHALL NOT BE CALLED FROM WITHIN AN INTERRUPT as fputs and printf use malloc,
 * which is interrupt-unsafe with the current __malloc_lock and __malloc_unlock.
 */
#if configDBG
	#define portDBG_TRACE(...)												\
	{																		\
	  fputs(__FILE__ ":" ASTRINGZ(__LINE__) ": ", stdout);					\
	  printf(__VA_ARGS__);													\
	  fputs("\r\n", stdout);												\
	}
#else
	#define portDBG_TRACE(...)
#endif


/* Critical section management. */
#define portDISABLE_INTERRUPTS()  DISABLE_ALL_INTERRUPTS()
#define portENABLE_INTERRUPTS()   ENABLE_ALL_INTERRUPTS()


extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portENTER_CRITICAL()      vPortEnterCritical();
#define portEXIT_CRITICAL()       vPortExitCritical();


/* Added as there is no such function in FreeRTOS. */
extern void *pvPortRealloc( void *pv, size_t xSize );
/*-----------------------------------------------------------*/


/*=============================================================================================*/

/*
 * Restore Context for cases other than INTi.
 */
#define portRESTORE_CONTEXT()																\
{																							\
  extern volatile unsigned portLONG ulCriticalNesting;										\
  extern volatile void *volatile pxCurrentTCB;												\
																							\
  __asm__ __volatile__ (																	\
    /* Set SP to point to new stack */														\
    "mov     r8, LWRD("ASTRINGZ(pxCurrentTCB)")												\n\t"\
    "orh     r8, HWRD("ASTRINGZ(pxCurrentTCB)")												\n\t"\
    "ld.w    r0, r8[0]																		\n\t"\
    "ld.w    sp, r0[0]																		\n\t"\
																							\
    /* Restore ulCriticalNesting variable */												\
    "ld.w    r0, sp++																		\n\t"\
    "mov     r8, LWRD("ASTRINGZ(ulCriticalNesting)")										\n\t"\
    "orh     r8, HWRD("ASTRINGZ(ulCriticalNesting)")										\n\t"\
    "st.w    r8[0], r0																		\n\t"\
																							\
    /* Restore R0..R7 */																	\
    "ldm     sp++, r0-r7																	\n\t"\
    /* R0-R7 should not be used below this line */											\
    /* Skip PC and SR (will do it at the end) */											\
    "sub     sp, -2*4																		\n\t"\
    /* Restore R8..R12 and LR */															\
    "ldm     sp++, r8-r12, lr																\n\t"\
    /* Restore SR */																		\
    "ld.w    r0, sp[-8*4]																	\n\t" /* R0 is modified, is restored later. */\
    "mtsr    "ASTRINGZ(AVR32_SR)", r0														\n\t"\
    /* Restore r0 */																		\
    "ld.w    r0, sp[-9*4]																	\n\t"\
    /* Restore PC */																		\
    "ld.w    pc, sp[-7*4]" /* Get PC from stack - PC is the 7th register saved */			\
  );																						\
																							\
  /* Force import of global symbols from assembly */										\
  ulCriticalNesting;																		\
  pxCurrentTCB;																				\
}


/*
 * portSAVE_CONTEXT_INT() and portRESTORE_CONTEXT_INT(): for INT0..3 exceptions.
 * portSAVE_CONTEXT_SCALL() and portRESTORE_CONTEXT_SCALL(): for the scall exception.
 *
 * Had to make different versions because registers saved on the system stack
 * are not the same between INT0..3 exceptions and the scall exception.
 */

// Task context stack layout:
  // R8  (*)
  // R9  (*)
  // R10 (*)
  // R11 (*)
  // R12 (*)
  // R14/LR (*)
  // R15/PC (*)
  // SR (*)
  // R0
  // R1
  // R2
  // R3
  // R4
  // R5
  // R6
  // R7
  // ulCriticalNesting
// (*) automatically done for INT0..INT3, but not for SCALL

/*
 * The ISR used for the scheduler tick depends on whether the cooperative or
 * the preemptive scheduler is being used.
 */
#if configUSE_PREEMPTION == 0

/*
 * portSAVE_CONTEXT_OS_INT() for OS Tick exception.
 */
#define portSAVE_CONTEXT_OS_INT()															\
{																							\
  /* Save R0..R7 */																			\
  __asm__ __volatile__ ("stm     --sp, r0-r7");												\
																							\
  /* With the cooperative scheduler, as there is no context switch by interrupt, */			\
  /* there is also no context save. */														\
}

/*
 * portRESTORE_CONTEXT_OS_INT() for Tick exception.
 */
#define portRESTORE_CONTEXT_OS_INT()														\
{																							\
  __asm__ __volatile__ (																	\
    /* Restore R0..R7 */																	\
    "ldm     sp++, r0-r7																	\n\t"\
																							\
    /* With the cooperative scheduler, as there is no context switch by interrupt, */		\
    /* there is also no context restore. */													\
    "rete"																					\
  );																						\
}

#else

/*
 * portSAVE_CONTEXT_OS_INT() for OS Tick exception.
 */
#define portSAVE_CONTEXT_OS_INT()																	\
{																									\
  extern volatile unsigned portLONG ulCriticalNesting;												\
  extern volatile void *volatile pxCurrentTCB;														\
																									\
  /* When we come here */																			\
  /* Registers R8..R12, LR, PC and SR had already been pushed to system stack */					\
																									\
  __asm__ __volatile__ (																			\
    /* Save R0..R7 */																				\
    "stm     --sp, r0-r7																			\n\t"\
																									\
    /* Save ulCriticalNesting variable  - R0 is overwritten */										\
    "mov     r8, LWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "orh     r8, HWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "ld.w    r0, r8[0]																				\n\t"\
    "st.w    --sp, r0																				\n\t"\
																									\
    /* Check if INT0 or higher were being handled (case where the OS tick interrupted another */	\
    /* interrupt handler (which was of a higher priority level but decided to lower its priority */	\
    /* level and allow other lower interrupt level to occur). */									\
    /* In this case we don't want to do a task switch because we don't know what the stack */		\
    /* currently looks like (we don't know what the interrupted interrupt handler was doing). */	\
    /* Saving SP in pxCurrentTCB and then later restoring it (thinking restoring the task) */		\
    /* will just be restoring the interrupt handler, no way!!! */									\
    /* So, since we won't do a vTaskSwitchContext(), it's of no use to save SP. */					\
    "ld.w    r0, sp[9*4]																			\n\t" /* Read SR in stack */\
    "bfextu  r0, r0, 22, 3																			\n\t" /* Extract the mode bits to R0. */\
    "cp.w    r0, 1																					\n\t" /* Compare the mode bits with supervisor mode(b'001) */\
    "brhi    LABEL_INT_SKIP_SAVE_CONTEXT_"ASTRINGZ(__LINE__)"										\n\t"\
																									\
    /* Store SP in the first member of the structure pointed to by pxCurrentTCB */					\
    /* NOTE: we don't enter a critical section here because all interrupt handlers */				\
    /* MUST perform a SAVE_CONTEXT/RESTORE_CONTEXT in the same way as */							\
    /* portSAVE_CONTEXT_OS_INT/port_RESTORE_CONTEXT_OS_INT if they call OS functions. */			\
    /* => all interrupt handlers must use portENTER_SWITCHING_ISR/portEXIT_SWITCHING_ISR. */		\
    "mov     r8, LWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "orh     r8, HWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "ld.w    r0, r8[0]																				\n\t"\
    "st.w    r0[0], sp																				\n"\
																									\
    "LABEL_INT_SKIP_SAVE_CONTEXT_"ASTRINGZ(__LINE__)":"												\
  );																								\
}

/*
 * portRESTORE_CONTEXT_OS_INT() for Tick exception.
 */
#define portRESTORE_CONTEXT_OS_INT()																\
{																									\
  extern volatile unsigned portLONG ulCriticalNesting;												\
  extern volatile void *volatile pxCurrentTCB;														\
																									\
  /* Check if INT0 or higher were being handled (case where the OS tick interrupted another */		\
  /* interrupt handler (which was of a higher priority level but decided to lower its priority */	\
  /* level and allow other lower interrupt level to occur). */										\
  /* In this case we don't want to do a task switch because we don't know what the stack */			\
  /* currently looks like (we don't know what the interrupted interrupt handler was doing). */		\
  /* Saving SP in pxCurrentTCB and then later restoring it (thinking restoring the task) */			\
  /* will just be restoring the interrupt handler, no way!!! */										\
  __asm__ __volatile__ (																			\
    "ld.w    r0, sp[9*4]																			\n\t" /* Read SR in stack */\
    "bfextu  r0, r0, 22, 3																			\n\t" /* Extract the mode bits to R0. */\
    "cp.w    r0, 1																					\n\t" /* Compare the mode bits with supervisor mode(b'001) */\
    "brhi    LABEL_INT_SKIP_RESTORE_CONTEXT_"ASTRINGZ(__LINE__)										\
  );																								\
																									\
  /* Else */																						\
  /* because it is here safe, always call vTaskSwitchContext() since an OS tick occurred. */		\
  /* A critical section has to be used here because vTaskSwitchContext handles FreeRTOS linked lists. */\
  portENTER_CRITICAL();																				\
  vTaskSwitchContext();																				\
  portEXIT_CRITICAL();																				\
																									\
  /* Restore all registers */																		\
																									\
  __asm__ __volatile__ (																			\
    /* Set SP to point to new stack */																\
    "mov     r8, LWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "orh     r8, HWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "ld.w    r0, r8[0]																				\n\t"\
    "ld.w    sp, r0[0]																				\n"\
																									\
    "LABEL_INT_SKIP_RESTORE_CONTEXT_"ASTRINGZ(__LINE__)":											\n\t"\
																									\
    /* Restore ulCriticalNesting variable */														\
    "ld.w    r0, sp++																				\n\t"\
    "mov     r8, LWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "orh     r8, HWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "st.w    r8[0], r0																				\n\t"\
																									\
    /* Restore R0..R7 */																			\
    "ldm     sp++, r0-r7																			\n\t"\
																									\
    /* Now, the stack should be R8..R12, LR, PC and SR */											\
    "rete"																							\
  );																								\
																									\
  /* Force import of global symbols from assembly */												\
  ulCriticalNesting;																				\
  pxCurrentTCB;																						\
}

#endif


/*
 * portSAVE_CONTEXT_SCALL() for SupervisorCALL exception.
 *
 * NOTE: taskYIELD()(== SCALL) MUST NOT be called in a mode > supervisor mode.
 *
 */
#define portSAVE_CONTEXT_SCALL()																	\
{																									\
  extern volatile unsigned portLONG ulCriticalNesting;												\
  extern volatile void *volatile pxCurrentTCB;														\
																									\
  /* Warning: the stack layout after SCALL doesn't match the one after an interrupt. */				\
  /* If SR[M2:M0] == 001 */																			\
  /*    PC and SR are on the stack.  */																\
  /* Else (other modes) */																			\
  /*    Nothing on the stack. */																	\
																									\
  /* WARNING NOTE: the else case cannot happen as it is strictly forbidden to call */				\
  /* vTaskDelay() and vTaskDelayUntil() OS functions (that result in a taskYield()) */				\
  /* in an interrupt|exception handler. */															\
																									\
  __asm__ __volatile__ (																			\
    /* in order to save R0-R7 */																	\
    "sub     sp, 6*4																				\n\t"\
    /* Save R0..R7 */																				\
    "stm     --sp, r0-r7																			\n\t"\
																									\
    /* in order to save R8-R12 and LR */															\
    /* do not use SP if interrupts occurs, SP must be left at bottom of stack */					\
    "sub     r7, sp,-16*4																			\n\t"\
    /* Copy PC and SR in other places in the stack. */												\
    "ld.w    r0, r7[-2*4]																			\n\t" /* Read SR */\
    "st.w    r7[-8*4], r0																			\n\t" /* Copy SR */\
    "ld.w    r0, r7[-1*4]																			\n\t" /* Read PC */\
    "st.w    r7[-7*4], r0																			\n\t" /* Copy PC */\
																									\
    /* Save R8..R12 and LR on the stack. */															\
    "stm     --r7, r8-r12, lr																		\n\t"\
																									\
    /* Arriving here we have the following stack organizations: */									\
    /* R8..R12, LR, PC, SR, R0..R7. */																\
																									\
    /* Now we can finalize the save. */																\
																									\
    /* Save ulCriticalNesting variable  - R0 is overwritten */										\
    "mov     r8, LWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "orh     r8, HWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "ld.w    r0, r8[0]																				\n\t"\
    "st.w    --sp, r0"																				\
  );																								\
																									\
  /* Disable the its which may cause a context switch (i.e. cause a change of */					\
  /* pxCurrentTCB). */																				\
  /* Basically, all accesses to the pxCurrentTCB structure should be put in a */					\
  /* critical section because it is a global structure. */											\
  portENTER_CRITICAL();																				\
																									\
  /* Store SP in the first member of the structure pointed to by pxCurrentTCB */					\
  __asm__ __volatile__ (																			\
    "mov     r8, LWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "orh     r8, HWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "ld.w    r0, r8[0]																				\n\t"\
    "st.w    r0[0], sp"																				\
  );																								\
}

/*
 * portRESTORE_CONTEXT() for SupervisorCALL exception.
 */
#define portRESTORE_CONTEXT_SCALL()																	\
{																									\
  extern volatile unsigned portLONG ulCriticalNesting;												\
  extern volatile void *volatile pxCurrentTCB;														\
																									\
  /* Restore all registers */																		\
																									\
  /* Set SP to point to new stack */																\
  __asm__ __volatile__ (																			\
    "mov     r8, LWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "orh     r8, HWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "ld.w    r0, r8[0]																				\n\t"\
    "ld.w    sp, r0[0]"																				\
  );																								\
																									\
  /* Leave pxCurrentTCB variable access critical section */											\
  portEXIT_CRITICAL();																				\
																									\
  __asm__ __volatile__ (																			\
    /* Restore ulCriticalNesting variable */														\
    "ld.w    r0, sp++																				\n\t"\
    "mov     r8, LWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "orh     r8, HWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "st.w    r8[0], r0																				\n\t"\
																									\
    /* skip PC and SR */																			\
    /* do not use SP if interrupts occurs, SP must be left at bottom of stack */					\
    "sub     r7, sp, -10*4																			\n\t"\
    /* Restore r8-r12 and LR */																		\
    "ldm     r7++, r8-r12, lr																		\n\t"\
																									\
    /* RETS will take care of the extra PC and SR restore. */										\
    /* So, we have to prepare the stack for this. */												\
    "ld.w    r0, r7[-8*4]																			\n\t" /* Read SR */\
    "st.w    r7[-2*4], r0																			\n\t" /* Copy SR */\
    "ld.w    r0, r7[-7*4]																			\n\t" /* Read PC */\
    "st.w    r7[-1*4], r0																			\n\t" /* Copy PC */\
																									\
    /* Restore R0..R7 */																			\
    "ldm     sp++, r0-r7																			\n\t"\
																									\
    "sub     sp, -6*4																				\n\t"\
																									\
    "rets"																							\
  );																								\
																									\
  /* Force import of global symbols from assembly */												\
  ulCriticalNesting;																				\
  pxCurrentTCB;																						\
}


/*
 * The ISR used depends on whether the cooperative or
 * the preemptive scheduler is being used.
 */
#if configUSE_PREEMPTION == 0

/*
 * ISR entry and exit macros.  These are only required if a task switch
 * is required from the ISR.
 */
#define portENTER_SWITCHING_ISR()																	\
{																									\
  /* Save R0..R7 */																					\
  __asm__ __volatile__ ("stm     --sp, r0-r7");														\
																									\
  /* With the cooperative scheduler, as there is no context switch by interrupt, */					\
  /* there is also no context save. */																\
}

/*
 * Input parameter: in R12, boolean. Perform a vTaskSwitchContext() if 1
 */
#define portEXIT_SWITCHING_ISR()																	\
{																									\
  __asm__ __volatile__ (																			\
    /* Restore R0..R7 */																			\
    "ldm     sp++, r0-r7																			\n\t"\
																									\
    /* With the cooperative scheduler, as there is no context switch by interrupt, */				\
    /* there is also no context restore. */															\
    "rete"																							\
  );																								\
}

#else

/*
 * ISR entry and exit macros.  These are only required if a task switch
 * is required from the ISR.
 */
#define portENTER_SWITCHING_ISR()																	\
{																									\
  extern volatile unsigned portLONG ulCriticalNesting;												\
  extern volatile void *volatile pxCurrentTCB;														\
																									\
  /* When we come here */																			\
  /* Registers R8..R12, LR, PC and SR had already been pushed to system stack */					\
																									\
  __asm__ __volatile__ (																			\
    /* Save R0..R7 */																				\
    "stm     --sp, r0-r7																			\n\t"\
																									\
    /* Save ulCriticalNesting variable  - R0 is overwritten */										\
    "mov     r8, LWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "orh     r8, HWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "ld.w    r0, r8[0]																				\n\t"\
    "st.w    --sp, r0																				\n\t"\
																									\
    /* Check if INT0 or higher were being handled (case where the OS tick interrupted another */	\
    /* interrupt handler (which was of a higher priority level but decided to lower its priority */	\
    /* level and allow other lower interrupt level to occur). */									\
    /* In this case we don't want to do a task switch because we don't know what the stack */		\
    /* currently looks like (we don't know what the interrupted interrupt handler was doing). */	\
    /* Saving SP in pxCurrentTCB and then later restoring it (thinking restoring the task) */		\
    /* will just be restoring the interrupt handler, no way!!! */									\
    /* So, since we won't do a vTaskSwitchContext(), it's of no use to save SP. */					\
    "ld.w    r0, sp[9*4]																			\n\t" /* Read SR in stack */\
    "bfextu  r0, r0, 22, 3																			\n\t" /* Extract the mode bits to R0. */\
    "cp.w    r0, 1																					\n\t" /* Compare the mode bits with supervisor mode(b'001) */\
    "brhi    LABEL_ISR_SKIP_SAVE_CONTEXT_"ASTRINGZ(__LINE__)"										\n\t"\
																									\
    /* Store SP in the first member of the structure pointed to by pxCurrentTCB */					\
    "mov     r8, LWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "orh     r8, HWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "ld.w    r0, r8[0]																				\n\t"\
    "st.w    r0[0], sp																				\n"\
																									\
    "LABEL_ISR_SKIP_SAVE_CONTEXT_"ASTRINGZ(__LINE__)":"												\
  );																								\
}


/*
 * Input parameter: in R12, boolean. Perform a vTaskSwitchContext() if 1
 */
#define portEXIT_SWITCHING_ISR()																	\
{																									\
  extern volatile unsigned portLONG ulCriticalNesting;												\
  extern volatile void *volatile pxCurrentTCB;														\
																									\
  __asm__ __volatile__ (																			\
    /* Check if INT0 or higher were being handled (case where the OS tick interrupted another */	\
    /* interrupt handler (which was of a higher priority level but decided to lower its priority */	\
    /* level and allow other lower interrupt level to occur). */									\
    /* In this case it's of no use to switch context and restore a new SP because we purposedly */	\
    /* did not previously save SP in its TCB. */													\
    "ld.w    r0, sp[9*4]																			\n\t" /* Read SR in stack */\
    "bfextu  r0, r0, 22, 3																			\n\t" /* Extract the mode bits to R0. */\
    "cp.w    r0, 1																					\n\t" /* Compare the mode bits with supervisor mode(b'001) */\
    "brhi    LABEL_ISR_SKIP_RESTORE_CONTEXT_"ASTRINGZ(__LINE__)"									\n\t"\
																									\
    /* If a switch is required then we just need to call */											\
    /* vTaskSwitchContext() as the context has already been */										\
    /* saved. */																					\
    "cp.w    r12, 1																					\n\t" /* Check if Switch context is required. */\
    "brne    LABEL_ISR_RESTORE_CONTEXT_"ASTRINGZ(__LINE__)":C"										\
  );																								\
																									\
  /* A critical section has to be used here because vTaskSwitchContext handles FreeRTOS linked lists. */\
  portENTER_CRITICAL();																				\
  vTaskSwitchContext();																				\
  portEXIT_CRITICAL();																				\
																									\
  __asm__ __volatile__ (																			\
    "LABEL_ISR_RESTORE_CONTEXT_"ASTRINGZ(__LINE__)":												\n\t"\
    /* Restore the context of which ever task is now the highest */									\
    /* priority that is ready to run. */															\
																									\
    /* Restore all registers */																		\
																									\
    /* Set SP to point to new stack */																\
    "mov     r8, LWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "orh     r8, HWRD("ASTRINGZ(pxCurrentTCB)")														\n\t"\
    "ld.w    r0, r8[0]																				\n\t"\
    "ld.w    sp, r0[0]																				\n"\
																									\
    "LABEL_ISR_SKIP_RESTORE_CONTEXT_"ASTRINGZ(__LINE__)":											\n\t"\
																									\
    /* Restore ulCriticalNesting variable */														\
    "ld.w    r0, sp++																				\n\t"\
    "mov     r8, LWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "orh     r8, HWRD("ASTRINGZ(ulCriticalNesting)")												\n\t"\
    "st.w    r8[0], r0																				\n\t"\
																									\
    /* Restore R0..R7 */																			\
    "ldm     sp++, r0-r7																			\n\t"\
																									\
    /* Now, the stack should be R8..R12, LR, PC and SR  */											\
    "rete"																							\
  );																								\
																									\
  /* Force import of global symbols from assembly */												\
  ulCriticalNesting;																				\
  pxCurrentTCB;																						\
}

#endif


#define portYIELD()                 {__asm__ __volatile__ ("scall");}

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */