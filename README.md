[![Build Status](https://img.shields.io/badge/USEFUL%20ELECTRONICS-YOUTUBE-red)](https://www.youtube.com/user/wardzx1)

# Sponsors

### PCBWay
Tutorial Series Sponsor PCBWay Website. You can get extra free coupons using the link below:

[<img src="https://github.com/UsefulElectronics/esp32s3-gc9a01-lvgl/blob/main/banner/banner(320x100).gif">](https://www.pcbway.com/setinvite.aspx?inviteid=582640)

### REYAX
RYLR993 LoRaWAN module manufacturer 
https://reyax.com/products/RYLR993
***
# ESP32 RYLR993 LoRaWAN Communication

### [Tutorial Link](https://youtu.be/zhOi7GMKKLE) On [![Build Status](https://img.shields.io/badge/YouTube-FF0000?style=for-the-badge&logo=youtube&logoColor=white)](https://www.youtube.com/wardzx1) 

In this tutorial, I have built a simple system that is powered with 1 W 5 V solar panel that monitors the connected battery voltage with ADC and gets the environment temperature and shares this information with the thing network over LoRaWAN. The system also supports modifying GPIO state wirelessly over LoRaWAN.

The system uses RYLR993 LoRaWAN Module and ESP32C2 as hosting MCU. The LoRaWAN module is controlled using AT commands over UART and contains an internal temperature sensor that the system makes use of.

![Circuit Diagram](https://github.com/UsefulElectronics/esp32c2-rylr993-battery-voltage-lorawan/blob/main/circuit%20diagram/system%20diagram.PNG)
***

Remember to execute set-target command depending on your MCU
```sh
idf.py set-target
```

| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |


