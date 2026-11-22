# Hardware and Node-RED Bring-up

[简体中文](bring-up.zh-CN.md) · [Project home](../README.md)

| Connection | Project configuration |
| --- | --- |
| ESP8266 AT serial | USART2, PA2 TX to ESP RX and PA3 RX to ESP TX, 115200 baud |
| PC debugging | USART1 on PA9/PA10 |
| DHT11 data | PG11 according to the original board driver's GPIO macros |
| Light sensor analog output | PF8 according to the original board driver |
| ESP8266 supply | Stable 3.3 V and common ground |

The course report identifies the duplicated use of USART1 by the board's debug interface and the ESP8266 as the main bring-up fault. Moving the ESP8266 to USART2 kept debugging separate. The entry point supplied here uses the original board's drivers through a standard peripheral library Keil project. Fill in the Wi-Fi and Node-RED placeholders in your working copy.

The firmware joins Wi-Fi via AT commands, opens a TCP connection to port 1880 and sends a POST to /sensor with numeric fields `temp`, `humi` and `light`. Temperature is in °C and humidity is in %RH. Light is the board driver's relative reading, which is not a calibrated illuminance value.

Deploy [flows.json](../node-red/flows.json) in Node-RED. Run the following smoke test from the Node-RED host. To connect the real device, use the host's reachable LAN address.

~~~powershell
Invoke-RestMethod -Method Post -Uri http://localhost:1880/sensor -ContentType 'application/json' -Body '{"temp":24,"humi":52,"light":37}'
Invoke-RestMethod -Uri http://localhost:1880/sensor/latest
~~~

Node-RED 4.0.9 returned 200 for valid input and the latest sample, 400 for an incomplete sample and 404 when queried before any sample was received. The shipped flow keeps the latest reading in memory. Recheck timing and reconnect behaviour before relying on the firmware for long unattended runs.
