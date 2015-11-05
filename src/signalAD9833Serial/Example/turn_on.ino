#include <signalAD9833Serial.h>

#define sine_data P2_5       // Use for data pin of the AD9833
#define sine_clock P2_4      // Use for clock pin of AD9833
#define sine_CS P1_5         // Use for chip select of AD9833


const int sine_freq = 97;    // This controls the frequency output of the AD9833 in Hz.  
signalAD9833 sine(sine_data, sine_clock, sine_CS, sine_freq);  // Call to create an AD9833 object  

void setup()
{

  sine_turn_on(); 
  
}

void loop()
{
  
}

void sine_turn_on()
{
  sine.start_signal();  
}

// Turn off the AD9833
void sine_turn_off()
{
  sine.stop_signal();
}
