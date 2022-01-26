#define KEY_IN_1 GPIO_ReadInputDataBit(GPIOB,GPIO_PIN_0)
#define KEY_IN_2 GPIO_ReadInputDataBit(GPIOB,GPIO_PIN_1)
#define KEY_IN_3 GPIO_ReadInputDataBit(GPIOB,GPIO_PIN_2)
#define KEY_IN_4 GPIO_ReadInputDataBit(GPIOB,GPIO_PIN_3)

void key_scan(void)
{
    uint8_t i = 0;
    static uint8_t keyout = 0;
    static uint8_t keybuf[4][4] =
    {
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF
    };
    /*
      矩阵按键扫描算法：
      每次存储一位，存储四次来进行存储判断该按键是松开还是按下，该函数放在1ms定时器中断中，
      行列都扫描一遍需要4ms，四次就需要16ms，而按键抖动时间一般在10ms左右，所以节省时间只
      判断四位 & 0x0F。 在独立按键中一般是判断八位 & 0xFF
    */
    keybuf[keyout][0] = (keybuf[keyout][0] << 1) | KEY_IN_1;
    keybuf[keyout][1] = (keybuf[keyout][1] << 1) | KEY_IN_2;
    keybuf[keyout][2] = (keybuf[keyout][2] << 1) | KEY_IN_3;
    keybuf[keyout][3] = (keybuf[keyout][3] << 1) | KEY_IN_4;

    for (i = 0; i < 4; i++)
    {
        if ((keybuf[keyout][i] & 0x0F) == 0x0F)
        {
            keysta[keyout][i] = 1;
        }
        else if ((keybuf[keyout][i] & 0x0F) == 0x00)
        {
            keysta[keyout][i] = 0;
        }
    }
    keyout++;
    keyout &= 0x03;

    switch (keyout)
    {
    case 0:
        KEY_OUT_4 = 1;
        KEY_OUT_1 = 0;
        break;
    case 1:
        KEY_OUT_1 = 1;
        KEY_OUT_2 = 0;
        break;
    case 2:
        KEY_OUT_2 = 1;
        KEY_OUT_3 = 0;
        break;
    case 3:
        KEY_OUT_3 = 1;
        KEY_OUT_4 = 0;
        break;
    default:
        break;
    }
}