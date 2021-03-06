/************************************************************************************
 * arch/arm/src/stm32/stm32_pwm.h
 *
 *   Copyright (C) 2011, 2015 Gregory Nutt. All rights reserved.
 *   Copyright (C) 2015 Omni Hoverboards Inc. All rights reserved.
 *   Authors: Gregory Nutt <gnutt@nuttx.org>
 *            Paul Alexander Patience <paul-a.patience@polymtl.ca>
 *            Mateusz Szafoni <raiden00@railab.me>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

#ifndef __ARCH_ARM_SRC_STM32_STM32_PWM_H
#define __ARCH_ARM_SRC_STM32_STM32_PWM_H

/* The STM32 does not have dedicated PWM hardware.  Rather, pulsed output control
 * is a capabilitiy of the STM32 timers.  The logic in this file implements the
 * lower half of the standard, NuttX PWM interface using the STM32 timers.  That
 * interface is described in include/nuttx/drivers/pwm.h.
 */

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>

#include "chip.h"

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/* Configuration ********************************************************************/
/* Timer devices may be used for different purposes.  One special purpose is
 * to generate modulated outputs for such things as motor control.  If CONFIG_STM32_TIMn
 * is defined then the CONFIG_STM32_TIMn_PWM must also be defined to indicate that
 * timer "n" is intended to be used for pulsed output signal generation.
 */

#ifndef CONFIG_STM32_TIM1
#  undef CONFIG_STM32_TIM1_PWM
#endif
#ifndef CONFIG_STM32_TIM2
#  undef CONFIG_STM32_TIM2_PWM
#endif
#ifndef CONFIG_STM32_TIM3
#  undef CONFIG_STM32_TIM3_PWM
#endif
#ifndef CONFIG_STM32_TIM4
#  undef CONFIG_STM32_TIM4_PWM
#endif
#ifndef CONFIG_STM32_TIM5
#  undef CONFIG_STM32_TIM5_PWM
#endif
#ifndef CONFIG_STM32_TIM8
#  undef CONFIG_STM32_TIM8_PWM
#endif
#ifndef CONFIG_STM32_TIM9
#  undef CONFIG_STM32_TIM9_PWM
#endif
#ifndef CONFIG_STM32_TIM10
#  undef CONFIG_STM32_TIM10_PWM
#endif
#ifndef CONFIG_STM32_TIM11
#  undef CONFIG_STM32_TIM11_PWM
#endif
#ifndef CONFIG_STM32_TIM12
#  undef CONFIG_STM32_TIM12_PWM
#endif
#ifndef CONFIG_STM32_TIM13
#  undef CONFIG_STM32_TIM13_PWM
#endif
#ifndef CONFIG_STM32_TIM14
#  undef CONFIG_STM32_TIM14_PWM
#endif
#ifndef CONFIG_STM32_TIM15
#  undef CONFIG_STM32_TIM15_PWM
#endif
#ifndef CONFIG_STM32_TIM16
#  undef CONFIG_STM32_TIM16_PWM
#endif
#ifndef CONFIG_STM32_TIM17
#  undef CONFIG_STM32_TIM17_PWM
#endif

/* The basic timers (timer 6 and 7) are not capable of generating output pulses */

#undef CONFIG_STM32_TIM6_PWM
#undef CONFIG_STM32_TIM7_PWM

/* Check if PWM support for any channel is enabled. */

#if defined(CONFIG_STM32_TIM1_PWM)  || defined(CONFIG_STM32_TIM2_PWM)  || \
    defined(CONFIG_STM32_TIM3_PWM)  || defined(CONFIG_STM32_TIM4_PWM)  || \
    defined(CONFIG_STM32_TIM5_PWM)  || defined(CONFIG_STM32_TIM8_PWM)  || \
    defined(CONFIG_STM32_TIM9_PWM)  || defined(CONFIG_STM32_TIM10_PWM) || \
    defined(CONFIG_STM32_TIM11_PWM) || defined(CONFIG_STM32_TIM12_PWM) || \
    defined(CONFIG_STM32_TIM13_PWM) || defined(CONFIG_STM32_TIM14_PWM) || \
    defined(CONFIG_STM32_TIM15_PWM) || defined(CONFIG_STM32_TIM16_PWM) || \
    defined(CONFIG_STM32_TIM17_PWM)

#include <arch/board/board.h>
#include "chip/stm32_tim.h"

#ifdef CONFIG_STM32_PWM_MULTICHAN
#  ifndef CONFIG_PWM_MULTICHAN
#    error CONFIG_STM32_PWM_MULTICHAN enabled but CONFIG_PWM_MULTICHAN not set!
#  endif
#endif

#ifdef CONFIG_PWM_MULTICHAN

