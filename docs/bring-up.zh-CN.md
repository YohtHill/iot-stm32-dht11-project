# 硬件接线与 Node-RED 联调

[English](bring-up.md) · [项目首页](../README.zh-CN.md)

| 接口 | 工程配置 |
| --- | --- |
| ESP8266 AT 串口 | USART2，PA2 TX 接 ESP RX，PA3 RX 接 ESP TX，115200 波特率 |
| 电脑串口调试 | USART1 的 PA9/PA10 |
| DHT11 数据 | 原板驱动的 GPIO 宏使用 PG11 |
| 光敏传感器模拟输出 | 原板驱动使用 PF8 |
| ESP8266 供电 | 稳定 3.3 V，与 STM32 共地 |

课程报告记录的主要联调问题是板载调试口和 ESP8266 同时占用 USART1。将 ESP8266 改接 USART2 后，调试输出与 AT 指令分开。仓库提供应用入口，构建时需加入使用标准外设库和对应板级驱动的 Keil 工程。Wi-Fi 和 Node-RED 参数填写在本地工作副本。

固件通过 AT 指令入网，建立到 1880 端口的 TCP 连接并向 /sensor 发送 POST。JSON 字段为 `temp`、`humi` 和 `light`。温度单位为 ℃，湿度为相对湿度百分比，光照为板级驱动的相对读数。

导入并部署 [flows.json](../node-red/flows.json)，在 Node-RED 主机上执行下方测试。接入实物时，在固件中填写设备能访问的主机局域网地址。

~~~powershell
Invoke-RestMethod -Method Post -Uri http://localhost:1880/sensor -ContentType 'application/json' -Body '{"temp":24,"humi":52,"light":37}'
Invoke-RestMethod -Uri http://localhost:1880/sensor/latest
~~~

Node-RED 4.0.9 实测有效上报和最近读数查询返回 200，缺字段返回 400，尚未上报时查询返回 404。当前流程在内存中保存最新读数。长时间独立运行前还需验证固件的上传时序和重连行为。
