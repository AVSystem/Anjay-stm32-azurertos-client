/**
  ******************************************************************************
  * @file    com_trace.h
  * @author  MCD Application Team
  * @brief   COM trace module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef COM_TRACE_H
#define COM_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "plf_config.h"

/* PRINT FORCE must always be displayed whatever the configuration */
#if (USE_PRINTF == 0U)
#include "trace_interface.h"
#define PRINT_FORCE(format, args...)     \
  TRACE_PRINT_FORCE(DBG_CHAN_COMLIB, DBL_LVL_P0, format "\n\r", ## args)
#else /* USE_PRINTF == 1U */
#include <stdio.h>
#define PRINT_FORCE(format, args...)     (void)printf(format "\n\r", ## args);
#endif /* USE_PRINTF == 0U */

/* Optional Trace definition */
#if (USE_TRACE_COMLIB == 1U)

#if (USE_PRINTF == 0U)
#define PRINT_DBG(format, args...) \
  TRACE_PRINT(DBG_CHAN_COMLIB, DBL_LVL_P1, "ComLib: " format "\n\r", ## args)
#define PRINT_INFO(format, args...) \
  TRACE_PRINT(DBG_CHAN_COMLIB, DBL_LVL_P0, "ComLib: " format "\n\r", ## args)
#define PRINT_ERR(format, args...) \
  TRACE_PRINT(DBG_CHAN_COMLIB, DBL_LVL_ERR, "ComLib ERROR: " format "\n\r", ## args)
#else /* USE_PRINTF == 1U */
/* To reduce trace PRINT_DBG is deactivated when using printf */
#define PRINT_DBG(...)                   __NOP(); /* Nothing to do */
/*#define PRINT_DBG(format, args...)       (void)printf("ComLib: " format "\n\r", ## args);*/
#define PRINT_INFO(format, args...)      (void)printf("ComLib: " format "\n\r", ## args);
#define PRINT_ERR(format, args...)       (void)printf("ComLib ERROR: " format "\n\r", ## args);
#endif /* USE_PRINTF == 0U */

#else /* USE_TRACE_COMLIB == 0U */
/* No trace, deactivate all */
#define PRINT_DBG(...)                   __NOP(); /* Nothing to do */
#define PRINT_INFO(...)                  __NOP(); /* Nothing to do */
#define PRINT_ERR(...)                   __NOP(); /* Nothing to do */

#endif /* USE_TRACE_COMLIB == 1U */


#ifdef __cplusplus
}
#endif

#endif /* USE_COMLIB_TRACE_H */
