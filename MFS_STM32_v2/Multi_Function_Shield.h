/*

MFS		ARDUINO		STM32
-------------------------------
BTN1	A1			PA1
BTN2	A2			PA4
BTN3	A3			PB0

LED1	D13			PA5
LED2	D12			PA6
LED3	D11			PA7
LED4	D10			PB6

*/


// File: Multi_Function_Shield.h
/*Define to prevent recursive inclusion-------------------------------------*/
#ifndef __MULTI_FUNCTION_SHIEL_H__
#define __MULTI_FUNCTION_SHIEL_H__



#define STM32F1
//#define STM32F0
//#define STM32F4
//#define STM32F7
//#define STM32H7
//#define STM32L0
//#define STM32L4

#if defined(STM32F0)
#include "stm32f0xx_hal.h"
#elif defined(STM32F1)
#include "stm32f1xx_hal.h"
#elif defined(STM32F4)
#include "stm32f4xx_hal.h"
#elif defined(STM32L0)
#include "stm32l0xx_hal.h"
#elif defined(STM32L4)
#include "stm32l4xx_hal.h"
#elif defined(STM32F3)
#include "stm32f3xx_hal.h"
#elif defined(STM32H7)
#include "stm32h7xx_hal.h"
#elif defined(STM32F7)
#include "stm32f7xx_hal.h"
#else
 #error "MULTI FUNCTION SHIEL library was tested only on STM32F1, STM32F3, STM32F4, STM32F7, STM32L0, STM32L4, STM32H7 MCU families. Please modify ssd1306.h if you know what you are doing. Also please send a pull request if it turns out the library works on other MCU's as well!"
#endif


/*=======================NUCLEO64-103RB Arduino Header Pin assignment begin=======================*/


// A0 ~ A5
#define A0_Port GPIOA
#define A1_Port GPIOA
#define A2_Port GPIOA
#define A3_Port GPIOB
#define A4_Port GPIOC
#define A5_Port GPIOC
//
#define A0_Pin GPIO_PIN_0
#define A1_Pin GPIO_PIN_1
#define A2_Pin GPIO_PIN_2
#define A3_Pin GPIO_PIN_3
#define A4_Pin GPIO_PIN_1
#define A5_Pin GPIO_PIN_0


// D0 ~ D5
#define D0_Port GPIOA
#define D1_Port GPIOA
#define D2_Port GPIOA
#define D3_Port GPIOB
#define D4_Port GPIOB
#define D5_Port GPIOB
//
#define D0_Pin GPIO_PIN_3
#define D1_Pin GPIO_PIN_2
#define D2_Pin GPIO_PIN_10
#define D3_Pin GPIO_PIN_3
#define D4_Pin GPIO_PIN_5
#define D5_Pin GPIO_PIN_4


#define D6_Port GPIOB
#define D7_Port GPIOA
#define D8_Port GPIOA
#define D9_Port GPIOC
#define D10_Port GPIOB
#define D11_Port GPIOA
#define D12_Port GPIOA
#define D13_Port GPIOA
//
#define D6_Pin GPIO_PIN_10
#define D7_Pin GPIO_PIN_8
#define D8_Pin GPIO_PIN_9
#define D9_Pin GPIO_PIN_7
#define D10_Pin GPIO_PIN_6
#define D11_Pin GPIO_PIN_7
#define D12_Pin GPIO_PIN_6
#define D13_Pin GPIO_PIN_5


/*=======================NUCLEO64-103RB Arduino Header Pin assignment end=======================*/


/*=======================MuitiFunction Shield begin=======================*/
//BTN
#define BTN1_Port A1_Port
#define BTN2_Port A2_Port
#define BTN3_Port A3_Port
//
#define BTN1_Pin A1_Pin
#define BTN2_Pin A2_Pin
#define BTN3_Pin A3_Pin


//FND
#define LCH_Port D4_Port
#define SFT_Port D7_Port
#define SDI_Port D8_Port
//
#define LCH_Pin D4_Pin//#define LATCH_PIN     4
#define SFT_Pin D7_Pin//#define CLK_PIN       7
#define SDI_Pin D8_Pin//#define DATA_PIN      8


//LED
#define LED4_Port D10_Port
#define LED3_Port D11_Port
#define LED2_Port D12_Port
#define LED1_Port D13_Port
//
#define LED4_Pin D10_Pin
#define LED3_Pin D11_Pin
#define LED2_Pin D12_Pin
#define LED1_Pin D13_Pin


//else
#define LM35_Pin A4_Pin
#define POTION_Pin D0_Pin
#define BEEPER_Pin D3_Pin
//
#define LM35_Port A4_Port
#define POTION_Port D0_Port
#define BEEPER_Port D3_Port

/*=======================MuitiFunction Shield end=======================*/
void fndbuf_set(uint8_t data);
void FND_hex(long da,char start,char len);
void FND_bcd(long da,char start,char len);
void FND_Disp(void);


void MFS_LED_Toggle(uint8_t LED_num);
void MFS_LED_Write(uint8_t LED_num, uint8_t pin_state);
uint8_t MFS_BTN_Read(uint8_t BTN_num);

#endif
