/*
 * main.c
 *
 *  Created on: Mar 2, 2018
 *      Author: Iggy
 */
#include "xgpio.h"
#include "xil_printf.h"
 #include "xaxipmon.h"

int main(){
	xil_printf("Enter........\n\r");
	int i = 0;
	int counter = 0;
	uint8_t data;
	XGpio gpio;

	XAxiPmon_Config *pmonConfig;
	XAxiPmon pmon;
	u32 *increment = NULL;
 	pmonConfig = XAxiPmon_LookupConfig(0);



	xil_printf("Performance Monitor address: 0x%X\n\r",pmonConfig->BaseAddress);
	if (XAxiPmon_CfgInitialize(&pmon, pmonConfig,pmonConfig->BaseAddress) == 0){
		xil_printf("Performance Monitor initialized........\n\r");

	}else{
		xil_printf("Performance Monitor Initialization failed........\n\r");

	}
 	xil_printf("Performance Monitor counter reset: %d\n\r",XAxiPmon_ResetMetricCounter(&pmon));
 	XAxiPmon_ResetGlobalClkCounter(&pmon);
 	XAxiPmon_ResetFifo(&pmon);
 	XAxiPmon_EnableGlobalClkCounter(&pmon);
 	XAxiPmon_EnableMetricCounterReset(&pmon);
 	XAxiPmon_EnableSampleIntervalCounter(&pmon);
 	XAxiPmon_SetIncrementerRange(&pmon,0x01,0x1000,0x0001);
 	XAxiPmon_SetSampleInterval(&pmon,0x00000001);
 	xil_printf("Performance Monitor counter start: %d\n\r",XAxiPmon_StartCounters(&pmon,0x00000001));



	if (XGpio_Initialize(&gpio,0) == XST_SUCCESS){
		xil_printf("GPIO Initialized........\n\r");
 	}
	else{
		xil_printf("GPIO Initialization failed........\n\r");
	}
	XGpio_SetDataDirection(&gpio,1, 0x000);
	XGpio_SetDataDirection(&gpio,2, 0xFFFF);

	while(1){
 		data = XGpio_DiscreteRead(&gpio,2);
 		XAxiPmon_GetSampleInterval(&pmon,increment);
 		xil_printf("\rPM Counter 0x%08x", *increment);

 		xil_printf("Sampled Metric Counter %d ", XAxiPmon_GetSampledMetricCounter(&pmon,1));

 		//xil_printf(" PM Incrementer: %d", XAxiPmon_GetSampledIncrementer(&pmon,0x00000001));
		xil_printf(" GPIO DATA  0x%08x " , data);
		//("\rGPIO address: 0x%08x Written ", gpio.GpioConfig.BaseAddr);
		XGpio_DiscreteWrite(&gpio,1, data);
		for(i = 0; i < 10000; i++){};
		counter++;

	}

}
