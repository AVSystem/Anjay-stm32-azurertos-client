/**
  ******************************************************************************
  * @file    plf_config.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines of the application
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
#ifndef PLF_CONFIG_H
#define PLF_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Specific project Includes -------------------------------------------------*/
#if defined(APPLICATION_CONFIG_FILE)
#include APPLICATION_CONFIG_FILE /* First include to overwrite Platform defines */
#endif /* defined(APPLICATION_CONFIG_FILE) */

/* Common projects Includes --------------------------------------------------*/
#include "plf_features.h"
#include "plf_hw_config.h"
#include "plf_sw_config.h"
#include "plf_cellular_config.h"
#include "plf_power_config.h"
#include "plf_thread_config.h"

#ifdef __cplusplus
}
#endif

#endif /* PLF_CONFIG_H */