#ifdef CONFIG_STM32_TIM1_CHANNEL1
#  define PWM_TIM1_CHANNEL1 1
#else
#  define PWM_TIM1_CHANNEL1 0
#endif
#ifdef CONFIG_STM32_TIM1_CHANNEL2
#  define PWM_TIM1_CHANNEL2 1
#else
#  define PWM_TIM1_CHANNEL2 0
#endif
#ifdef CONFIG_STM32_TIM1_CHANNEL3
#  define PWM_TIM1_CHANNEL3 1
#else
#  define PWM_TIM1_CHANNEL3 0
#endif
#ifdef CONFIG_STM32_TIM1_CHANNEL4
#  define PWM_TIM1_CHANNEL4 1
#else
#  define PWM_TIM1_CHANNEL4 0
#endif
#define PWM_TIM1_NCHANNELS (PWM_TIM1_CHANNEL1 + PWM_TIM1_CHANNEL2 + \
                            PWM_TIM1_CHANNEL3 + PWM_TIM1_CHANNEL4)

#ifdef CONFIG_STM32_TIM2_CHANNEL1
#  define PWM_TIM2_CHANNEL1 1
#else
#  define PWM_TIM2_CHANNEL1 0
#endif
#ifdef CONFIG_STM32_TIM2_CHANNEL2
#  define PWM_TIM2_CHANNEL2 1
#else
#  define PWM_TIM2_CHANNEL2 0
#endif
#ifdef CONFIG_STM32_TIM2_CHANNEL3
#  define PWM_TIM2_CHANNEL3 1
#else
#  define PWM_TIM2_CHANNEL3 0
#endif
#ifdef CONFIG_STM32_TIM2_CHANNEL4
#  define PWM_TIM2_CHANNEL4 1
#else
#  define PWM_TIM2_CHANNEL4 0
#endif
#define PWM_TIM2_NCHANNELS (PWM_TIM2_CHANNEL1 + PWM_TIM2_CHANNEL2 + \
                            PWM_TIM2_CHANNEL3 + PWM_TIM2_CHANNEL4)

#ifdef CONFIG_STM32_TIM3_CHANNEL1
#  define PWM_TIM3_CHANNEL1 1
#else
#  define PWM_TIM3_CHANNEL1 0
#endif
#ifdef CONFIG_STM32_TIM3_CHANNEL2
#  define PWM_TIM3_CHANNEL2 1
#else
#  define PWM_TIM3_CHANNEL2 0
#endif
#ifdef CONFIG_STM32_TIM3_CHANNEL3
#  define PWM_TIM3_CHANNEL3 1
#else
#  define PWM_TIM3_CHANNEL3 0
#endif
#ifdef CONFIG_STM32_TIM3_CHANNEL4
#  define PWM_TIM3_CHANNEL4 1
#else
#  define PWM_TIM3_CHANNEL4 0
#endif
#define PWM_TIM3_NCHANNELS (PWM_TIM3_CHANNEL1 + PWM_TIM3_CHANNEL2 + \
                            PWM_TIM3_CHANNEL3 + PWM_TIM3_CHANNEL4)

#ifdef CONFIG_STM32_TIM4_CHANNEL1
#  define PWM_TIM4_CHANNEL1 1
#else
#  define PWM_TIM4_CHANNEL1 0
#endif
#ifdef CONFIG_STM32_TIM4_CHANNEL2
#  define PWM_TIM4_CHANNEL2 1
#else
#  define PWM_TIM4_CHANNEL2 0
#endif
#ifdef CONFIG_STM32_TIM4_CHANNEL3
#  define PWM_TIM4_CHANNEL3 1
#else
#  define PWM_TIM4_CHANNEL3 0
#endif
#ifdef CONFIG_STM32_TIM4_CHANNEL4
#  define PWM_TIM4_CHANNEL4 1
#else
#  define PWM_TIM4_CHANNEL4 0
#endif
#define PWM_TIM4_NCHANNELS (PWM_TIM4_CHANNEL1 + PWM_TIM4_CHANNEL2 + \
                            PWM_TIM4_CHANNEL3 + PWM_TIM4_CHANNEL4)

#ifdef CONFIG_STM32_TIM5_CHANNEL1
#  define PWM_TIM5_CHANNEL1 1
#else
#  define PWM_TIM5_CHANNEL1 0
#endif
#ifdef CONFIG_STM32_TIM5_CHANNEL2
#  define PWM_TIM5_CHANNEL2 1
#else
#  define PWM_TIM5_CHANNEL2 0
#endif
#ifdef CONFIG_STM32_TIM5_CHANNEL3
#  define PWM_TIM5_CHANNEL3 1
#else
#  define PWM_TIM5_CHANNEL3 0
#endif
#ifdef CONFIG_STM32_TIM5_CHANNEL4
#  define PWM_TIM5_CHANNEL4 1
#else
#  define PWM_TIM5_CHANNEL4 0
#endif
#define PWM_TIM5_NCHANNELS (PWM_TIM5_CHANNEL1 + PWM_TIM5_CHANNEL2 + \
                            PWM_TIM5_CHANNEL3 + PWM_TIM5_CHANNEL4)

#ifdef CONFIG_STM32_TIM8_CHANNEL1
#  define PWM_TIM8_CHANNEL1 1
#else
#  define PWM_TIM8_CHANNEL1 0
#endif
#ifdef CONFIG_STM32_TIM8_CHANNEL2
#  define PWM_TIM8_CHANNEL2 1
#else
#  define PWM_TIM8_CHANNEL2 0
#endif
#ifdef CONFIG_STM32_TIM8_CHANNEL3
#  define PWM_TIM8_CHANNEL3 1
#else
#  define PWM_TIM8_CHANNEL3 0
#endif
#ifdef CONFIG_STM32_TIM8_CHANNEL4
#  define PWM_TIM8_CHANNEL4 1
#else
#  define PWM_TIM8_CHANNEL4 0
#endif
#define PWM_TIM8_NCHANNELS (PWM_TIM8_CHANNEL1 + PWM_TIM8_CHANNEL2 + \
                            PWM_TIM8_CHANNEL3 + PWM_TIM8_CHANNEL4)

