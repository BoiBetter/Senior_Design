/* Copyright (C) 2015 Hoa Nguyen. All rights reserved.

 This software may be distributed and modified under the terms of the GNU
 General Public License version 2 (GPL2) as published by the Free Software
 Foundation and appearing in the file GPL2.TXT included in the packaging of
 this file. Please note that GPL2 Section 2[b] requires that all works based
 on this software must also be made publicly available under the terms of
 the GPL2 ("Copyleft").

 Contact information
 -------------------

 Hoa Nguyen 
 Email: hoa.nguyen12349@yahoo.com
*/
#include "signalAD9833Serial.h"

/* Description: Initialize an instance of signalAD9833. 
   Input: Digital pins that will control w_clk, FU_UD, data, and reset on the sinewave generator AD9850. Also include the desired output frequency. 
   Example: data_pin = P2_5, clock_pin = P2_4, CS_pin = P1_5, freq = 60
   Output: None 
*/
signalAD9833::signalAD9833(int data_pin, int clock_pin, int CS_pin, uint32_t freq){
	pinMode(CS_pin, OUTPUT);
	digitalWrite(CS_pin, HIGH); 
	pinMode(data_pin, OUTPUT);
	pinMode(clock_pin, OUTPUT);

	__CS_pin = CS_pin; 
	__data_pin = data_pin;
	__clock_pin = clock_pin;
/*	if ( freq == 60){
		//__freq = 0b01000000000000000100001010000100; //Use for when clock is 25MHz
		__freq = 0b01000000000000000111111011101010;   //Use for when clock is 1MHz
	}
	else if(freq == 97){
		//__freq = 0b01000000000000000100010000010010; //Use for when clock is 25MHz
		__freq = 0b01000000000000010110010110110110; // Use for when clock is 1MHz
	}
	else{*/
		update_freq(freq); 
	//}

	
}

/*	Description: Transfer data to the freq register of AD9833. 
	Input: None. The freq data is taken from the object.
	Output: None.   
*/
void signalAD9833::transfer_freq_data(){ 
	shiftOut(__data_pin, __clock_pin, MSBFIRST, set_mask >> 8); 
	digitalWrite(__clock_pin, HIGH); 


	shiftOut(__data_pin, __clock_pin, MSBFIRST, set_mask); 
	digitalWrite(__clock_pin, HIGH); 


	shiftOut(__data_pin, __clock_pin, MSBFIRST, __freq >> 8); 
	digitalWrite(__clock_pin, HIGH); 


	shiftOut(__data_pin, __clock_pin, MSBFIRST, __freq); 
	digitalWrite(__clock_pin, HIGH); 


	shiftOut(__data_pin, __clock_pin, MSBFIRST, __freq >> 24); 
	digitalWrite(__clock_pin, HIGH); 

 
	shiftOut(__data_pin, __clock_pin, MSBFIRST, __freq >> 16); 
	digitalWrite(__clock_pin, HIGH); 

	shiftOut(__data_pin, __clock_pin, MSBFIRST, phase_mask >> 8); 
	digitalWrite(__clock_pin, HIGH); 


	shiftOut(__data_pin, __clock_pin, MSBFIRST, phase_mask); 
	digitalWrite(__clock_pin, HIGH); 

}

/*	Description: Set Chip Select(CS) low for data transfer. 
	Input: None.
	Output: None.   
*/
void signalAD9833::begin_signal(){
    digitalWrite(__CS_pin,LOW);
    
}

/* This function turns on the sinewave generator AD9850. 
   Input: None 
   Output: None 
*/
void signalAD9833::start_signal(){
	begin_signal(); 
	transfer_freq_data();

	shiftOut(__data_pin, __clock_pin, MSBFIRST, start_mask >> 8); 
	digitalWrite(__clock_pin, HIGH); 

	shiftOut(__data_pin, __clock_pin, MSBFIRST, start_mask); 
	digitalWrite(__clock_pin, HIGH); 
	digitalWrite(__CS_pin, HIGH); 
	
}

/* This function turns off the sinewave generator AD9850. 
   Input: None 
   Output: None 
*/
void signalAD9833::stop_signal(){
	shiftOut(__data_pin, __clock_pin, MSBFIRST, stop_mask >> 8); 
	digitalWrite(__clock_pin, HIGH); 


	shiftOut(__data_pin, __clock_pin, MSBFIRST, stop_mask); 
	digitalWrite(__clock_pin, HIGH); 

	
	digitalWrite(__CS_pin, HIGH); 

}

/* This function updates the frequency 
   Input: frequency of type unsigned long
   Output: None 
*/
void signalAD9833::update_freq(uint32_t freq){
	if(freq >0){
		float f = (freq * 268435456.0)/AD9833_clock;
		__freq = (uint32_t) f;  
		uint32_t high = (__freq << 2) & 0b00111111111111110000000000000000;   
		uint32_t low =  __freq &0b00000000000000000011111111111111; 
		__freq = high | low | freq_mask;

	} 

}
	