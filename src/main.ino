#include <SD.h>
#include <SPI.h>
#include <signalAD9833Serial.h>

#define SD_LED RED_LED       // Use to signify whether or not SD card is present. On means card is not present.  
#define ACTIVE_LED GREEN_LED // Use to signify whethere or not the MSP430 is active. On means MSP430 is active. 
#define SD_CS P2_6           // Use for chip select of SD card
#define sine_data P2_5       // Use for data pin of the AD9833
#define sine_clock P2_4      // Use for clock pin of AD9833
#define sine_CS P1_5         // Use for chip select of AD9833
#define remote_switch P1_3   // Use for remote switch of the microcontroller

const int sine_freq = 60;    // This controls the frequency output of the AD9833 in Hz.  
signalAD9833 sine(sine_data, sine_clock, sine_CS, sine_freq);  // Call to create an AD9833 object  

const uint16_t sample_num = 550; // Number of sample to be collected
volatile uint16_t current_sample = 0;   // Need to be volatile so the compiler will not optimize this variable     
uint16_t sample_write = 0; 
const char * file_name [] = {"Data0.txt", "Data1.txt", "Data2.txt", "Data3.txt", "Data4.txt", "Data5.txt", "Data6.txt"}; // Names of datafile   

int data [sample_num][6];    // Data buffer 
int max_file_num = 7;        // Number of unique files 
int next_file = 0;           // Use to point to the next file name
String space = "      ";  
String column_name = "EX1" + space + "EX2" + space + "EY1" + space + "EY2" + space + "EZ1" + space + "EZ2" + space;   

const int samp_rate = 3000;   // Use to change the sampling rate of the ADC module 
const int timer_value = 32768/samp_rate;  // Value for TimerB  

void setup()
{ 
  pinMode(SD_LED, OUTPUT);
  pinMode(ACTIVE_LED, OUTPUT);
  pinMode(SD_CS, OUTPUT); 
  
  digitalWrite(ACTIVE_LED, HIGH); 
  while (!SD.begin(SD_CS)){        // Check for the present of an SD card
    digitalWrite(SD_LED, HIGH);     
  }  
  digitalWrite(SD_LED,LOW);  
  delay(1000);
  
  pinMode(remote_switch,INPUT_PULLDOWN);  
  attachInterrupt(remote_switch, turn_on, RISING);  // Pin P2_5 is used to receive external interrupt signal. Can be changed to any other digital pin.  
  digitalWrite(ACTIVE_LED, LOW); 
  suspend();         //MSP430 enters LP4

}

void loop()
{
  digitalWrite(ACTIVE_LED, HIGH); 
  sine_turn_on(); 
  delay(200);  //200ms delay so the AD9833 output can stabilize
  timer_setup(); 
  ADC_setup(); 
  while(current_sample < sample_num){}
  //write_data();
  timer_stop(); 
  write_data(); 
  sine_turn_off(); 
   
  attachInterrupt(remote_switch, turn_on, RISING);
  digitalWrite(ACTIVE_LED, LOW);   
  suspend();  
}

/* This interrupt will be used to turn on the whole entire board from LP4. 
It also disables itself to prevent another similar interrupt from running while 
the main loop is running. 
*/
void turn_on()
{ 
  detachInterrupt(remote_switch);   
  wakeup();  
    
}

// Turn on the AD9833
void sine_turn_on()
{
  sine.start_signal();  
}

// Turn off the AD9833
void sine_turn_off()
{
  sine.stop_signal();
}

/* This functions write all the data into one datafile. If no new data file can be created, the function will loop around and start appending sequentially 
to the first, second, third, ect.. data file. 
*/
void write_data()
{
  File dataFile = SD.open(file_name[next_file], FILE_WRITE);
  dataFile.println(column_name); 

  while ( current_sample < sample_num | sample_write < current_sample)
  {
    if (sample_write < current_sample)
    {
      for(int k = 0; k < 6; k ++)
      {
         dataFile.print(String(data[sample_write][k]) + space);    
      }
      dataFile.println();
      sample_write ++; 
    }
  }
  dataFile.close(); 
  sample_write = 0;
  current_sample = 0; 
  next_file++;
  next_file%= max_file_num;  
}

// Setup the TimerB to trigger the ADC module
void timer_setup(){
    P5SEL |= BIT7;                            // P5.7/TB1 option select
    P5DIR |= BIT7;                            // Output direction
    
    //Setup Timer B0
    TBCCR0 = timer_value;
    TBCCR1 = 0x0002;
    TBCCTL1 = OUTMOD_3;                       // CCR1 set/reset mode
    TBCTL = TBSSEL_1+MC_1+TBCLR;              // ACLK, Up-Mode
}

// Stop TimerB
void timer_stop()
{
  TBCTL = MC_0;  
}

// Setup the ADC module to collect data in sequency from analog channel 0, 1, 2, 3, 4, and 12. 
void ADC_setup()
{
  ADC12CTL0 = ADC12SHT0_4+ADC12MSC+ADC12ON;// Sampling time, MSC, ADC12 on
  ADC12CTL1 = ADC12SHS_3+ADC12CONSEQ_1 + ADC12SSEL_0 + ADC12SHP;     // Use sampling timer; ADC12MEM0
                                          
                                            // TBCCR1 output
                                            // Sequence-channel
  ADC12MCTL0 = ADC12SREF_0+ADC12INCH_0;     // V+=AVcc V-=AVss, A0 channel
  ADC12MCTL1 = ADC12SREF_0+ADC12INCH_1;
  ADC12MCTL2 = ADC12SREF_0+ADC12INCH_2;
  ADC12MCTL3 = ADC12SREF_0+ADC12INCH_3;
  ADC12MCTL4 = ADC12SREF_0+ADC12INCH_4;
  ADC12MCTL5 = ADC12SREF_0+ADC12INCH_12 + ADC12EOS;  // End of channel A12
  
  ADC12IE = 0x0020;         //enable interrupt
  ADC12CTL0 |= ADC12ENC;  
}

// ADC interrupt that will transfer ADC data from the ADCMEMx memory to the data buffer
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
  data[current_sample][0] = ADC12MEM0 & 0x0FFF;    //lower 12 bit result
  data[current_sample][1] = ADC12MEM1 & 0x0FFF;
  data[current_sample][2] = ADC12MEM2 & 0x0FFF;
  data[current_sample][3] = ADC12MEM3 & 0x0FFF;
  data[current_sample][4] = ADC12MEM4 & 0x0FFF;
  data[current_sample][5] = ADC12MEM5 & 0x0FFF;
  
  ADC12CTL0 &= ~ADC12ENC;
  current_sample++; 
  if( current_sample < sample_num)
 {
    ADC12CTL0 |= ADC12ENC;
 }
}


