/**
  ******************************************************************************
  * @file      stm32_lock.h
  * @author    STMicroelectronics
  * @brief     STMicroelectronics lock mechanisms
  *
  * @details
  * This implementation supports the following strategies for handling
  * thread-safe locks. The strategy can be explicitly selected by
  * defining <tt>\STM32_THREAD_SAFE_STRATEGY = \<number></tt> in the project.
  * Please look at the '<toolchain/library>_lock_glue.c' file for more details.
  *
  * 1. User defined thread-safe implementation.
  *    User defined solution for handling thread-safety.
  *    <br>
  *    <b>NOTE:</b> The stubs in stm32_lock_user.h needs to be implemented to gain
  *    thread-safety.
  *
  * 2. [<b>DEFAULT</b>] Allow lock usage from interrupts.
  *    This implementation will ensure thread-safety by disabling all interrupts
  *    during e.g. calls to malloc.
  *    <br>
  *    <b>NOTE:</b> Disabling all interrupts creates interrupt latency which
  *    might not be desired for this application!
  *
  * 3. Deny lock usage from interrupts.
  *    This implementation assumes single thread of execution.
  *    <br>
  *    <b>NOTE:</b> Thread-safety dependent functions will enter an infinity loop
  *    if used in interrupt context.
  *
  * 4. Allow lock usage from interrupts. Implemented using FreeRTOS locks.
  *    This implementation will ensure thread-safety by entering RTOS ISR capable
  *    critical sections during e.g. calls to malloc.
  *    By default this implementation supports 2 levels of recursive locking.
  *    Adding additional levels requires 4 bytes per lock per level of RAM.
  *    <br>
  *    <b>NOTE:</b> Interrupts with high priority are not disabled. This implies
  *    that the lock is not thread-safe from high priority interrupts!
  *
  * 5. Deny lock usage from interrupts. Implemented using FreeRTOS locks.
  *    This implementation will ensure thread-safety by suspending all tasks
  *    during e.g. calls to malloc.
  *    <br>
  *    <b>NOTE:</b> Thread-safety dependent functions will enter an infinity loop
  *    if used in interrupt context.
  *
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef __STM32_LOCK_H__
#define __STM32_LOCK_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <cmsis_compiler.h>

/* Function prototypes -------------------------------------------------------*/
void Error_Handler(void);

/* Public macros -------------------------------------------------------------*/
/** Blocks execution */
#define STM32_LOCK_BLOCK()                      \
  do                                            \
  {                                             \
    __disable_irq();                            \
    Error_Handler();                            \
    while (1);                                  \
  } while (0)

/** Blocks execution if argument is NULL */
#define STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(x)    \
  do                                            \
  {                                             \
    if ((x) == NULL)                            \
    {                                           \
      STM32_LOCK_BLOCK();                       \
    }                                           \
  } while (0)

/** Blocks execution if in interrupt context */
#define STM32_LOCK_BLOCK_IF_INTERRUPT_CONTEXT() \
  do                                            \
  {                                             \
    if (__get_IPSR())                           \
    {                                           \
      STM32_LOCK_BLOCK();                       \
    }                                           \
  } while (0)

/** Hide unused parameter warning from compiler */
#define STM32_LOCK_UNUSED(var) (void)var

/** Size of array */
#define STM32_LOCK_ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

/*
 * Allow lock usage from interrupts. Implemented using FreeRTOS locks.
 */

/* Includes ----------------------------------------------------------------*/
#include <tx_port.h>
#include <tx_api.h>

/* Private defines ---------------------------------------------------------*/
/** Initialize members in instance of <code>LockingData_t</code> structure */
#define LOCKING_DATA_INIT { {0, 0}, 0 }
#define STM32_LOCK_MAX_NESTED_LEVELS 2 /**< Max nesting level of interrupts */
typedef struct
{
  uint32_t basepri[STM32_LOCK_MAX_NESTED_LEVELS];
  uint8_t nesting_level;
} LockingData_t;

/* Private macros ----------------------------------------------------------*/
/** Blocks execution if reached max nesting level */
#define STM32_LOCK_ASSERT_VALID_NESTING_LEVEL(lock)                   \
  do                                                                  \
  {                                                                   \
    if (lock->nesting_level >= STM32_LOCK_ARRAY_SIZE(lock->basepri))  \
    {                                                                 \
      STM32_LOCK_BLOCK();                                             \
    }                                                                 \
  } while (0)

/* Private functions -------------------------------------------------------*/

/**
  * @brief Initialize STM32 lock
  * @param lock The lock to init
  */
static inline void stm32_lock_init(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  for (size_t i = 0; i < STM32_LOCK_ARRAY_SIZE(lock->basepri); i++)
  {
    lock->basepri[i] = 0;
  }
  lock->nesting_level = 0;
}

/**
  * @brief Acquire STM32 lock
  * @param lock The lock to acquire
  */
static inline void stm32_lock_acquire(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  STM32_LOCK_ASSERT_VALID_NESTING_LEVEL(lock);
  lock->basepri[lock->nesting_level++] = tx_interrupt_control(TX_INT_DISABLE);
}

/**
  * @brief Release STM32 lock
  * @param lock The lock to release
  */
static inline void stm32_lock_release(LockingData_t *lock)
{
  STM32_LOCK_BLOCK_IF_NULL_ARGUMENT(lock);
  lock->nesting_level--;
  STM32_LOCK_ASSERT_VALID_NESTING_LEVEL(lock);
  tx_interrupt_control(lock->basepri[lock->nesting_level]);
}

#undef STM32_LOCK_ASSERT_VALID_NESTING_LEVEL
#undef STM32_LOCK_MAX_NESTED_LEVELS

#endif /* __STM32_LOCK_H__ */
