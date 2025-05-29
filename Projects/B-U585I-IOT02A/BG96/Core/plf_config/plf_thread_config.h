/**
  ******************************************************************************
  * @file    plf_thread_config.h
  * @author  MCD Application Team
  * @brief   This file contains thread configuration
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the ST_LICENSE file
  * in the root directory of this software component.
  * If no ST_LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PLF_THREAD_CONFIG_H
#define PLF_THREAD_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "plf_features.h"
#if defined(APPLICATION_THREAD_CONFIG_FILE)
#include APPLICATION_THREAD_CONFIG_FILE
#endif /* defined(APPLICATION_THREAD_CONFIG_FILE) */

/* Exported constants --------------------------------------------------------*/

/* ========================*/
/* BEGIN - Stack Priority  */
/* ========================*/
#define ATCORE_THREAD_STACK_PRIO           osPriorityNormal
#define CELLULAR_SERVICE_THREAD_PRIO       osPriorityNormal

#if (USE_SOCKETS_TYPE == USE_SOCKETS_LWIP)
#define TCPIP_THREAD_PRIO                  osPriorityBelowNormal
#define PPPOSIF_CLIENT_THREAD_PRIO         osPriorityHigh
#endif /* USE_SOCKETS_TYPE == USE_SOCKETS_LWIP */

#if (USE_CMD_CONSOLE == 1)
#define CMD_THREAD_PRIO                    osPriorityBelowNormal
#endif /* USE_CMD_CONSOLE == 1 */

/* ========================*/
/* END - Stack Priority    */
/* ========================*/

/* =====================================*/
/* BEGIN - Stack Size and Thread Number */
/* =====================================*/
#define ATCORE_THREAD_STACK_SIZE            (384U)
#define ATCORE_THREAD_NB                    (1U)

#define CELLULAR_SERVICE_THREAD_STACK_SIZE  (2048U)
#define CELLULAR_SERVICE_THREAD_NB          (1U)

#if (USE_SOCKETS_TYPE == USE_SOCKETS_LWIP)
#define TCPIP_THREAD_STACK_SIZE             (512U)
#define PPPOSIF_CLIENT_THREAD_STACK_SIZE    (640U)
#define TCPIP_THREAD_NB                     (1U)
#define PPPOSIF_CLIENT_THREAD_NB            (1U)
#else
#define TCPIP_THREAD_STACK_SIZE             (0U)
#define PPPOSIF_CLIENT_THREAD_STACK_SIZE    (0U)
#define TCPIP_THREAD_NB                     (0U)
#define PPPOSIF_CLIENT_THREAD_NB            (0U)
#endif /* USE_SOCKETS_TYPE == USE_SOCKETS_LWIP */

#if (USE_CMD_CONSOLE == 1)
#if !defined CMD_THREAD_STACK_SIZE
#define CMD_THREAD_STACK_SIZE               (600U)
#endif /* !defined CMD_THREAD_STACK_SIZE */
#define CMD_THREAD_NB                       (1U)
#else  /* USE_CMD_CONSOLE == 0 */
#define CMD_THREAD_STACK_SIZE               (0U)
#define CMD_THREAD_NB                       (0U)
#endif /* USE_CMD_CONSOLE == 1 */

#if !defined APPLICATION_THREAD_STACK_SIZE
#define APPLICATION_THREAD_STACK_SIZE       (0U)
#endif /* !defined APPLICATION_THREAD_STACK_SIZE */

#if !defined APPLICATION_THREAD_NB
#define APPLICATION_THREAD_NB               (0U)
#endif /* !defined APPLICATION_THREAD_NB */

#if !defined APPLICATION_PARTIAL_HEAP_SIZE
#define APPLICATION_PARTIAL_HEAP_SIZE       (0U)
#endif /* !defined APPLICATION_PARTIAL_HEAP_SIZE */

/* =====================================*/
/* END -   Stack Size and Thread Number */
/* =====================================*/

/* ============================================*/
/* BEGIN - Total Stack Size/Number Calculation */
/* ============================================*/

#define CELLULAR_THREAD_STACK_SIZE                 \
  (size_t)( ATCORE_THREAD_STACK_SIZE               \
            +CELLULAR_SERVICE_THREAD_STACK_SIZE    \
            +CMD_THREAD_STACK_SIZE                 \
            +TCPIP_THREAD_STACK_SIZE               \
            +PPPOSIF_CLIENT_THREAD_STACK_SIZE      )

#define CELLULAR_THREAD_NUMBER                     \
  (uint8_t)( ATCORE_THREAD_NB                      \
             +CELLULAR_SERVICE_THREAD_NB           \
             +CMD_THREAD_NB                        \
             +TCPIP_THREAD_NB                      \
             +PPPOSIF_CLIENT_THREAD_NB             )

/*
 * Partial Heap used for: RTOS Timer/Mutex/Semaphore/Message objects and extra pvPortMalloc call
 * Mutex/Semaphore # 88 bytes
 * Queue           # 96 bytes + (max nb of elements * sizeof(uint32_t))
 * Thread          #104 bytes
 * Timer           # 56 bytes
 */
#define CELLULAR_PARTIAL_HEAP_SIZE   (CELLULAR_THREAD_NUMBER * 800U)


/* Total Heap defined used by RTOS to allocate the heap */
#define TOTAL_HEAP_SIZE              (  (size_t)(CELLULAR_THREAD_STACK_SIZE    * 4U)   \
                                        + (size_t)(APPLICATION_THREAD_STACK_SIZE * 4U) \
                                        + (size_t)(CELLULAR_PARTIAL_HEAP_SIZE)         \
                                        + (size_t)(APPLICATION_PARTIAL_HEAP_SIZE))

/* ============================================*/
/* END - Total Stack Size/Number Calculation   */
/* ============================================*/

/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#ifdef __cplusplus
}
#endif

#endif /* PLF_THREAD_CONFIG_H */
