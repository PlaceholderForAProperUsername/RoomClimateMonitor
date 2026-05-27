/**
 * @file rp2040_startup.S
 * @brief
 * @author Thorsten Hoffmeister
 * @version 1.0
 * @date 27.05.26
 * @toolchain arm-none-eabi-gcc
 *
 *
 * @copyright (C) 2026  Thorsten Hoffmeister
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

    .syntax unified
    .cpu cortex-m0plus
    .thumb


    .section .text
    .align 2
/**
 * void Reset_Handler(void);
 * @brief Performs basic system initialization after a reset.
 *
 * @param None
 * @retval None
 *
 * @pre The data and bss sections must be word aligned.
 *
 * @post The data from the data section is copied from FLASH to RAM
 * @post The bss section is initialized with the value 0.
 * @post The c standard library is initialized.
 */
    .global Reset_Handler
    .func Reset_Handler
    .thumb_func

    .equ BYTES_PER_WORD,    4

r0_pu32Src      .req    r0
r1_pu32Dest     .req    r1
r2_pu32DestEnd  .req    r2
r3_u32Tmp       .req    r3
Reset_Handler:
Reset_Handler_copy_data_init:
    ldr r0_pu32Src, =__stored_data__                    // uint32_t *r0_pu32Src = __stored_data__;
    ldr r1_pu32Dest, =__data_start__                    // uint32_t *r1_pu32Dest = __data_start__;
    ldr r2_pu32DestEnd, =__data_end__                   // uint32_t *r2_pu32DestEnd = __data_end__;
Reset_Handler_copy_data_loop:
    cmp r1_pu32Dest, r2_pu32DestEnd                     // if (r1_pu32Dest >= r2_pu32DestEnd) {
    bhs Reset_Handler_copy_bss_init                     // goto Reset_Handler_copy_bss_init; }
    ldr r3_u32Tmp, [r0_pu32Src]                         // uint32_t r3_u32Tmp = *r0_pu32Src;
    str r3_u32Tmp, [r1_pu32Dest]                        // *r1_pu32Dest = r3_u32Tmp;
    adds r0_pu32Src, r0_pu32Src, BYTES_PER_WORD         // r0_pu32Src += BYTES_PER_WORD
    adds r1_pu32Dest, r1_pu32Dest, BYTES_PER_WORD       // r1_pu32Dest += BYTES_PER_WORD
    b Reset_Handler_copy_data_loop                      // goto Reset_Handler_copy_data_loop;
Reset_Handler_copy_bss_init:
    ldr r1_pu32Dest, =__bss_start__                     // r1_pu32Dest = __bss_start__ ;
    ldr r2_pu32DestEnd, =__bss_end__                    // r2_pu32DestEnd = __bss_end__;
    movs r3_u32Tmp, #0                                  // r3_u32Tmp = 0;
Reset_Handler_copy_bss_loop:
    cmp r1_pu32Dest, r2_pu32DestEnd                     // if (r1_pu32Dest >= r2_pu32DestEnd) {
    bhs Reset_Handler_copy_finished                     // goto Reset_Handler_copy_finished; }
    str r3_u32Tmp, [r1_pu32Dest]                        // *r1_pu32Dest = r3_u32Tmp;
    adds r1_pu32Dest, r1_pu32Dest, BYTES_PER_WORD       // r1_pu32Dest += BYTES_PER_WORD
    b Reset_Handler_copy_bss_loop                       // goto Reset_Handler_copy_bss_loop
Reset_Handler_copy_finished:
    bl __libc_init_array                                // __libc_init_array();
    bl main                                             // main();

    .unreq r0_pu32Src
    .unreq r1_pu32Dest
    .unreq r2_pu32DestEnd
    .unreq r3_u32Tmp
    .endfunc // Reset_Handler

    .func Default_Handler
    .thumb_func
Default_Handler:
    b Default_Handler               // goto Default_Handler;
    .endfunc // Default_Handler



