#include <plf_config.h>
#include <ipc_uart.h>
#include <at_modem_api.h>

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == MODEM_UART_INSTANCE)
  {
    IPC_UART_RxCpltCallback(huart);
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == MODEM_UART_INSTANCE)
  {
    IPC_UART_TxCpltCallback(huart);
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == MODEM_UART_INSTANCE)
  {
    IPC_UART_ErrorCallback(huart);
  }
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == MODEM_RING_PIN) {
    GPIO_PinState gstate;
    gstate = HAL_GPIO_ReadPin(MODEM_RING_GPIO_PORT, MODEM_RING_PIN);
    atcc_hw_event(DEVTYPE_MODEM_CELLULAR, HWEVT_MODEM_RING, gstate);
  }
}

