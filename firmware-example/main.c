#include "sys.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "dht11.h"
#include "lsens.h"
#include <stdio.h>
#include <string.h>   // ★ 必须加

/* ================= ESP8266 发送 ================= */
void ESP8266_SendString(char *str)
{
    while (*str)
    {
        USART_SendData(USART2, *str++);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}

int main(void)
{
    /* ===== 所有变量必须放在最前面（C89） ===== */
    u8 temp = 0;
    u8 humi = 0;
    u8 light = 0;

    char json[128];
    char http[512];
    char cmd[64];

    delay_init();
    LED_Init();
    uart2_init(115200);   // USART2 -> ESP8266
    Lsens_Init();

    /* DHT11 初始化 */
    while (DHT11_Init())
    {
        LED0 = !LED0;
        delay_ms(300);
    }

    delay_ms(3000);   // 等 ESP8266 上电

    /* ===== ESP8266 基本配置 ===== */
    ESP8266_SendString("AT\r\n");
    delay_ms(1000);

    ESP8266_SendString("AT+CWMODE=1\r\n");
    delay_ms(1000);

    ESP8266_SendString(
        "AT+CWJAP=\"YOUR_WIFI_SSID\",\"YOUR_WIFI_PASSWORD\"\r\n"
    );
    delay_ms(6000);

    LED0 = 1;   // WiFi OK

    while (1)
    {
        /* 采集传感器 */
        DHT11_Read_Data(&temp, &humi);
        light = Lsens_Get_Val();

        /* 建立 TCP 连接 */
        ESP8266_SendString(
            "AT+CIPSTART=\"TCP\",\"NODE_RED_HOST\",1880\r\n"
        );
        delay_ms(2000);

        /* JSON 数据 */
        sprintf(json,
            "{\"temp\":%d,\"humi\":%d,\"light\":%d}",
            temp, humi, light
        );

        /* HTTP POST */
        sprintf(http,
            "POST /sensor HTTP/1.1\r\n"
            "Host: NODE_RED_HOST\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n\r\n"
            "%s",
            strlen(json), json
        );

        /* 通知发送长度 */
        sprintf(cmd, "AT+CIPSEND=%d\r\n", strlen(http));
        ESP8266_SendString(cmd);
        delay_ms(1000);

        /* 发送数据 */
        ESP8266_SendString(http);

        LED0 = !LED0;
        delay_ms(200);
    }
}