#ifdef CONFIG_STM32_TIM9_CHANNEL1
#  define PWM_TIM9_CHANNEL1 1
#else
#  define PWM_TIM9_CHANNEL1 0
#endif
#ifdef CONFIG_STM32_TIM9_CHANNEL2
#  define PWM_TIM9_CHANNEL2 1
#else
#  define PWM_TIM9_CHANNEL2 0
#endif
#define PWM_TIM9_NCHANNELS (PWM_TIM9_CHANNEL1 + PWM_TIM9_CHANNEL2)

#ifdef CONFIG_STM32_TIM10_CHANNEL1
#  define PWM_TIM10_CHANNEL1 1
#else
#  define PWM_TIM10_CHANNEL1 0
#endif
#define PWM_TIM10_NCHANNELS (PWM_TIM10_CHANNEL1)

#ifdef CONFIG_STM32_TIM11_CHANNEL1
#  define PWM_TIM11_CHANNEL1 1
#else
#  define PWM_TIM11_CHANNEL1 0
#endif
#define PWM_TIM11_NCHANNELS (PWM_TIM11_CHANNEL1)

#ifdef CONFIG_STM32_TIM12_CHANNEL1
#  define PWM_TIM12_CHANNEL1 1
#else
#  define PWM_TIM12_CHANNEL1 0
#endif
#ifdef CONFIG_STM32_TIM12_CHANNEL2
#  define PWM_TIM12_CHANNEL2 1
#else
#  define PWM_TIM12_CHANNEL2 0
#endif
#define PWM_TIM12_NCHANNELS (PWM_TIM12_CHANNEL1 + PWM_TIM12_CHANNEL2)

#ifdef CONFIG_STM32_TIM13_CHANNEL1
#  define PWM_TIM13_CHANNEL1 1
#else
#  define PWM_TIM13_CHANNEL1 0
#endif
#define PWM_TIM13_NCHANNELS (PWM_TIM13_CHANNEL1)

#ifdef CONFIG_STM32_TIM14_CHANNEL1
#  define PWM_TIM14_CHANNEL1 1
#else
#  define PWM_TIM14_CHANNEL1 0
#endif
#define PWM_TIM14_NCHANNELS (PWM_TIM14_CHANNEL1)

#ifdef CONFIG_STM32_TIM15_CHANNEL1
#  define PWM_TIM15_CHANNEL1 1
#else
#  define PWM_TIM15_CHANNEL1 0
#endif
#ifdef CONFIG_STM32_TIM15_CHANNEL2
#  define PWM_TIM15_CHANNEL2 1
#else
#  define PWM_TIM15_CHANNEL2 0
#endif
#define PWM_TIM15_NCHANNELS (PWM_TIM15_CHANNEL1 + PWM_TIM15_CHANNEL2)

#ifdef CONFIG_STM32_TIM16_CHANNEL1
#  define PWM_TIM16_CHANNEL1 1
#else
#  define PWM_TIM16_CHANNEL1 0
#endif
#define PWM_TIM16_NCHANNELS PWM_TIM16_CHANNEL1

#ifdef CONFIG_STM32_TIM17_CHANNEL1
#  define PWM_TIM17_CHANNEL1 1
#else
#  define PWM_TIM17_CHANNEL1 0
#endif
#define PWM_TIM17_NCHANNELS PWM_TIM17_CHANNEL1

#else  /* !CONFIG_PWM_MULTICHAN */

/* For each timer that is enabled for PWM usage, we need the following additional
 * configuration settings:
 *
 * CONFIG_STM32_TIMx_CHANNEL - Specifies the timer output channel {1,..,4}
 * PWM_TIMx_CHn - One of the values defined in chip/stm32*_pinmap.h.  In the case
 *   where there are multiple pin selections, the correct setting must be provided
 *   in the arch/board/board.h file.
 *
 * NOTE: The STM32 timers are each capable of generating different signals on
 * each of the four channels with different duty cycles.  That capability is
 * not supported by this driver:  Only one output channel per timer.
 */

