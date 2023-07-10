#include "ERU_Interrupt.h"
#include "Bsp.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_SCUERU_INT0   40                       /* Define the SCU ERU interrupt priority                */
#define REQ_IN                     &IfxScu_REQ8_P33_7_IN    /* External request pin                                 */
#define TRIGGER_PIN                &MODULE_P00,7            /* Pin that is toggled by pressing the board's button   */
#define LED                        &MODULE_P00,5            /* LED which gets toggled in Interrupt Service Routine  */
#define ENCODER_A                  &MODULE_P00,2
#define ENCODER_B                  &MODULE_P00,3

#define WAIT_TIME                  50

extern long count = 0;
extern long encoderPos=0;

int encoder_mode = 0;


/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
ERUconfig g_ERUconfig;                                      /* SCU_ERU global data                                  */

/*********************************************************************************************************************/
/*----------------------------------------------Function Implementations---------------------------------------------*/
/*********************************************************************************************************************/
/* Macro to define Interrupt Service Routine.
 * This macro makes the following definitions:
 * 1) Define linker section as .intvec_tc<vector number>_<interrupt priority>.
 * 2) Define compiler specific attribute for the interrupt functions.
 * 3) Define the Interrupt Service Routine as ISR function.
 *
 * IFX_INTERRUPT(isr, vectabNum, priority)
 *  - isr: Name of the ISR function.
 *  - vectabNum: Vector table number.
 *  - priority: Interrupt priority. Refer Usage of Interrupt Macro for more details.
 */
IFX_INTERRUPT(SCUERU_Int0_Handler, 0, ISR_PRIORITY_SCUERU_INT0);

/* Interrupt Service Routine */
void SCUERU_Int0_Handler(void)
{
    count++;
    //waitTime(IfxStm_getTicksFromMicroseconds(BSP_DEFAULT_TIMER, 5));
    if(IfxPort_getPinState(ENCODER_B) == 0 && IfxPort_getPinState(ENCODER_A) == 1)        encoder_mode = 0;
    else if(IfxPort_getPinState(ENCODER_B) == 1 && IfxPort_getPinState(ENCODER_A) == 1)   encoder_mode = 1;

    if(encoder_mode == 1)
    {
        if((IfxPort_getPinState(ENCODER_B) == 0))   encoderPos++;
    }
    else if(encoder_mode == 0)
    {
        if((IfxPort_getPinState(ENCODER_B) == 0))   encoderPos--;
    }

}

/* This functions initializes the output pin for the LED and the pin which toggles the state for generating
 * the falling and rising edges which are used to trigger the interrupt.
 * Additionally this function is configuring the ERU module including the service request configuration */
void initPeripheralsAndERU(void)
{
    /* Initialize pins which are used to trigger and visualize the interrupt and set the default states */
    IfxPort_setPinMode(TRIGGER_PIN, IfxPort_Mode_inputPullUp);              /* Initialize TRIGGER_PIN port pin  */
    IfxPort_setPinMode(LED, IfxPort_Mode_outputPushPullGeneral);            /* Initialize LED port pin          */
    IfxPort_setPinMode(ENCODER_A, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(ENCODER_B, IfxPort_Mode_inputPullUp);

    IfxPort_setPinState(LED, IfxPort_State_high);                           /* Turn off LED (LED is low-active) */

    /* Trigger pin */
    g_ERUconfig.reqPin = REQ_IN; /* Select external request pin */

    /* Initialize this pin with pull-down enabled
     * This function will also configure the input multiplexers of the ERU (Register EXISx)
     */
    IfxScuEru_initReqPin(g_ERUconfig.reqPin, IfxPort_InputMode_pullDown);

    /* Determine input channel depending on input pin */
    g_ERUconfig.inputChannel = (IfxScuEru_InputChannel) g_ERUconfig.reqPin->channelId;

    /* Input channel configuration */
    IfxScuEru_enableRisingEdgeDetection(g_ERUconfig.inputChannel);          /* Interrupt triggers on
                                                                               rising edge (Register RENx) and  */
    IfxScuEru_enableFallingEdgeDetection(g_ERUconfig.inputChannel);         /* on falling edge (Register FENx)  */

    /* Signal destination */
    g_ERUconfig.outputChannel = IfxScuEru_OutputChannel_0;                  /* OGU channel 0                    */
    /* Event from input ETL0 triggers output OGU0 (signal TRx0) */
    g_ERUconfig.triggerSelect = IfxScuEru_InputNodePointer_0;

    /* Connecting Matrix, Event Trigger Logic ETL block */
    /* Enable generation of trigger event (Register EIENx) */
    IfxScuEru_enableTriggerPulse(g_ERUconfig.inputChannel);
    /* Determination of output channel for trigger event (Register INPx) */
    IfxScuEru_connectTrigger(g_ERUconfig.inputChannel, g_ERUconfig.triggerSelect);

    /* Configure Output channels, OutputGating Unit OGU (Register IGPy) */
    IfxScuEru_setInterruptGatingPattern(g_ERUconfig.outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive);

    /* Service request configuration */
    /* Get source pointer depending on outputChannel (SRC_SCUERU0 for outputChannel0) */
    g_ERUconfig.src = &MODULE_SRC.SCU.SCU.ERU[(int) g_ERUconfig.outputChannel % 4];
    IfxSrc_init(g_ERUconfig.src, IfxSrc_Tos_cpu0, ISR_PRIORITY_SCUERU_INT0);
    IfxSrc_enable(g_ERUconfig.src);
}
