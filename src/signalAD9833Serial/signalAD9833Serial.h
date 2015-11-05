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
#ifndef signalAD9833Serial_h
#define signalAD9833Serial_h

#include "Energia.h"
//#include <SPI.h>

#define freq_mask 0b01000000000000000100000000000000
#define set_mask 0b0010000100001000
#define start_mask 0b0000000000001000
#define stop_mask 0b0000000011001000
#define phase_mask 0b1100000000000000
#define AD9833_clock 1000000

class signalAD9833
{
	public:

		signalAD9833(int data_pin, int clock_pin, int CS_pin, uint32_t freq); 
		void start_signal(); 
		void stop_signal(); 
		void update_freq(uint32_t freq); 

	private:
		void begin_signal();
		void transfer_freq_data(); 
		uint32_t __freq;
		int __CS_pin;
		int __data_pin;
		int __clock_pin;   
		
}; 

#endif; 