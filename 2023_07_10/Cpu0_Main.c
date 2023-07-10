#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "ERU_Interrupt.h"
#include "Motor_control_pwm.h"

extern long encoderPos;
IfxCpu_syncEvent g_cpuSyncEvent = 0;
int core0_main(void)
{
    IfxCpu_enableInterrupts();

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    /* Initialize ERU module and peripherals for ERU_Interrupt example */
    initPeripheralsAndERU();
    init_ASCLIN_UART();
    init_PWM_signal_generation();  // PWM 신호 생성 함수 추가

    while(1)
    {

        send_ASCLIN_UART_message();
        //motor_control(pwm);

    }
    return (1);
}
