/*
 * max30100.c
 *
 *  Created on: Jun 26, 2025
 *      Author: srushti
 */

#include "i2c.h"
#include "max30100.h"
#include "Uart.h"


static uint16_t ir_buffer[NUM_SAMPLES];
static uint16_t red_buffer[NUM_SAMPLES];

void max30100_write(uint8_t reg,uint8_t val)
{
	I2CStart();
	I2CSendSlaveAddr(MAX30100_Write_ADDR);
	I2CSendData(reg);
	I2CSendData(val);
	I2CStop();
}

uint8_t max30100_read(uint8_t reg)
{
	I2CStart();
	I2CSendSlaveAddr(MAX30100_Write_ADDR);
	I2CSendData(reg);
	I2CRepeatStart();

	I2CSendSlaveAddr(MAX30100_Read_ADDR);
	uint8_t data = I2CRecvDataNAck();
	I2CStop();
	return data;
}

char message[64];

uint8_t pwr_ready = 0;

void max30100_init(void)
{
    I2CInit();

    for(int i = 0;i<10;i++)
    {
    	sprintf(message, "Initing Max30100. %d...\n\r", i+1);
    	UartPuts(message);
    	uint8_t init = max30100_read(int_status);
    	sprintf(message, "INT_STATUS = 0x%02X\n\r", init);
    	UartPuts(message);

    	if(init & 0x01)
    	{
    		pwr_ready = 1;
    		break;
    	}
    }


    	if(pwr_ready)
    	{
    		UartPuts("Init Process Successfull\n\r");

    		max30100_write(mode_config, spo2_only);
    		DelayMs(50);
    		UartPuts("Step 1 Complete...Mode Config\n\r");

    		max30100_write(spo2_config,0x5C);
    		DelayMs(50);
    		UartPuts("Step 2 Complete...Spo2 Config\n\r");

    		max30100_write(led_config, 0x59);
    		DelayMs(50);
    		UartPuts("Step 2 Complete...Led current\n\r");

    		UartPuts("Max30100 init successfully\n\r");
    	}
    	else
    	{
    		//print uart fail to power on
    		UartPuts("Fail to init Max30100\n\r");
    	}
}

void max30100_read_fifo(void)
{

	UartPuts("Inside fifo read\n\r");

	for(int i = 0; i < 16; i++)
	{
		uint8_t data[4];

		// Read 4 bytes from FIFO data register (0x05)
		I2CStart();
		I2CSendSlaveAddr(MAX30100_Write_ADDR);  // Write address
		I2CSendData(MAX30100_FIFO_DATA);        // FIFO data register
		I2CRepeatStart();
		I2CSendSlaveAddr(MAX30100_Read_ADDR);   // Read address

		// Receive 4 bytes: IR (MSB, LSB), RED (MSB, LSB)
		data[0] = I2CRecvDataAck();  // IR MSB
		data[1] = I2CRecvDataAck();  // IR LSB
		data[2] = I2CRecvDataAck();  // RED MSB
		data[3] = I2CRecvDataNAck(); // RED LSB (last byte)
		I2CStop();

		// Combine high and low bytes
        ir_buffer[i]  = ((uint16_t)data[0] << 8) | data[1];
        red_buffer[i] = ((uint16_t)data[2] << 8) | data[3];

	}

    // Print to UART
    char buf[64];

    sprintf(buf, "Collected 16 samples from FIFO\n\r");
    UartPuts(buf);
}

float calculate_spo2(void) {
    uint32_t dc_ir = 0, dc_red = 0;
    uint16_t ir_min = 0xFFFF, ir_max = 0;
    uint16_t red_min = 0xFFFF, red_max = 0;
    int i;

    // Calculate DC averages and find AC peaks/troughs
    for (i = 0; i < NUM_SAMPLES; i++) {
        uint16_t ir = ir_buffer[i];
        uint16_t red = red_buffer[i];

        dc_ir += ir;
        dc_red += red;

        if (ir < ir_min) ir_min = ir;
        if (ir > ir_max) ir_max = ir;

        if (red < red_min) red_min = red;
        if (red > red_max) red_max = red;
    }

    // Check for "no finger" condition based on low DC signal
    // If the average DC signal is too low, it means there's no object
    // blocking the light, so the reading is invalid.
    if ((dc_ir / NUM_SAMPLES) < NO_FINGER_THRESHOLD || (dc_red / NUM_SAMPLES) < NO_FINGER_THRESHOLD) {
        UartPuts("No finger detected. Returning 0.\n");
        return 0.0f; // Return 0.0 for no finger
    }

    float avg_ir = dc_ir / (float)NUM_SAMPLES;
    float avg_red = dc_red / (float)NUM_SAMPLES;

    float ac_ir = ir_max - ir_min;
    float ac_red = red_max - red_min;

    if (ac_ir == 0.0f || avg_ir == 0.0f || avg_red == 0.0f)
        return -1.0f; // Invalid reading

    float R = (ac_red / avg_red) / (ac_ir / avg_ir);
    float spo2 = 110.0f - 25.0f * R;

    if (spo2 > 100.0f) {
        spo2 = 100.0f;
    }
    
    // Discard data less than 85%
    if (spo2 < 85.0f) {
        UartPuts("Invalid SpO2 data (less than 85%). Returning 0.\n");
        return 0.0f;
    }
    
    return spo2;
}


void max30100_read_and_calculate_spo2(void) {
    max30100_read_fifo();

    UartPuts("Return from max30100_read_spo2\n\r");

    float spo2 = calculate_spo2();
    DelayMs(200);

    int spo2_int = (int)(spo2 * 100);  // e.g., 97.56 → 9756
    DelayMs(200);

    if (spo2 > 0.0f) // Check for a valid positive value
    {
        static char buf[64];
        sprintf(buf,"SpO2 = %d.%02d\n\r", spo2_int / 100, spo2_int % 100);
        UartPuts(buf);
        DelayMs(200);
    }
    else if (spo2 < 0.0f)
    {
        UartPuts("Invalid SpO2 Data\n\r");
    }
}