#ifdef CONFIG_STM32_TIM1_PWM
#  if !defined(CONFIG_STM32_TIM1_CHANNEL)
#    error "CONFIG_STM32_TIM1_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM1_CHANNEL == 1
#    define CONFIG_STM32_TIM1_CHANNEL1 1
#    define CONFIG_STM32_TIM1_CH1MODE  CONFIG_STM32_TIM1_CHMODE
#  elif CONFIG_STM32_TIM1_CHANNEL == 2
#    define CONFIG_STM32_TIM1_CHANNEL2 1
#    define CONFIG_STM32_TIM1_CH2MODE  CONFIG_STM32_TIM1_CHMODE
#  elif CONFIG_STM32_TIM1_CHANNEL == 3
#    define CONFIG_STM32_TIM1_CHANNEL3 1
#    define CONFIG_STM32_TIM1_CH3MODE  CONFIG_STM32_TIM1_CHMODE
#  elif CONFIG_STM32_TIM1_CHANNEL == 4
#    define CONFIG_STM32_TIM1_CHANNEL4 1
#    define CONFIG_STM32_TIM1_CH4MODE  CONFIG_STM32_TIM1_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM1_CHANNEL"
#  endif
#  define PWM_TIM1_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM2_PWM
#  if !defined(CONFIG_STM32_TIM2_CHANNEL)
#    error "CONFIG_STM32_TIM2_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM2_CHANNEL == 1
#    define CONFIG_STM32_TIM2_CHANNEL1 1
#    define CONFIG_STM32_TIM2_CH1MODE  CONFIG_STM32_TIM2_CHMODE
#  elif CONFIG_STM32_TIM2_CHANNEL == 2
#    define CONFIG_STM32_TIM2_CHANNEL2 1
#    define CONFIG_STM32_TIM2_CH2MODE  CONFIG_STM32_TIM2_CHMODE
#  elif CONFIG_STM32_TIM2_CHANNEL == 3
#    define CONFIG_STM32_TIM2_CHANNEL3 1
#    define CONFIG_STM32_TIM2_CH3MODE  CONFIG_STM32_TIM2_CHMODE
#  elif CONFIG_STM32_TIM2_CHANNEL == 4
#    define CONFIG_STM32_TIM2_CHANNEL4 1
#    define CONFIG_STM32_TIM2_CH4MODE  CONFIG_STM32_TIM2_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM2_CHANNEL"
#  endif
#  define PWM_TIM2_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM3_PWM
#  if !defined(CONFIG_STM32_TIM3_CHANNEL)
#    error "CONFIG_STM32_TIM3_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM3_CHANNEL == 1
#    define CONFIG_STM32_TIM3_CHANNEL1 1
#    define CONFIG_STM32_TIM3_CH1MODE  CONFIG_STM32_TIM3_CHMODE
#  elif CONFIG_STM32_TIM3_CHANNEL == 2
#    define CONFIG_STM32_TIM3_CHANNEL2 1
#    define CONFIG_STM32_TIM3_CH2MODE  CONFIG_STM32_TIM3_CHMODE
#  elif CONFIG_STM32_TIM3_CHANNEL == 3
#    define CONFIG_STM32_TIM3_CHANNEL3 1
#    define CONFIG_STM32_TIM3_CH3MODE  CONFIG_STM32_TIM3_CHMODE
#  elif CONFIG_STM32_TIM3_CHANNEL == 4
#    define CONFIG_STM32_TIM3_CHANNEL4 1
#    define CONFIG_STM32_TIM3_CH4MODE  CONFIG_STM32_TIM3_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM3_CHANNEL"
#  endif
#  define PWM_TIM3_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM4_PWM
#  if !defined(CONFIG_STM32_TIM4_CHANNEL)
#    error "CONFIG_STM32_TIM4_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM4_CHANNEL == 1
#    define CONFIG_STM32_TIM4_CHANNEL1 1
#    define CONFIG_STM32_TIM4_CH1MODE  CONFIG_STM32_TIM4_CHMODE
#  elif CONFIG_STM32_TIM4_CHANNEL == 2
#    define CONFIG_STM32_TIM4_CHANNEL2 1
#    define CONFIG_STM32_TIM4_CH2MODE  CONFIG_STM32_TIM4_CHMODE
#  elif CONFIG_STM32_TIM4_CHANNEL == 3
#    define CONFIG_STM32_TIM4_CHANNEL3 1
#    define CONFIG_STM32_TIM4_CH3MODE  CONFIG_STM32_TIM4_CHMODE
#  elif CONFIG_STM32_TIM4_CHANNEL == 4
#    define CONFIG_STM32_TIM4_CHANNEL4 1
#    define CONFIG_STM32_TIM4_CH4MODE  CONFIG_STM32_TIM4_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM4_CHANNEL"
#  endif
#  define PWM_TIM4_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM5_PWM
#  if !defined(CONFIG_STM32_TIM5_CHANNEL)
#    error "CONFIG_STM32_TIM5_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM5_CHANNEL == 1
#    define CONFIG_STM32_TIM5_CHANNEL1 1
#    define CONFIG_STM32_TIM5_CH1MODE  CONFIG_STM32_TIM5_CHMODE
#  elif CONFIG_STM32_TIM5_CHANNEL == 2
#    define CONFIG_STM32_TIM5_CHANNEL2 1
#    define CONFIG_STM32_TIM5_CH2MODE  CONFIG_STM32_TIM5_CHMODE
#  elif CONFIG_STM32_TIM5_CHANNEL == 3
#    define CONFIG_STM32_TIM5_CHANNEL3 1
#    define CONFIG_STM32_TIM5_CH3MODE  CONFIG_STM32_TIM5_CHMODE
#  elif CONFIG_STM32_TIM5_CHANNEL == 4
#    define CONFIG_STM32_TIM5_CHANNEL4 1
#    define CONFIG_STM32_TIM5_CH4MODE  CONFIG_STM32_TIM5_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM5_CHANNEL"
#  endif
#  define PWM_TIM5_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM8_PWM
#  if !defined(CONFIG_STM32_TIM8_CHANNEL)
#    error "CONFIG_STM32_TIM8_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM8_CHANNEL == 1
#    define CONFIG_STM32_TIM8_CHANNEL1 1
#    define CONFIG_STM32_TIM8_CH1MODE  CONFIG_STM32_TIM8_CHMODE
#  elif CONFIG_STM32_TIM8_CHANNEL == 2
#    define CONFIG_STM32_TIM8_CHANNEL2 1
#    define CONFIG_STM32_TIM8_CH2MODE  CONFIG_STM32_TIM8_CHMODE
#  elif CONFIG_STM32_TIM8_CHANNEL == 3
#    define CONFIG_STM32_TIM8_CHANNEL3 1
#    define CONFIG_STM32_TIM8_CH3MODE  CONFIG_STM32_TIM8_CHMODE
#  elif CONFIG_STM32_TIM8_CHANNEL == 4
#    define CONFIG_STM32_TIM8_CHANNEL4 1
#    define CONFIG_STM32_TIM8_CH4MODE  CONFIG_STM32_TIM8_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM8_CHANNEL"
#  endif
#  define PWM_TIM8_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM9_PWM
#  if !defined(CONFIG_STM32_TIM9_CHANNEL)
#    error "CONFIG_STM32_TIM9_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM9_CHANNEL == 1
#    define CONFIG_STM32_TIM9_CHANNEL1 1
#    define CONFIG_STM32_TIM9_CH1MODE  CONFIG_STM32_TIM9_CHMODE
#  elif CONFIG_STM32_TIM9_CHANNEL == 2
#    define CONFIG_STM32_TIM9_CHANNEL2 1
#    define CONFIG_STM32_TIM9_CH2MODE  CONFIG_STM32_TIM9_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM9_CHANNEL"
#  endif
#  define PWM_TIM9_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM10_PWM
#  if !defined(CONFIG_STM32_TIM10_CHANNEL)
#    error "CONFIG_STM32_TIM10_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM10_CHANNEL == 1
#    define CONFIG_STM32_TIM10_CHANNEL1 1
#    define CONFIG_STM32_TIM10_CH1MODE  CONFIG_STM32_TIM10_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM10_CHANNEL"
#  endif
#  define PWM_TIM10_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM11_PWM
#  if !defined(CONFIG_STM32_TIM11_CHANNEL)
#    error "CONFIG_STM32_TIM11_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM11_CHANNEL == 1
#    define CONFIG_STM32_TIM11_CHANNEL1 1
#    define CONFIG_STM32_TIM11_CH1MODE  CONFIG_STM32_TIM11_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM11_CHANNEL"
#  endif
#  define PWM_TIM11_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM12_PWM
#  if !defined(CONFIG_STM32_TIM12_CHANNEL)
#    error "CONFIG_STM32_TIM12_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM12_CHANNEL == 1
#    define CONFIG_STM32_TIM12_CHANNEL1 1
#    define CONFIG_STM32_TIM12_CH1MODE  CONFIG_STM32_TIM12_CHMODE
#  elif CONFIG_STM32_TIM12_CHANNEL == 2
#    define CONFIG_STM32_TIM12_CHANNEL2 1
#    define CONFIG_STM32_TIM12_CH2MODE  CONFIG_STM32_TIM12_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM12_CHANNEL"
#  endif
#  define PWM_TIM12_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM13_PWM
#  if !defined(CONFIG_STM32_TIM13_CHANNEL)
#    error "CONFIG_STM32_TIM13_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM13_CHANNEL == 1
#    define CONFIG_STM32_TIM13_CHANNEL1 1
#    define CONFIG_STM32_TIM13_CH1MODE  CONFIG_STM32_TIM13_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM13_CHANNEL"
#  endif
#  define PWM_TIM13_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM14_PWM
#  if !defined(CONFIG_STM32_TIM14_CHANNEL)
#    error "CONFIG_STM32_TIM14_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM14_CHANNEL == 1
#    define CONFIG_STM32_TIM14_CHANNEL1 1
#    define CONFIG_STM32_TIM14_CH1MODE  CONFIG_STM32_TIM14_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM14_CHANNEL"
#  endif
#  define PWM_TIM14_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM15_PWM
#  if !defined(CONFIG_STM32_TIM15_CHANNEL)
#    error "CONFIG_STM32_TIM15_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM15_CHANNEL == 1
#    define CONFIG_STM32_TIM15_CHANNEL1 1
#    define CONFIG_STM32_TIM15_CH1MODE  CONFIG_STM32_TIM15_CHMODE
#  elif CONFIG_STM32_TIM15_CHANNEL == 2
#    define CONFIG_STM32_TIM15_CHANNEL2 1
#    define CONFIG_STM32_TIM15_CH2MODE  CONFIG_STM32_TIM15_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM15_CHANNEL"
#  endif
#  define PWM_TIM15_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM16_PWM
#  if !defined(CONFIG_STM32_TIM16_CHANNEL)
#    error "CONFIG_STM32_TIM16_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM16_CHANNEL == 1
#    define CONFIG_STM32_TIM16_CHANNEL1 1
#    define CONFIG_STM32_TIM16_CH1MODE  CONFIG_STM32_TIM16_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM16_CHANNEL"
#  endif
#  define PWM_TIM16_NCHANNELS 1
#endif

