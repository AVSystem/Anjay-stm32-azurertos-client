/**
  ******************************************************************************
  * @file    com_utils.h
  * @author  MCD Application Team
  * @brief   Header for com_utils.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef COM_UTILS_H
#define COM_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes -----------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>

/** @addtogroup COM
  * @{
  */

/** @defgroup COM_UTILS Utilities
  * @{
  */


/* Exported constants --------------------------------------------------------*/
/* None */
/* Exported types ------------------------------------------------------------*/
/* None */

/* External variables --------------------------------------------------------*/
/* None */

/* Exported macros -----------------------------------------------------------*/
/* None */

/* Exported functions ------------------------------------------------------- */

/** @defgroup COM_UTILS_Functions Functions
  * @{
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup COM_UTILS_Functions
  * @{
  */


/*** Utils functionalities ****************************************************/
/**
  * @brief  Convert two Hexadecimal byte to one Char byte
  * @note   example: msd:0x31 lsd:0x32 result = true *p_conv=0x12
  * @param  msd    - msd part possible value [0,9],[a,f],[A,F]
  * @param  lsd    - lsd part possible value [0,9],[a,f],[A,F]
  * @param  p_conv - conversion result pointer
  * @retval true/false - conversion OK/NOK
  */
bool com_utils_convertHEXToChar(uint8_t msd, uint8_t lsd, uint8_t *p_conv);
/**
  * @brief  Convert one Char byte to two Hexadecimal
  * @note   example: val:0x12 result = true, *p_msd:0x31 *p_lsd:0x32
  * @param  val    - value to convert
  * @param  p_msd  - msd part possible value [0,9],[a,f]
  * @param  p_lsd  - lsd part possible value [0,9],[a,f]
  * @retval true/false - conversion OK/NOK
  */
bool com_utils_convertCharToHEX(uint8_t val, uint8_t *p_msd, uint8_t *p_lsd);

/**
  * @}
  */

/*** Component Initialization/Start *******************************************/
/*** Used by com_core module - Not an User Interface **************************/


#ifdef __cplusplus
}
#endif

#endif /* COM_UTILS_H */
