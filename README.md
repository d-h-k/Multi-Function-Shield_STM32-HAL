# Multi-Function-Shield_STM32-HAL
Multi-Function-Shield_STM32-HAL


 - Demo : https://youtu.be/FD9NlIemW7M
 - 진행중 이슈
    - USART2 TX RX 핀과 SW2 와 겹치는 문제 해결 방안 고민중 : SB를 다르게 붙이거나 USART2를 사용하지 않는 중 고민
    - H/W Timer가 아닌 SW Timer를 이용한(Tick같은..) 74HC595 제어 -> FND 제어를 HW dependency 줄이는 방향으로
    - BSP API 제공 범위>
