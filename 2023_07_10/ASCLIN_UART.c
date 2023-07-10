#include "IfxAsclin_Asc.h"
#include "IfxCpu_Irq.h"
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define UART_BAUDRATE           115200                                  /* UART baud rate in bit/s                  */

#define UART_PIN_RX             IfxAsclin0_RXB_P15_3_IN                 /* UART receive port pin                    */
#define UART_PIN_TX             IfxAsclin0_TX_P15_3_OUT                 /* UART transmit port pin                   */

/* Definition of the interrupt priorities */
#define INTPRIO_ASCLIN0_RX      18
#define INTPRIO_ASCLIN0_TX      19

#define UART_RX_BUFFER_SIZE     32                                     /* Definition of the receive buffer size    */
#define UART_TX_BUFFER_SIZE     64                                      /* Definition of the transmit buffer size   */
#define SIZE                    13                                      /* Size of the string                       */

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/


/*********************************************************************************************************************/
/*********************************************************************************************************************/
/* Declaration of the ASC handle */
static IfxAsclin_Asc g_ascHandle;


/* Declarg_ascHandleation of the FIFOs parameters */
static uint8 g_ascTxBuffer[UART_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];
static uint8 g_ascRxBuffer[UART_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];

char msg1[100] = {0,} ;
//double buffer ;
char c = 'A';

extern long encoderPos;

//int cnt = 0;
//sprintf(msg1,"Hellow");
//u//int32 cnt = 0;
/* Definition of txData and rxData */
uint8 g_txData[] = {''};
//uint8 g_rxData[SIZE] = {''}+1;

/* Size of the message */
Ifx_SizeT g_count = sizeof(g_txData);

int cnt_rcv = 0;
/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* Adding of the interrupt service routines */
IFX_INTERRUPT(asclin0TxISR, 0, INTPRIO_ASCLIN0_TX);
void asclin0TxISR(void)
{
    IfxAsclin_Asc_isrTransmit(&g_ascHandle);
    cnt_rcv++;

}

IFX_INTERRUPT(asclin0RxISR, 0, INTPRIO_ASCLIN0_RX);
void asclin0RxISR(void)
{
    IfxAsclin_Asc_isrReceive(&g_ascHandle);
    cnt_rcv++;
}

sint32 receive_buff_count(void)
{
    return IfxAsclin_Asc_getReadCount(&g_ascHandle);

}
/* This function initializes the ASCLIN UART module */
void init_ASCLIN_UART(void)
{
    /* Initialize an instance of IfxAsclin_Asc_Config with default values */
    IfxAsclin_Asc_Config ascConfig;
    IfxAsclin_Asc_initModuleConfig(&ascConfig, &MODULE_ASCLIN0);

    /* Set the desired baud rate */
    ascConfig.baudrate.baudrate = UART_BAUDRATE;

    /* ISR priorities and interrupt target */
    ascConfig.interrupt.txPriority = INTPRIO_ASCLIN0_TX;
    ascConfig.interrupt.rxPriority = INTPRIO_ASCLIN0_RX;
    ascConfig.interrupt.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());

    /* FIFO configuration */
    ascConfig.txBuffer = &g_ascTxBuffer;
    ascConfig.txBufferSize = UART_TX_BUFFER_SIZE;
    ascConfig.rxBuffer = &g_ascRxBuffer;
    ascConfig.rxBufferSize = UART_RX_BUFFER_SIZE;

    /* Pin configuration */
    const IfxAsclin_Asc_Pins pins =
    {
        NULL_PTR,       IfxPort_InputMode_pullUp,     /* CTS pin not used */
        &UART_PIN_RX,   IfxPort_InputMode_pullUp,     /* RX pin           */
        NULL_PTR,       IfxPort_OutputMode_pushPull,  /* RTS pin not used */
        &UART_PIN_TX,   IfxPort_OutputMode_pushPull,  /* TX pin           */
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    ascConfig.pins = &pins;

    IfxAsclin_Asc_initModule(&g_ascHandle, &ascConfig); /* Initialize module with above parameters */
}

/* This function sends and receives the string "Hello World!" */
void send_receive_ASCLIN_UART_message(void)
{
 //   g_count = sprintf(g_txData,"%ld",encoderPos);
    IfxAsclin_Asc_write(&g_ascHandle, g_txData, &g_count, TIME_INFINITE);   /* Transmit data via TX */
//    IfxAsclin_Asc_read(&g_ascHandle, g_rxData, &g_count, TIME_INFINITE);    /* Receive data via RX  */
}

void send_ASCLIN_UART_message(void)
{
    g_count = sprintf(g_txData,"%ld\n",encoderPos);
   IfxAsclin_Asc_write(&g_ascHandle, g_txData, &g_count, TIME_INFINITE);   /* Transmit data via TX */
   //IfxAsclin_Asc_getWriteCount(&g_ascHandle);
   //IfxAsclin_Asc_canWriteCount(&g_ascHandle, &g_count, TIME_INFINITE);
   // IfxAsclin_Asc_blockingWrite(&g_ascHandle,g_txData);
  // IfxAsclin_Asc_canWriteCount(&g_ascHandle, &g_count, TIME_INFINITE);
}
