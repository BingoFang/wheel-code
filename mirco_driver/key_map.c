/* 面向对象处理按键获取键值，包含机械按键和触摸芯片 */
typedef struct
{
    uint16 key;
    uint16 keyValue;
} key_map_t;

typedef struct
{
    bool (*init)(void);
    bool (*poll)(uint8 *pdata);
    const key_map_t *touch_key_map;
} touchKey_t;


//DNAKE硬件 按键映射配置
const key_map_t key_map[BSP_NUM_KEYS][BSP_NUM_KEYS] =
{
    {
        {PRESS1_VAL, 0},
        {PRESS2_VAL, 0},
        {PRESS3_VAL, 0},
        {PRESS4_VAL, 0},
        {PRESS5_VAL, 0},
        {PRESS6_VAL, 0},
    },
    {
        {PRESS1_VAL, 0},
        {PRESS2_VAL, 0},
        {PRESS3_VAL, 0},
        {PRESS4_VAL, 0},
        {PRESS5_VAL, 0},
        {PRESS6_VAL, 0},
    },
    {
        {PRESS1_VAL, HAL_KEY_SW_1},
        {PRESS2_VAL, HAL_KEY_SW_2},
        {PRESS3_VAL, HAL_KEY_SW_3},
        {PRESS4_VAL, 0},
        {PRESS5_VAL, 0},
        {PRESS6_VAL, 0},
    },
    {
        {PRESS1_VAL, 0},
        {PRESS2_VAL, 0},
        {PRESS3_VAL, 0},
        {PRESS4_VAL, 0},
        {PRESS5_VAL, 0},
        {PRESS6_VAL, 0},
    },
    {
        {PRESS1_VAL, 0},
        {PRESS2_VAL, 0},
        {PRESS3_VAL, 0},
        {PRESS4_VAL, 0},
        {PRESS5_VAL, 0},
        {PRESS6_VAL, 0},
    },
    {
        {PRESS1_VAL, HAL_KEY_SW_1},
        {PRESS2_VAL, HAL_KEY_SW_2},
        {PRESS3_VAL, HAL_KEY_SW_3},
        {PRESS4_VAL, HAL_KEY_SW_4},
        {PRESS5_VAL, HAL_KEY_SW_5},
        {PRESS6_VAL, HAL_KEY_SW_6},
    },
};

const key_map_t *pkey_map = key_map[5];

const key_map_t cy8_key_map[8] =
{
    {TOUCH1_VAL, PRESS1_VAL},
    {TOUCH2_VAL, PRESS2_VAL},
    {TOUCH3_VAL, PRESS3_VAL},
    {TOUCH4_VAL, PRESS4_VAL},
    {TOUCH5_VAL, PRESS5_VAL},
    {TOUCH6_VAL, PRESS6_VAL},
    {0, 0},
    {0, 0},
};

const key_map_t gt316l_key_map[8] =
{
    {0x01, PRESS1_VAL},
    {0x02, PRESS2_VAL},
    {0x04, PRESS3_VAL},
    {0x08, PRESS4_VAL},
    {0x10, PRESS5_VAL},
    {0x20, PRESS6_VAL},
    {0, 0},
    {0, 0},
};

const key_map_t cp2532_key_map[8] =
{
    {0x01, PRESS1_VAL},
    {0x02, PRESS2_VAL},
    {0x04, PRESS3_VAL},
    {0x08, PRESS4_VAL},
    {0x10, PRESS5_VAL},
    {0x20, PRESS6_VAL},
    {0, 0},
    {0, 0},
};

const touchKey_t cy8 =
{
    CY8CMBR3xxx_Init,
    CY8CMBR3xxx_GetTouchKeyValue,
    cy8_key_map,
};

const touchKey_t gt316l =
{
    GT316L_Init,
    GT316L_GetTouchKeyValue,
    gt316l_key_map,
};

const touchKey_t cp2528 =
{
    CP2532_Init,
    CP2532_GetTouchKeyValue,
    cp2532_key_map,
};

const touchKey_t *touchKeyArray[] =
{
    &cy8,
    &gt316l,
    &cp2528
};

const uint8_t touchKeyCount = sizeof(touchKeyArray) / sizeof(touchKeyArray[0]);

const touchKey_t *touchKey = NULL;

/* 触摸芯片寄存器对应按键值 */
uint8 regValueToKeyValue(uint8 regValue)
{
    uint8 keyValue = 0;

    for (uint8 i = 0; i < 8; i++)
    {
        if (touchKey->touch_key_map[i].bspKeyValue & regValue)
        {
            keyValue |= touchKey->touch_key_map[i].halKeyValue;
        }
    }

    return keyValue;
}

/* 获取板载上触摸芯片初始化，获得keyValue */
uint8 bspKeyGetTouchKeyValue(void)
{
    uint8 keyValue = 0;
    uint8 regValue = 0;

    if (touchKey)
    {
        if (!touchKey->poll(&regValue))
        return 0;
        keyValue = regValueToKeyValue(regValue);
    }
    else
    {
        for (uint8_t i = 0; i < touchKeyCount; i++)
        {
            if (touchKeyArray[i]->init())
            {
                touchKey = touchKeyArray[i];
                return 0;
            }
        }
    }

    return keyValue;
}

uint16_t bspKeyGetValue(void)
{
    uint16 halKeyValue = 0;
    uint16 bspKeyValue = bspKeyGetTouchKeyValue(); /* 获取触摸芯片返回按键值 */
    bspKeyValue |= bspKeyGetPressKeyValue(); /* 获取机械按键返回按键值 */

    for (uint8 i = 0; i < BSP_NUM_KEYS; i++) /* 根据bsp_key找到对应hal_key */
    {
        if (pkey_map[i].bspKeyValue & bspKeyValue)
        {
            halKeyValue |= pkey_map[i].halKeyValue;
        }
    }

    return halKeyValue;
}