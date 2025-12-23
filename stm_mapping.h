#ifndef HW_MAPPING_H
#define HW_MAPPING_H

/* ========================================================================== */
/* 1. BASE ADDRESSES (Địa chỉ cơ sở)                                          */
/* ========================================================================== */

#define PERIPH_BASE         (0x40000000UL)
#define APB1PERIPH_BASE     (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000UL)

/* ========================================================================== */
/* 2. RCC (Reset & Clock Control)                                             */
/* ========================================================================== */
#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800UL) // 0x40023800

typedef struct {
    volatile unsigned int CR;            // 0x00
    volatile unsigned int PLLCFGR;       // 0x04
    volatile unsigned int CFGR;          // 0x08
    volatile unsigned int CIR;           // 0x0C
    volatile unsigned int AHB1RSTR;      // 0x10
    volatile unsigned int AHB2RSTR;      // 0x14
    volatile unsigned int AHB3RSTR;      // 0x18
    volatile unsigned int Reserved1;     // 0x1C
    volatile unsigned int APB1RSTR;      // 0x20
    volatile unsigned int APB2RSTR;      // 0x24
    volatile unsigned int Reserved2[2];  // 0x28-0x2C
    volatile unsigned int AHB1ENR;       // 0x30: Bật clock GPIO
    volatile unsigned int AHB2ENR;       // 0x34
    volatile unsigned int AHB3ENR;       // 0x38
    volatile unsigned int Reserved3;     // 0x3C
    volatile unsigned int APB1ENR;       // 0x40: Bật clock TIM2
    volatile unsigned int APB2ENR;       // 0x44: Bật clock USART1, SYSCFG
} RCC_TypeDef;

#define RCC                 ((RCC_TypeDef *) RCC_BASE)

/* ========================================================================== */
/* 3. GPIO (General Purpose I/O)                                              */
/* ========================================================================== */
#define GPIOA_BASE          (AHB1PERIPH_BASE + 0x0000UL) // Button (PA0), USART1 (PA9/10)
#define GPIOG_BASE          (AHB1PERIPH_BASE + 0x1800UL) // LED Green (PG13), Red (PG14)

typedef struct {
    volatile unsigned int MODER;    // 0x00: Mode (Input/Output/Alt Func)
    volatile unsigned int OTYPER;   // 0x04: Output Type
    volatile unsigned int OSPEEDR;  // 0x08: Output Speed
    volatile unsigned int PUPDR;    // 0x0C: Pull-up/Pull-down
    volatile unsigned int IDR;      // 0x10: Input Data
    volatile unsigned int ODR;      // 0x14: Output Data
    volatile unsigned int BSRR;     // 0x18: Bit Set/Reset
    volatile unsigned int LCKR;     // 0x1C: Lock
    volatile unsigned int AFRL;     // 0x20: Alternate Function Low
    volatile unsigned int AFRH;     // 0x24: Alternate Function High
} GPIO_TypeDef;

#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)

/* ========================================================================== */
/* 4. SYSCFG (System Configuration Controller)                                */
/* ========================================================================== */
/* Dùng để map chân GPIO vào ngắt EXTI (VD: PA0 -> EXTI0) */
#define SYSCFG_BASE         (APB2PERIPH_BASE + 0x3800UL) // 0x40013800

typedef struct {
    volatile unsigned int MEMRMP;       // 0x00
    volatile unsigned int PMC;          // 0x04
    volatile unsigned int EXTICR[4];    // 0x08-0x14: EXTI config (CR1..CR4)
    volatile unsigned int CMPCR;        // 0x20
} SYSCFG_TypeDef;

#define SYSCFG              ((SYSCFG_TypeDef *) SYSCFG_BASE)

/* ========================================================================== */
/* 5. EXTI (External Interrupt/Event Controller)                              */
/* ========================================================================== */
#define EXTI_BASE           (APB2PERIPH_BASE + 0x3C00UL) // 0x40013C00