#ifdef CONFIG_STM32_TIM17_PWM
#  if !defined(CONFIG_STM32_TIM17_CHANNEL)
#    error "CONFIG_STM32_TIM17_CHANNEL must be provided"
#  elif CONFIG_STM32_TIM17_CHANNEL == 1
#    define CONFIG_STM32_TIM17_CHANNEL1 1
#    define CONFIG_STM32_TIM17_CH1MODE  CONFIG_STM32_TIM17_CHMODE
#  else
#    error "Unsupported value of CONFIG_STM32_TIM17_CHANNEL"
#  endif
#  define PWM_TIM17_NCHANNELS 1
#endif

#endif /* CONFIG_PWM_MULTICHAN */

#ifdef CONFIG_STM32_TIM1_CH1OUT
#  define PWM_TIM1_CH1CFG GPIO_TIM1_CH1OUT
#else
#  define PWM_TIM1_CH1CFG 0
#endif
#ifdef CONFIG_STM32_TIM1_CH1NOUT
#  define PWM_TIM1_CH1NCFG GPIO_TIM1_CH1NOUT
#else
#  define PWM_TIM1_CH1NCFG 0
#endif
#ifdef CONFIG_STM32_TIM1_CH2OUT
#  define PWM_TIM1_CH2CFG GPIO_TIM1_CH2OUT
#else
#  define PWM_TIM1_CH2CFG 0
#endif
#ifdef CONFIG_STM32_TIM1_CH2NOUT
#  define PWM_TIM1_CH2NCFG GPIO_TIM1_CH2NOUT
#else
#  define PWM_TIM1_CH2NCFG 0
#endif
#ifdef CONFIG_STM32_TIM1_CH3OUT
#  define PWM_TIM1_CH3CFG GPIO_TIM1_CH3OUT
#else
#  define PWM_TIM1_CH3CFG 0
#endif
#ifdef CONFIG_STM32_TIM1_CH3NOUT
#  define PWM_TIM1_CH3NCFG GPIO_TIM1_CH3NOUT
#else
#  define PWM_TIM1_CH3NCFG 0
#endif
#ifdef CONFIG_STM32_TIM1_CH4OUT
#  define PWM_TIM1_CH4CFG GPIO_TIM1_CH4OUT
#else
#  define PWM_TIM1_CH4CFG 0
#endif

