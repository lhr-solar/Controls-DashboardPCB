#include "stm32xx_hal.h"

#ifndef CONSTANTS_H
#define CONSTANTS_H

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} GPIO_Pin;

typedef enum {
    PTT_STATE,            // ADC2_IN12 GPIO_PB2
    FDCAN2_RX_Index,            // ADC2_IN11 GPIO_PC1
    IGN_ARR_STATE,        // ADC1_IN15 GPIO_PB0
    IGN_OFF_STATE,        // ADC1_IN1  GPIO_PA0
    REGEN_EN_STATE,       // ADC1_IN8  GPIO_PA7
    NEUTRAL_GEAR_STATE,   // ADC1_IN6  GPIO_PA5
    REV_SW_STATE,         // I2C4_SDA  GPIO_PC7
    CRUISE_EN_STATE,      // I2C4_SCL  GPIO_PC6
    CRUISE_SET_STATE,     // I2C3_SDA  GPIO_PC9
    HAZARD_STATE,         // I2C3_SCL  GPIO_PA8
    FDCAN2_TX_Index,       //SPI2_SCK   GPIO_PB13

    INPUT_STATE_COUNT
} Input_Indexes;

typedef enum {
    AKSHAY_LED,           // UART4_TX  GPIO_PC12
    CONTROLS_HB_LED,      // UART4_RX  GPIO_PD2
    X_LED2,               // SPI2_MISO GPIO_PB14
    INDICATOR_PWM,        // SPI2_MOSI GPIO_PB15
    PH_CANTX_LED,         // SPI3_MISO GPIO_PB4
    PH_CANRX_LED,         // SPI3_NSS  GPIO_PA15
    CARCAN_TX_LED,        // SPI3_MOSI GPIO_PB5
    CARCAN_RX_LED,        // SPI3_SCK  GPIO_PB3

    LED_COUNT
} LED_Indexes;


typedef enum {
    // Inputs
    PTT_STATE_GPIO,
    FDCAN2_RX,
    IGN_ARR_GPIO,
    IGN_OFF_GPIO,
    REGEN_EN_GPIO,
    NEUTRAL_GEAR_GPIO,
    REV_SW_GPIO,
    CRUISE_EN_GPIO,
    CRUISE_SET_GPIO,
    HAZARD_GPIO,
    FDCAN2_TX,

    // LEDs
    AKSHAY_LED_PIN,
    CONTROLS_HB_LED_PIN,
    X_LED2_PIN,
    INDICATOR_PWM_PIN,
    PH_CANTX_LED_PIN,
    PH_CANRX_LED_PIN,
    CARCAN_TX_LED_PIN,
    CARCAN_RX_LED_PIN,

    GPIO_MAP_COUNT
} GPIO_Map_Index;

static const GPIO_Pin gpio_map[GPIO_MAP_COUNT] = {
    //Input Pin Map
    [PTT_STATE_GPIO]          = { GPIOB, GPIO_PIN_2  },
    [FDCAN2_RX]          = { GPIOC, GPIO_PIN_1  },
    [IGN_ARR_GPIO]      = { GPIOB, GPIO_PIN_0  },
    [IGN_OFF_GPIO]      = { GPIOA, GPIO_PIN_0  },
    [REGEN_EN_GPIO]     = { GPIOA, GPIO_PIN_7  },
    [NEUTRAL_GEAR_GPIO] = { GPIOA, GPIO_PIN_5  },
    [REV_SW_GPIO]       = { GPIOC, GPIO_PIN_7  },
    [CRUISE_EN_GPIO]    = { GPIOC, GPIO_PIN_6  },
    [CRUISE_SET_GPIO]   = { GPIOC, GPIO_PIN_9  },
    [HAZARD_GPIO]       = { GPIOA, GPIO_PIN_8  },
    [FDCAN2_TX]         = { GPIOB, GPIO_PIN_3  },

    //LED Pin map
    [AKSHAY_LED_PIN]         = { GPIOC, GPIO_PIN_12 },
    [CONTROLS_HB_LED_PIN]    = { GPIOD, GPIO_PIN_2  },
    [X_LED2_PIN]             = { GPIOB, GPIO_PIN_14 },
    [INDICATOR_PWM_PIN]      = { GPIOB, GPIO_PIN_15 },
    [PH_CANTX_LED_PIN]       = { GPIOB, GPIO_PIN_4  },
    [PH_CANRX_LED_PIN]       = { GPIOA, GPIO_PIN_15 },
    [CARCAN_TX_LED_PIN]      = { GPIOB, GPIO_PIN_5  },
    [CARCAN_RX_LED_PIN]      = { GPIOB, GPIO_PIN_3  }
};



#endif