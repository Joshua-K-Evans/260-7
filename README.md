# 260-7
 In this lab, I used USART2 (PA2/PA3) to send messages between my laptop and my
 Nucleo board over the USB connection (Part 1). I also used USART1 (PA9/PA10) for
 another UART channel to send messages between my Nucleo board and my lab partner’s
 Nucleo board (Part 2). I expanded on this in Part 3
  Specifications
 This project uses the UART communication mode of an STM32 microcontroller on a
 Nucleo development board, specifically the Nucleo32-L476RG [1]. An overview of the UART
 protocol may be found in Chapter 9 of [2]. The task required 3 LEDs per person and the
 proper breadboard setup with 100 ohm resistors. The STM code was then setup so that
 Board 1 gave commands to Board 2, which in turn made commands to Board 3 which gave
 it’s commands to Board 1. The commands were to control the others LEDs(on, off or blink)
 via Putty. So using Putty our laptop talked to our board which in turn talked to the specified
 oher board which notified Putty on their laptop