#ifdef CONFIG_STM32_TIM2_CH1OUT
#  define PWM_TIM2_CH1CFG GPIO_TIM2_CH1OUT
#else
#  define PWM_TIM2_CH1CFG 0
#endif
#ifdef CONFIG_STM32_TIM2_CH2OUT
#  define PWM_TIM2_CH2CFG GPIO_TIM2_CH2OUT
#else
#  define PWM_TIM2_CH2CFG 0
#endif
#ifdef CONFIG_STM32_TIM2_CH3OUT
#  define PWM_TIM2_CH3CFG GPIO_TIM2_CH3OUT
#else
#  define PWM_TIM2_CH3CFG 0
#endif
#ifdef CONFIG_STM32_TIM2_CH4OUT
#  define PWM_TIM2_CH4CFG GPIO_TIM2_CH4OUT
#else
#  define PWM_TIM2_CH4CFG 0
#endif

#ifdef CONFIG_STM32_TIM3_CH1OUT
#  define PWM_TIM3_CH1CFG GPIO_TIM3_CH1OUT
#else
#  define PWM_TIM3_CH1CFG 0
#endif
#ifdef CONFIG_STM32_TIM3_CH2OUT
#  define PWM_TIM3_CH2CFG GPIO_TIM3_CH2OUT
#else
#  define PWM_TIM3_CH2CFG 0
#endif
#ifdef CONFIG_STM32_TIM3_CH3OUT
#  define PWM_TIM3_CH3CFG GPIO_TIM3_CH3OUT
#else
#  define PWM_TIM3_CH3CFG 0
#endif
#ifdef CONFIG_STM32_TIM3_CH4OUT
#  define PWM_TIM3_CH4CFG GPIO_TIM3_CH4OUT
#else
#  define PWM_TIM3_CH4CFG 0
#endif

#ifdef CONFIG_STM32_TIM4_CH1OUT
#  define PWM_TIM4_CH1CFG GPIO_TIM4_CH1OUT
#else
#  define PWM_TIM4_CH1CFG 0
#endif
#ifdef CONFIG_STM32_TIM4_CH2OUT
#  define PWM_TIM4_CH2CFG GPIO_TIM4_CH2OUT
#else
#  define PWM_TIM4_CH2CFG 0
#endif
#ifdef CONFIG_STM32_TIM4_CH3OUT
#  define PWM_TIM4_CH3CFG GPIO_TIM4_CH3OUT
#else
#  define PWM_TIM4_CH3CFG 0
#endif
#ifdef CONFIG_STM32_TIM4_CH4OUT
#  define PWM_TIM4_CH4CFG GPIO_TIM4_CH4OUT
#else
#  define PWM_TIM4_CH4CFG 0
#endif

