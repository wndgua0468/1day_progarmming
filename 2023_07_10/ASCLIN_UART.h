#ifndef ASCLIN_UART_H_
#define ASCLIN_UART_H_

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
void init_ASCLIN_UART(void);                 /* Initialization function   */
void send_receive_ASCLIN_UART_message(void); /* Send and receive function */
void send_ASCLIN_UART_message(void);
void receive_buff_count(void);
void sample(void);
#endif /* ASCLIN_UART_H_ */
