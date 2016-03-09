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

#ifndef HTU21D_REGISTER_H
#define HTU21D_REGISTER_H

#define TRIG_TEMP_HOLD		0xE3 //Triggers a Temperature Measurement. Holds the SCK line (blocks i2c bus) until measurement complete.
#define TRIG_HUM_HOLD		0xE5 //Triggers a Humidity Measurement. Holds the SCK line (blocks i2c bus) until measurement complete.
#define TRIG_TEMP_RLS		0xF3 //Triggers a Temperature Measurement. Releases the SCK line (unblocks i2c bus). User must manually wait for completion before grabbing data.
#define TRIG_HUM_RLS		0xF5 //Triggers a Humidity Measurement. Releases the SCK line (unblocks i2c bus). User must manually wait for completion before grabbing data.
#define USER_REG_WRITE		0xE6 //Write to user register (refer to datasheet)
#define USER_REG_READ		0xE7 //Reads from user register (refer to datasheet)
#define SOFT_RESET			0xFE //Performs a soft-reset. Requires 15mS max. 

//TODO: add command registers for reading serial number (refer to separate document as not covered in datasheet)

//User Reg Masks. Obtain User Reg Byte then "USER_REG_BYTE &= UREG_XYZ" to get relevant status bit. Refer to datasheet for more info.
#define UREG_RES	0x81 //Measurement Resolution. 0x00 = 12bit RH / 14bit TEMP; 0x01 = 8bit RH / 12bit TEMP; 0x80 = 10bit RH / 13bit TEMP; 0x81 = 11bit RH / 11bit TEMP
#define UREG_BATT	0x40 //Low Battery Alarm - 0 = VDD>2.25V, 1 = VDD<2.25V
#define UREG_HEAT	0x04 //On-chip heater. 1=enabled | 0=disabled (default)
#define UREG_OTP	0x02 //Disable OTP reload. 1=OTP_reload_disabled (default) | 0=OTP_reload_enabled
//bits 3,4,5 are reserved (unused)


#endif