#ifdef CONFIG_STM32_TIM5_CH1OUT
#  define PWM_TIM5_CH1CFG GPIO_TIM5_CH1OUT
#else
#  define PWM_TIM5_CH1CFG 0
#endif
#ifdef CONFIG_STM32_TIM5_CH2OUT
#  define PWM_TIM5_CH2CFG GPIO_TIM5_CH2OUT
#else
#  define PWM_TIM5_CH2CFG 0
#endif
#ifdef CONFIG_STM32_TIM5_CH3OUT
#  define PWM_TIM5_CH3CFG GPIO_TIM5_CH3OUT
#else
#  define PWM_TIM5_CH3CFG 0
#endif
#ifdef CONFIG_STM32_TIM5_CH4OUT
#  define PWM_TIM5_CH4CFG GPIO_TIM5_CH4OUT
#else
#  define PWM_TIM5_CH4CFG 0
#endif

#ifdef CONFIG_STM32_TIM8_CH1OUT
#  define PWM_TIM8_CH1CFG GPIO_TIM8_CH1OUT
#else
#  define PWM_TIM8_CH1CFG 0
#endif
#ifdef CONFIG_STM32_TIM8_CH1NOUT
#  define PWM_TIM8_CH1NCFG GPIO_TIM8_CH1NOUT
#else
#  define PWM_TIM8_CH1NCFG 0
#endif
#ifdef CONFIG_STM32_TIM8_CH2OUT
#  define PWM_TIM8_CH2CFG GPIO_TIM8_CH2OUT
#else
#  define PWM_TIM8_CH2CFG 0
#endif
#ifdef CONFIG_STM32_TIM8_CH2NOUT
#  define PWM_TIM8_CH2NCFG GPIO_TIM8_CH2NOUT
#else
#  define PWM_TIM8_CH2NCFG 0
#endif
#ifdef CONFIG_STM32_TIM8_CH3OUT
#  define PWM_TIM8_CH3CFG GPIO_TIM8_CH3OUT
#else
#  define PWM_TIM8_CH3CFG 0
#endif
#ifdef CONFIG_STM32_TIM8_CH3NOUT
#  define PWM_TIM8_CH3NCFG GPIO_TIM8_CH3NOUT
#else
#  define PWM_TIM8_CH3NCFG 0
#endif
#ifdef CONFIG_STM32_TIM8_CH4OUT
#  define PWM_TIM8_CH4CFG GPIO_TIM8_CH4OUT
#else
#  define PWM_TIM8_CH4CFG 0
#endif

#ifdef CONFIG_STM32_TIM9_CH1OUT
#  define PWM_TIM9_CH1CFG GPIO_TIM9_CH1OUT
#else
#  define PWM_TIM9_CH1CFG 0
#endif

#ifdef CONFIG_STM32_TIM9_CH2OUT
#  define PWM_TIM9_CH2CFG GPIO_TIM9_CH2OUT
#else
#  define PWM_TIM9_CH2CFG 0
#endif

#ifdef CONFIG_STM32_TIM10_CH1OUT
#  define PWM_TIM10_CH1CFG GPIO_TIM10_CH1OUT
#else
#  define PWM_TIM10_CH1CFG 0
#endif

#ifdef CONFIG_STM32_TIM11_CH1OUT
#  define PWM_TIM11_CH1CFG GPIO_TIM11_CH1OUT
#else
#  define PWM_TIM11_CH1CFG 0
#endif

#ifdef CONFIG_STM32_TIM12_CH1OUT
#  define PWM_TIM12_CH1CFG GPIO_TIM12_CH1OUT
#else
#  define PWM_TIM12_CH1CFG 0
#endif
#ifdef CONFIG_STM32_TIM12_CH2OUT
#  define PWM_TIM12_CH2CFG GPIO_TIM12_CH2OUT
#else
#  define PWM_TIM12_CH2CFG 0
#endif

#ifdef CONFIG_STM32_TIM13_CH1OUT
#  define PWM_TIM13_CH1CFG GPIO_TIM13_CH1OUT
#else
#  define PWM_TIM13_CH1CFG 0
#endif

#ifdef CONFIG_STM32_TIM14_CH1OUT
#  define PWM_TIM14_CH1CFG GPIO_TIM14_CH1OUT
#else
#  define PWM_TIM14_CH1CFG 0
#endif

#ifdef CONFIG_STM32_TIM15_CH1OUT
#  define PWM_TIM15_CH1CFG GPIO_TIM15_CH1OUT
#else
#  define PWM_TIM15_CH1CFG 0
#endif

#ifdef CONFIG_STM32_TIM15_CH1NOUT
#  define PWM_TIM15_CH1NCFG GPIO_TIM15_CH1NOUT
#else
#  define PWM_TIM15_CH1NCFG 0
#endif
#ifdef CONFIG_STM32_TIM15_CH2OUT
#  define PWM_TIM15_CH2CFG GPIO_TIM15_CH2OUT
#else
#  define PWM_TIM15_CH2CFG 0
#endif

