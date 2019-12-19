使用软件
- Keil MDK 5.25
- IAR embedded Workbench 8.22


波特率115200  8位数据位 1位停止位 没有奇偶校验

使用串口模块（串口蓝牙，USB转TTL或者其他串口）
将 P0_29 连接串口模块（串口蓝牙.USB转TTL）的 TX上
将 P0_30 连接串口模块（串口蓝牙.USB转TTL）的 RX上
共地


测试systick定时器  systick可以用来 计时和延时
实验现象
示波器观察 LED_R_PIN  P0_8  500hz