.weak NMI_Handler
.thumb_set NMI_handler,Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler,Default_Handler

.weak SVCall_Handler
.thumb_set SVCall_Handler,Default_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler,Default_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler,Default_Handler

.weak TIMER_IRQ_0
.thumb_set TIMER_IRQ_0,Default_Handler

.weak TIMER_IRQ_1
.thumb_set TIMER_IRQ_1,Default_Handler

.weak TIMER_IRQ_2
.thumb_set TIMER_IRQ_2,Default_Handler

.weak TIMER_IRQ_3
.thumb_set TIMER_IRQ_3,Default_Handler

.weak PWM_IRQ_WRAP
.thumb_set PWM_IRQ_WRAP,Default_Handler

.weak USBCTRL_IRQ
.thumb_set USBCTRL_IRQ,Default_Handler

.weak XIP_IRQ
.thumb_set XIP_IRQ,Default_Handler

.weak PIO0_IRQ_0
.thumb_set PIO0_IRQ_0,Default_Handler

.weak PIO0_IRQ_1
.thumb_set PIO0_IRQ_1,Default_Handler

.weak PIO1_IRQ_0
.thumb_set PIO1_IRQ_0,Default_Handler

.weak PIO1_IRQ_1
.thumb_set PIO1_IRQ_1,Default_Handler

.weak DMA_IRQ_0
.thumb_set DMA_IRQ_0,Default_Handler

.weak DMA_IRQ_1
.thumb_set DMA_IRQ_1,Default_Handler

.weak IO_IRQ_BANK0
.thumb_set IO_IRQ_BANK0,Default_Handler

.weak IO_IRQ_QSPI
.thumb_set IO_IRQ_QSPI,Default_Handler

.weak SIO_IRQ_PROC0
.thumb_set SIO_IRQ_PROC0,Default_Handler

.weak SIO_IRQ_PROC1
.thumb_set SIO_IRQ_PROC1,Default_Handler

.weak CLOCKS_IRQ
.thumb_set CLOCKS_IRQ,Default_Handler

.weak SPI0_IRQ
.thumb_set SPI0_IRQ,Default_Handler

.weak SPI1_IRQ
.thumb_set SPI1_IRQ,Default_Handler

.weak UART0_IRQ
.thumb_set UART0_IRQ,Default_Handler

.weak UART1_IRQ
.thumb_set UART1_IRQ,Default_Handler

.weak ADC_IRQ_FIFO
.thumb_set ADC_IRQ_FIFO,Default_Handler

.weak I2C0_IRQ
.thumb_set I2C0_IRQ,Default_Handler

.weak I2C1_IRQ
.thumb_set I2C1_IRQ,Default_Handler

.weak RTC_IRQ
.thumb_set RTC_IRQ,Default_Handler


    .section .isr_vector_table
    .align 2

.word __end_stack__
.word Reset_Handler
.word NMI_Handler
.word HardFault_Handler
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word SVCall_Handler
.word 0
.word 0
.word PendSV_Handler
.word SysTick_Handler
.word TIMER_IRQ_0
.word TIMER_IRQ_1
.word TIMER_IRQ_2
.word TIMER_IRQ_3
.word PWM_IRQ_WRAP
.word USBCTRL_IRQ
.word XIP_IRQ
.word PIO0_IRQ_0
.word PIO0_IRQ_1
.word PIO1_IRQ_0
.word PIO1_IRQ_1
.word DMA_IRQ_0
.word DMA_IRQ_1
.word IO_IRQ_BANK0
.word IO_IRQ_QSPI
.word SIO_IRQ_PROC0
.word SIO_IRQ_PROC1
.word CLOCKS_IRQ
.word SPI0_IRQ
.word SPI1_IRQ
.word UART0_IRQ
.word UART1_IRQ
.word ADC_IRQ_FIFO
.word I2C0_IRQ
.word I2C1_IRQ
.word RTC_IRQ


    .end