#ifdef CONFIG_STM32_TIM16_CH1OUT
#  define PWM_TIM16_CH1CFG GPIO_TIM16_CH1OUT
#else
#  define PWM_TIM16_CH1CFG 0
#endif
#ifdef CONFIG_STM32_TIM16_CH1NOUT
#  define PWM_TIM16_CH1NCFG GPIO_TIM16_CH1NOUT
#else
#  define PWM_TIM16_CH1NCFG 0
#endif

#ifdef CONFIG_STM32_TIM17_CH1OUT
#  define PWM_TIM17_CH1CFG GPIO_TIM17_CH1OUT
#else
#  define PWM_TIM17_CH1CFG 0
#endif
#ifdef CONFIG_STM32_TIM17_CH1NOUT
#  define PWM_TIM17_CH1NCFG GPIO_TIM17_CH1NOUT
#else
#  define PWM_TIM17_CH1NCFG 0
#endif


/************************************************************************************
 * Public Types
 ************************************************************************************/

/* Timer mode */

enum stm32_pwm_tim_mode_e
{
  STM32_TIMMODE_COUNTUP   = 0,
  STM32_TIMMODE_COUNTDOWN = 1,
  STM32_TIMMODE_CENTER1   = 2,
  STM32_TIMMODE_CENTER2   = 3,
  STM32_TIMMODE_CENTER3   = 4,
};

/* Timer output polarity */

enum stm32_pwm_pol_e
{
  STM32_POL_POS  = 0,
  STM32_POL_NEG  = 1,
};

/* Timer output IDLE state */

enum stm32_pwm_idle_e
{
  STM32_IDLE_INACTIVE = 0,
  STM32_IDLE_ACTIVE   = 1
};

/* PWM channel mode */

enum stm32_chan_mode_e
{
  STM32_CHANMODE_PWM1        = 0,
  STM32_CHANMODE_PWM2        = 1,
#ifdef HAVE_IP_TIMERS_V2
  STM32_CHANMODE_COMBINED1   = 2,
  STM32_CHANMODE_COMBINED2   = 3,
  STM32_CHANMODE_ASYMMETRIC1 = 4,
  STM32_CHANMODE_ASYMMETRIC2 = 5
#endif
};

/* Timer channel */

enum stm32_chan_e
{
  STM32_CHAN1  = (1 << 0),
  STM32_CHAN1N = (1 << 1),
  STM32_CHAN2  = (1 << 2),
  STM32_CHAN2N = (1 << 3),
  STM32_CHAN3  = (1 << 4),
  STM32_CHAN3N = (1 << 5),
  STM32_CHAN4  = (1 << 6),
  /* No complementary output for CH4 */
#ifdef HAVE_IP_TIMERS_V2
  /* Only available inside micro */

  STM32_CHAN5  = (1 << 7),
  /* 1<<8 reserved */
  STM32_CHAN6  = (1 << 9),
  /* 1<<10 reserved */
#endif
};

#ifdef CONFIG_STM32_PWM_LL_OPS

/*  */
struct pwm_lowerhalf_s;
struct stm32_pwm_ops_s
{
  /* Update CCR register */

  int (*ccr_update)(FAR struct pwm_lowerhalf_s *dev, uint8_t index, uint32_t ccr);

  /* Get CCR register */

  uint32_t (*ccr_get)(FAR struct pwm_lowerhalf_s *dev, uint8_t index);

  /* Update ARR register */

  int (*arr_update)(FAR struct pwm_lowerhalf_s *dev, uint32_t arr);

  /* Get ARR register */

  uint32_t (*arr_get)(FAR struct pwm_lowerhalf_s *dev);

  /* Enable outputs */

  int (*outputs_enable)(FAR struct pwm_lowerhalf_s *dev, uint16_t outputs, bool state);

  /* Software update */

  int (*soft_update)(FAR struct pwm_lowerhalf_s *dev);

  /* PWM configure */

  int (*configure)(FAR struct pwm_lowerhalf_s *dev);

  /* Software break */

  int (*soft_break)(FAR struct pwm_lowerhalf_s *dev, bool state);

#ifdef HAVE_COMPLEMENTARY
  /* Deadtime update */

  int (*dt_update)(FAR struct pwm_lowerhalf_s *dev, uint8_t dt);
#endif
};

#endif

/************************************************************************************
 * Public Data
 ************************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: stm32_pwminitialize
 *
 * Description:
 *   Initialize one timer for use with the upper_level PWM driver.
 *
 * Input Parameters:
 *   timer - A number identifying the timer use.  The number of valid timer
 *     IDs varies with the STM32 MCU and MCU family but is somewhere in
 *     the range of {1,..,17}.
 *
 * Returned Value:
 *   On success, a pointer to the STM32 lower half PWM driver is returned.
 *   NULL is returned on any failure.
 *
 ************************************************************************************/

FAR struct pwm_lowerhalf_s *stm32_pwminitialize(int timer);

/************************************************************************************
 * Name: stm32_pwm_llops_get
 *
 ************************************************************************************/

#ifdef CONFIG_STM32_PWM_LL_OPS
  FAR const struct stm32_pwm_ops_s *stm32_pwm_llops_get(FAR struct pwm_lowerhalf_s *dev);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* CONFIG_STM32_TIMx_PWM */
#endif /* __ARCH_ARM_SRC_STM32_STM32_PWM_H */
