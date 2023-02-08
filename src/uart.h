/* date = February 6th 2023 0:00 pm */

#ifndef UART_H
#define UART_H

#include <stdint.h>

#define PAD_STRUCT(n) uint8_t __padding_##__COUNTER__[n];

typedef enum
{
    UART_BAUDRATE_300   = 0,
    UART_BAUDRATE_600   = 1,
    UART_BAUDRATE_1200  = 2,
    UART_BAUDRATE_2400  = 3,
    UART_BAUDRATE_4800  = 4,
    UART_BAUDRATE_9600  = 5,
    UART_BAUDRATE_14400 = 6,
    
    UART_BAUDRATE_LAST = UART_BAUDRATE_14400
} uart_baudrate_t;

#define UART0_REG_BASE_ADDR 0x44E09000
#define UART1_REG_BASE_ADDR 0x48024000
#define UART2_REG_BASE_ADDR 0x48024000
#define UART3_REG_BASE_ADDR 0x481A6000
#define UART4_REG_BASE_ADDR 0x481A8000
#define UART5_REG_BASE_ADDR 0x481AA000

// Represents a UART module's registers
typedef struct uart_regs_t
{
    union
    {
        uint16_t thr;
        uint16_t rhr;
        uint16_t dll;
    };
    PAD_STRUCT(2);
    union
    {
        uint16_t ier_irda;
        uint16_t ier_cir;
        uint16_t ier_uart;
        uint16_t dlh;
    };
    PAD_STRUCT(2);
    union
    {
        uint16_t efr;
        uint16_t iir_uart;
        uint16_t iir_cir;
        uint16_t fcr;
        uint16_t iir_irda;
    };
    PAD_STRUCT(2);
    uint16_t lcr;
    PAD_STRUCT(2);
    uint16_t mcr;
    PAD_STRUCT(2);
    union
    {
        uint16_t mcr;
        uint16_t xon1_addr1;
    };
    PAD_STRUCT(2);
    union
    {
        uint16_t xon2_addr2;
        uint16_t lsr_cir;
        uint16_t lsr_irda;
        uint16_t lsr_uart;
    };
    PAD_STRUCT(2);
    union
    {
        uint16_t tcr;
        uint16_t msr;
        uint16_t xoff1;
    };
    PAD_STRUCT(2);
    union
    {
        uint16_t spr;
        uint16_t tlr;
        uint16_t xoff2;
    }:
    PAD_STRUCT(2);
    uint16_t mdr1;
    PAD_STRUCT(2);
    uint16_t mdr2;
    PAD_STRUCT(2);
    union
    {
        uint16_t txfill;
        uint16_t sflsr;
    };
    PAD_STRUCT(2);
    union
    {
        uint16_t resume;
        uint16_t txflh;
    };
    PAD_STRUCT(2);
    union
    {
        uint16_t rxfll;
        uint16_t sfregl;
    };
    PAD_STRUCT(2);
    union
    {
        uint16_t sfregh;
        uint16_t rxflh;
    };
    PAD_STRUCT(2);
    union
    {
        uint16_t blr;
        uint16_t uasr;
    };
    PAD_STRUCT(2);
    uint16_t acreg;
    PAD_STRUCT(2);
    uint16_t scr;
    PAD_STRUCT(2);
    uint16_t ssr;
    PAD_STRUCT(2);
    uint16_t eblr;
    PAD_STRUCT(2);
    uint16_t mvr;
    PAD_STRUCT(2);
    uint16_t sysc;
    PAD_STRUCT(2);
    uint16_t syss;
    PAD_STRUCT(2);
    uint16_t wer;
    PAD_STRUCT(2);
    uint16_t cfps;
    PAD_STRUCT(2);
    uint16_t rxfifo_lvl;
    PAD_STRUCT(2);
    uint16_t txfifo_lvl;
    PAD_STRUCT(2);
    uint16_t ier2;
    PAD_STRUCT(2);
    uint16_t isr2;
    PAD_STRUCT(2);
    uint16_t freq_sel;
    PAD_STRUCT(2);
    uint16_t mdr3;
    PAD_STRUCT(2);
    uint16_t tx_dma_threshold;
} uart_regs_t;

volatile uart_regs_t *uart0 = (uart_regs_t*)UART0_REG_BASE_ADDR;
volatile uart_regs_t *uart1 = (uart_regs_t*)UART1_REG_BASE_ADDR;
volatile uart_regs_t *uart2 = (uart_regs_t*)UART2_REG_BASE_ADDR;
volatile uart_regs_t *uart3 = (uart_regs_t*)UART3_REG_BASE_ADDR;
volatile uart_regs_t *uart4 = (uart_regs_t*)UART4_REG_BASE_ADDR;
volatile uart_regs_t *uart5 = (uart_regs_t*)UART5_REG_BASE_ADDR;

typedef uart_regs_t uart_t;

// Initializes a UART module.
void uart_init(uart_t *uart);
// Sets the baud rate for a UART module.
void uart_set_baud(uart_t *uart, int baudrate);
// Writes to a UART module's output buffer.
void uart_write(uart_t *uart, void *src, size_t count);
// Reads from a UART module's input buffer.
void uart_read(uart_t *uart, void *dst, size_t count);

#endif //UART_H
