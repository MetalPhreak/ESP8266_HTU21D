/*
* The MIT License (MIT)
* 
* Copyright (c) 2016 David Ogilvy (MetalPhreak)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "osapi.h" //required for os_delay_us
#include "driver/htu21d.h"
#include "driver/i2c.h"




void HTU21D_writeUserReg(uint8 data){
	i2c_start();
	i2c_writeByte(0x80); //0x40 address << 1 + 0x00 Write
	i2c_check_ack(); 
	i2c_writeByte(USER_REG_WRITE); //Write User Register Command
	i2c_check_ack();
	i2c_writeByte(data);
	i2c_check_ack();
	i2c_stop();
}

uint8 HTU21D_readUserReg(){
	uint8 data;
	i2c_start();
	i2c_writeByte(0x80); //0x40 address << 1 + 0x00 Write
	if(!i2c_check_ack()) { return 0; }  
	i2c_writeByte(USER_REG_READ); //Read User Register Command
	if(!i2c_check_ack()) { return 0; } 
	i2c_start();
	i2c_writeByte(0x81); //0x40 address << 1 + 0x01 Read
	if(!i2c_check_ack()) { return 0; } 
	data = i2c_readByte();
	i2c_send_ack(0); //NACK
	i2c_stop();
	return data;
}

void HTU21D_softReset(){
	i2c_start();
	i2c_writeByte(0x80); //0x40 address << 1 + 0x00 Write
	i2c_check_ack(); 
	i2c_writeByte(SOFT_RESET); //Write User Register Command
	i2c_check_ack();
	i2c_stop();
}

float HTU21D_temperature(float *measurement) {
	uint8 msb, lsb, crc = 0;
	float val = 0;
	i2c_start();
	i2c_writeByte(0x80); //0x40 address << 1 + 0x00 Write
	if(!i2c_check_ack()) { return 0; } 
	i2c_writeByte(TRIG_TEMP_RLS); //Write User Register Command
	if(!i2c_check_ack()) { return 0; } 
	os_delay_us(50000); //wait 50mS
	i2c_start();
	i2c_writeByte(0x81); //0x40 address << 1 + 0x01 Read
	if(!i2c_check_ack()) { return 0; } 
	msb = i2c_readByte();
	i2c_send_ack(1);
	lsb = i2c_readByte();
	i2c_send_ack(1);
	crc = i2c_readByte();
	i2c_send_ack(0);
	i2c_stop();

	//os_printf("MSB: %02x LSB: %02x CRC: %02x\n", msb, lsb, crc);

	//TODO Check CRC Value and exit if transmission errors

	val = msb<<8 | lsb;
  	val *= 175.72;
  	val /= 65536;
  	val -= 46.85;
	*measurement = val;
	return 1;
}

float HTU21D_humidity(float *measurement) {
	uint8 msb, lsb, crc = 0;
	float val = 0;
	i2c_start();
	i2c_writeByte(0x80); //0x40 address << 1 + 0x00 Write
	if(!i2c_check_ack()) { return 0; } 
	i2c_writeByte(TRIG_HUM_RLS); //Write User Register Command
	if(!i2c_check_ack()) { return 0; } 
	os_delay_us(16000); //wait 16mS
	i2c_start();
	i2c_writeByte(0x81); //0x40 address << 1 + 0x01 Read
	if(!i2c_check_ack()) { return 0; } 
	msb = i2c_readByte();
	i2c_send_ack(1);
	lsb = i2c_readByte();
	i2c_send_ack(1);
	crc = i2c_readByte();
	i2c_send_ack(0);
	i2c_stop();

	//os_printf("MSB: %02x LSB: %02x CRC: %02x\n", msb, lsb, crc);

	//TODO Check CRC Value and exit if transmission errors

	val = msb<<8 | lsb;
 	val *= 125;
	val /= 65536;
	val -= 6;
	*measurement = val;
	return 1;
}


#ifndef FTOS_C
#define FTOS_C

//simple function converts from a float to a string. Decimal precision of 2.
//change the four 100s to 1000 for 3 decimal points along with the sprintf to %03u
//thanks to petoknm for the example.

void ICACHE_FLASH_ATTR ftos(float val, char *buff) {
    char smallBuff[16];
    int val1 = (int) val;
    unsigned int val2;
    if (val < 0) {
        val2 = (int) (-100 * val) % 100;
    } else {
        val2 = (int) (100 * val) % 100;
    }
    os_sprintf(smallBuff, "%i.%02u", val1, val2);

    os_strcpy(buff, smallBuff);
}

#endif










