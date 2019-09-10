#ifndef INCFILE1_H_
#define INCFILE1_H_

static const uint8_t VCNL4040_ALS_IT_MASK = (uint8_t)~((1 << 7) | (1 << 6));
static const uint8_t VCNL4040_ALS_IT_80MS = 0;
static const uint8_t VCNL4040_ALS_IT_160MS = (1 << 7);
static const uint8_t VCNL4040_ALS_IT_320MS = (1 << 6);
static const uint8_t VCNL4040_ALS_IT_640MS = (1 << 7) | (1 << 6);

static const uint8_t VCNL4040_ALS_PERS_MASK = (uint8_t)~((1 << 3) | (1 << 2));
static const uint8_t VCNL4040_ALS_PERS_1 = 0;
static const uint8_t VCNL4040_ALS_PERS_2 = (1 << 2);
static const uint8_t VCNL4040_ALS_PERS_4 = (1 << 3);
static const uint8_t VCNL4040_ALS_PERS_8 = (1 << 3) | (1 << 2);

static const uint8_t VCNL4040_ALS_INT_EN_MASK = (uint8_t)~((1 << 1));
static const uint8_t VCNL4040_ALS_INT_DISABLE = 0;
static const uint8_t VCNL4040_ALS_INT_ENABLE = (1 << 1);

static const uint8_t VCNL4040_ALS_SD_MASK = (uint8_t)~((1 << 0));
static const uint8_t VCNL4040_ALS_SD_POWER_ON = 0;
static const uint8_t VCNL4040_ALS_SD_POWER_OFF = (1 << 0);

static const uint8_t VCNL4040_PS_DUTY_MASK = (uint8_t)~((1 << 7) | (1 << 6));
static const uint8_t VCNL4040_PS_DUTY_40 = 0;
static const uint8_t VCNL4040_PS_DUTY_80 = (1 << 6);
static const uint8_t VCNL4040_PS_DUTY_160 = (1 << 7);
static const uint8_t VCNL4040_PS_DUTY_320 = (1 << 7) | (1 << 6);

static const uint8_t VCNL4040_PS_PERS_MASK = (uint8_t)~((1 << 5) | (1 << 4));
static const uint8_t VCNL4040_PS_PERS_1 = 0;
static const uint8_t VCNL4040_PS_PERS_2 = (1 << 4);
static const uint8_t VCNL4040_PS_PERS_3 = (1 << 5);
static const uint8_t VCNL4040_PS_PERS_4 = (1 << 5) | (1 << 4);

static const uint8_t VCNL4040_PS_IT_MASK = (uint8_t)~((1 << 3) | (1 << 2) | (1 << 1));
static const uint8_t VCNL4040_PS_IT_1T = 0;
static const uint8_t VCNL4040_PS_IT_15T = (1 << 1);
static const uint8_t VCNL4040_PS_IT_2T = (1 << 2);
static const uint8_t VCNL4040_PS_IT_25T = (1 << 2) | (1 << 1);
static const uint8_t VCNL4040_PS_IT_3T = (1 << 3);
static const uint8_t VCNL4040_PS_IT_35T = (1 << 3) | (1 << 1);
static const uint8_t VCNL4040_PS_IT_4T = (1 << 3) | (1 << 2);
static const uint8_t VCNL4040_PS_IT_8T = (1 << 3) | (1 << 2) | (1 << 1);

static const uint8_t VCNL4040_PS_SD_MASK = (uint8_t)~((1 << 0));
static const uint8_t VCNL4040_PS_SD_POWER_ON = 0;
static const uint8_t VCNL4040_PS_SD_POWER_OFF = (1 << 0);

static const uint8_t VCNL4040_PS_HD_MASK = (uint8_t)~((1 << 3));
static const uint8_t VCNL4040_PS_HD_12_BIT = 0;
static const uint8_t VCNL4040_PS_HD_16_BIT = (1 << 3);

static const uint8_t VCNL4040_PS_INT_MASK = (uint8_t)~((1 << 1) | (1 << 0));
static const uint8_t VCNL4040_PS_INT_DISABLE = 0;
static const uint8_t VCNL4040_PS_INT_CLOSE = (1 << 0);
static const uint8_t VCNL4040_PS_INT_AWAY = (1 << 1);
static const uint8_t VCNL4040_PS_INT_BOTH = (1 << 1) | (1 << 0);

static const uint8_t VCNL4040_PS_SMART_PERS_MASK = (uint8_t)~((1 << 4));
static const uint8_t VCNL4040_PS_SMART_PERS_DISABLE = 0;
static const uint8_t VCNL4040_PS_SMART_PERS_ENABLE = (1 << 1);

static const uint8_t VCNL4040_PS_AF_MASK = (uint8_t)~((1 << 3));
static const uint8_t VCNL4040_PS_AF_DISABLE = 0;
static const uint8_t VCNL4040_PS_AF_ENABLE = (1 << 3);

static const uint8_t VCNL4040_PS_TRIG_MASK = (uint8_t)~((1 << 3));
static const uint8_t VCNL4040_PS_TRIG_TRIGGER = (1 << 2);

static const uint8_t VCNL4040_WHITE_EN_MASK = (uint8_t)~((1 << 7));
static const uint8_t VCNL4040_WHITE_ENABLE = 0;
static const uint8_t VCNL4040_WHITE_DISABLE = (1 << 7);

static const uint8_t VCNL4040_PS_MS_MASK = (uint8_t)~((1 << 6));
static const uint8_t VCNL4040_PS_MS_DISABLE = 0;
static const uint8_t VCNL4040_PS_MS_ENABLE = (1 << 6);

static const uint8_t VCNL4040_LED_I_MASK = (uint8_t)~((1 << 2) | (1 << 1) | (1 << 0));
static const uint8_t VCNL4040_LED_50MA = 0;
static const uint8_t VCNL4040_LED_75MA = (1 << 0);
static const uint8_t VCNL4040_LED_100MA = (1 << 1);
static const uint8_t VCNL4040_LED_120MA = (1 << 1) | (1 << 0);
static const uint8_t VCNL4040_LED_140MA = (1 << 2);
static const uint8_t VCNL4040_LED_160MA = (1 << 2) | (1 << 0);
static const uint8_t VCNL4040_LED_180MA = (1 << 2) | (1 << 1);
static const uint8_t VCNL4040_LED_200MA = (1 << 2) | (1 << 1) | (1 << 0);

static const uint8_t VCNL4040_INT_FLAG_ALS_LOW = (1 << 5);
static const uint8_t VCNL4040_INT_FLAG_ALS_HIGH = (1 << 4);
static const uint8_t VCNL4040_INT_FLAG_CLOSE = (1 << 1);
static const uint8_t VCNL4040_INT_FLAG_AWAY = (1 << 0);

extern uint16_t proximity_get();
extern void proximity_init();

#endif /* INCFILE1_H_ */
