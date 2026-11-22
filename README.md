# STM32 IoT Sensor to Node-RED | STM32 物联网采集与 Node-RED

[English](#english) · [简体中文](#简体中文)

## English

An STM32F103ZE reads a DHT11 temperature and humidity sensor and an analog light sensor. The firmware sends JSON over an ESP8266 AT-command UART connection to Node-RED. This package includes the [application entry point](firmware-example/main.c) and an [importable Node-RED flow](node-red/flows.json).

1. For hardware, use a Keil MDK project for the STM32F103ZE with the corresponding standard peripheral library, DHT11, light sensor, board GPIO, delay and USART drivers. Copy the application entry point into that project and set the local Wi-Fi and Node-RED host placeholders. Keep real settings in your local working copy.
2. Import node-red/flows.json in Node-RED and deploy. The POST /sensor endpoint validates three numeric fields and writes the latest sample to the Debug sidebar. GET /sensor/latest returns the latest accepted sample.
3. Start with the HTTP smoke test below. Use the Node-RED machine's LAN address in the firmware when moving to hardware. The flow uses in-memory state by default and is intended for a local lab network.

## 简体中文

STM32F103ZE 采集 DHT11 温湿度和光敏传感器数据，经串口控制 ESP8266，将 JSON 传给 Node-RED。本仓库提供[应用入口](firmware-example/main.c)和[可导入的 Node-RED 流程](node-red/flows.json)。

1. 实物端使用 STM32F103ZE 的 Keil MDK 工程，准备对应标准外设库及 DHT11、光敏、GPIO、延时和 USART 驱动。将应用入口加入工程，并在本地工作副本里填写 Wi-Fi 和 Node-RED 主机占位符。
2. 在 Node-RED 中导入 node-red/flows.json 并部署。POST /sensor 校验三个数值字段，并在 Debug 侧栏显示最新采样。GET /sensor/latest 返回最后一次接收的读数。
3. 先执行下方 HTTP 冒烟测试。连接实物时，在固件中填写 Node-RED 主机的局域网地址。流程默认将读数保存在内存中，面向本地实验网络。

~~~powershell
Invoke-RestMethod -Method Post -Uri http://localhost:1880/sensor -ContentType 'application/json' -Body '{"temp":24,"humi":52,"light":37}'
Invoke-RestMethod -Uri http://localhost:1880/sensor/latest
~~~

Temperature is in °C, humidity is in %RH, and light is a firmware sensor reading. / 温度单位为 °C，湿度单位为 %RH，光照为固件传感器读数。

Verified on Node-RED 4.0.9 with an accepted sample, invalid input and an empty latest-reading request. / 已用 Node-RED 4.0.9 实测有效采样、无效输入和未收到读数时的查询。
