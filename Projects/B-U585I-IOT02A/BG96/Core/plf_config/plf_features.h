/**
  ******************************************************************************
  * @file    plf_features.h
  * @author  MCD Application Team
  * @brief   Includes feature list to include in firmware
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
#ifndef PLF_FEATURES_H
#define PLF_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#if (defined APPLICATION_CONFIG_FILE)
#include APPLICATION_CONFIG_FILE /* First include to overwrite Platform defines */
#endif /* defined APPLICATION_CONFIG_FILE */

/* Exported constants --------------------------------------------------------*/

/* ===================================== */
/* BEGIN - Cellular data mode            */
/* ===================================== */

/* Possible values for USE_SOCKETS_TYPE */
#define USE_SOCKETS_LWIP          (0)  /* define value affected to LwIP sockets type  */
#define USE_SOCKETS_MODEM         (1)  /* define value affected to Modem sockets type */

/* Sockets location */
#if !defined USE_SOCKETS_TYPE
#define USE_SOCKETS_TYPE          (USE_SOCKETS_MODEM) /* Possible values: USE_SOCKETS_LWIP or USE_SOCKETS_MODEM */
#endif /* !defined USE_SOCKETS_TYPE */

/* ===================================== */
/* END - Cellular data mode              */
/* ===================================== */

/* ======================================= */
/* BEGIN -  Miscellaneous functionalities  */
/* ======================================= */

/* To configure some parameters of the software */
#if !defined USE_CMD_CONSOLE
#define USE_CMD_CONSOLE           (1) /* 0: not activated, 1: activated */
#endif /* !defined USE_CMD_CONSOLE */

/* If included then com_sockets interfaces are defined in com module
 * If USE_SOCKETS_TYPE = USE_SOCKETS_LWIP and USE_COM_SOCKETS = 0, then com_sockets is not included and
 * for example another IP stack than LwIP can be used for sockets services
 */
#if !defined USE_COM_SOCKETS
#define USE_COM_SOCKETS           (1)  /* 0: not included, 1: included */
#endif /* !defined USE_COM_SOCKETS */

/* If included then com_ping interfaces are defined in com module
 * to use it USE_COM_SOCKETS must also be set to 1 */
#if !defined USE_COM_PING
#if (USE_COM_SOCKETS == 1)
#define USE_COM_PING              (1)  /* 0: not included, 1: included */
#else  /* USE_COM_SOCKETS == 0 */
#define USE_COM_PING              (0)  /* only possible value: 0: not included */
#endif /* USE_COM_SOCKETS == 1 */
#endif /* !defined USE_COM_PING */

/* If included then com_icc interfaces are defined in com module */
#if !defined USE_COM_ICC
#define USE_COM_ICC               (1)  /* 0: not included, 1: included */
#endif /* !defined USE_COM_ICC */

/* ======================================= */
/* END   -  Miscellaneous functionalities  */
/* ======================================= */

/* Exported types ------------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif /* PLF_FEATURES_H */