typedef struct {
    volatile unsigned int IMR;    // 0x00: Interrupt Mask (Cho phép ngắt)
    volatile unsigned int EMR;    // 0x04: Event Mask
    volatile unsigned int RTSR;   // 0x08: Rising Trigger (Cạnh lên)
    volatile unsigned int FTSR;   // 0x0C: Falling Trigger (Cạnh xuống)
    volatile unsigned int SWIER;  // 0x10: Software Interrupt
    volatile unsigned int PR;     // 0x14: Pending Register (Cờ báo ngắt)
} EXTI_TypeDef;

#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)

/* ========================================================================== */
/* 6. TIM2 (General Purpose Timer - 32 bit)                                   */
/* ========================================================================== */
#define TIM2_BASE           (APB1PERIPH_BASE + 0x0000UL) // 0x40000000

typedef struct {
    volatile unsigned int CR1;    // 0x00: Control Register 1
    volatile unsigned int CR2;    // 0x04: Control Register 2
    volatile unsigned int SMCR;   // 0x08: Slave Mode Control
    volatile unsigned int DIER;   // 0x0C: DMA/Interrupt Enable (Bật ngắt Update)
    volatile unsigned int SR;     // 0x10: Status Register (Cờ ngắt UIF)
    volatile unsigned int EGR;    // 0x14: Event Generation
    volatile unsigned int CCMR1;  // 0x18: Capture/Compare Mode 1
    volatile unsigned int CCMR2;  // 0x1C: Capture/Compare Mode 2
    volatile unsigned int CCER;   // 0x20: Capture/Compare Enable
    volatile unsigned int CNT;    // 0x24: Counter Value
    volatile unsigned int PSC;    // 0x28: Prescaler
    volatile unsigned int ARR;    // 0x2C: Auto-Reload Value
} TIM_TypeDef;

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)

/* ========================================================================== */
/* 7. USART1 (Universal Synchronous Asynchronous Receiver Transmitter)        */
/* ========================================================================== */
#define USART1_BASE         (APB2PERIPH_BASE + 0x1000UL) // 0x40011000

typedef struct {
    volatile unsigned int SR;     // 0x00: Status Register (RXNE, TXE)
    volatile unsigned int DR;     // 0x04: Data Register
    volatile unsigned int BRR;    // 0x08: Baud Rate Register
    volatile unsigned int CR1;    // 0x0C: Control 1 (UE, TE, RE, RXNEIE)
    volatile unsigned int CR2;    // 0x10: Control 2
    volatile unsigned int CR3;    // 0x14: Control 3
    volatile unsigned int GTPR;   // 0x18: Guard time and prescaler
} USART_TypeDef;

#define USART1              ((USART_TypeDef *) USART1_BASE)

/* ========================================================================== */
/* 8. NVIC (Nested Vectored Interrupt Controller) - Core Peripheral           */
/* ========================================================================== */
/* Địa chỉ này thuộc vùng System Control Space của Cortex-M4 */

#define NVIC_ISER0          (*(volatile unsigned int *)(0xE000E100UL)) // Enable 0-31
#define NVIC_ISER1          (*(volatile unsigned int *)(0xE000E104UL)) // Enable 32-63
#define NVIC_ICER0          (*(volatile unsigned int *)(0xE000E180UL)) // Disable 0-31
#define NVIC_ICER1          (*(volatile unsigned int *)(0xE000E184UL)) // Disable 32-63
#define NVIC_ICPR0          (*(volatile unsigned int *)(0xE000E280UL)) // Clear Pending 0-31
#define NVIC_ICPR1          (*(volatile unsigned int *)(0xE000E284UL)) // Clear Pending 32-63
#define NVIC_IPR_BASE       (0xE000E400UL) // Priority Register Base

/* IRQ Numbers (Số thứ tự ngắt trong Vector Table) */
#define EXTI0_IRQn          6   // Ngắt nút nhấn PA0
#define TIM2_IRQn           28  // Ngắt Timer 2
#define USART1_IRQn         37  // Ngắt USART 1

#endif /* HW_MAPPING_H */

