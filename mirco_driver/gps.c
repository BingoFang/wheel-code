#include <stdio.h>
#include <string.h>

#define DATA_BUF_LENGTH  	80
#define UTC_TIME_LENGTH		11
#define LATITUDE_LENGTH		10
#define N_S_LENGTH			2
#define LONGITUDE_LENGTH	11
#define E_W_LTNGTH			2

typedef struct
{
    char DataBuf[DATA_BUF_LENGTH];
    char IsGetData;		//是否获取到GPS数据
    char IsParseData;	//是否解析完成
    char UtcTime[UTC_TIME_LENGTH];	//UTC时间
    char latitude[LATITUDE_LENGTH];	//纬度
    char N_S[N_S_LENGTH];		//N/S
    char longitude[LONGITUDE_LENGTH];	//经度
    char E_W[E_W_LTNGTH];		//E/W
    char IsUsefull;		//定位信息是否有效
} UBLOX_GPS;

UBLOX_GPS UbloxGps = {0};

gps_parse_data(void)
{
    char *SubString;
    char *SubStringNext;
    char i;
    char UseFullBuffer[2] = {0};

    if (UbloxGps.IsGetData)	//获取gps数据成功
    {
        UbloxGps.IsGetData = false;
        printf("UbloxGps.DataBuf:%s\r\n", UbloxGps.DataBuf);
        for (i = 0; i <= 6; i++)
        {
            if (i == 0)
            {
                if ((SubString = strstr(UbloxGps.DataBuf, ",")) == NULL) //定位到第一个','位置
                    printf("gps data err:1!\r\n");
            }
            else
            {
                SubString++;  //跳过','
                if ((SubStringNext = strstr(SubString, ",")) != NULL)
                {
                    switch (i)
                    {
                    case 1:
                        memcpy(UbloxGps.UtcTime, SubString, SubStringNext - SubString);
                        break;
                    case 2:
                        memcpy(UseFullBuffer, SubString, SubStringNext - SubString);
                        break;
                    case 3:
                        memcpy(UbloxGps.latitude, SubString, SubStringNext - SubString);
                        break;
                    case 4:
                        memcpy(UbloxGps.N_S, SubString, SubStringNext - SubString);
                        break;
                    case 5:
                        memcpy(UbloxGps.longitude, SubString, SubStringNext - SubString);
                        break;
                    case 6:
                        memcpy(UbloxGps.E_W, SubString, SubStringNext - SubString);
                        break;
                    default:
                        break;
                    }
                    SubString = SubStringNext;
                    UbloxGps.IsParseData = true;
                    if (UseFullBuffer[0] == 'A')
                        UbloxGps.IsUsefull = true;
                    else if (UseFullBuffer[0] == 'V')
                        UbloxGps.IsUsefull = false;
                }
                else
                {
                    printf("gps data err:2!\r\n");
                }
            }
        }
    }
}

int main()
{
    char Uart0Buf[80] = "$GNRMC,085425.000,A,2239.7941,N,11417.8101,E,1.04,201.28,120619,,,A*74";
    char i;
    if (Uart0Buf[0] == '$' && Uart0Buf[4] == 'M' && Uart0Buf[5] == 'C')
    {

        memset(UbloxGps.DataBuf, 0, DATA_BUF_LENGTH); //清空
        memcpy(UbloxGps.DataBuf, Uart0Buf, DATA_BUF_LENGTH); //保存数据
        UbloxGps.IsGetData = 1;
    }

    gps_parse_data();

    return 0;
}
