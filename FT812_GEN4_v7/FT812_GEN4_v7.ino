// Example built on top of AN_275 FT800 Example with Arduino
// for unit testing purposes.

/* This program shows the FTDI Chip Logo animation on
 * GEN4-FT812-43T resistive screen. Touch and sound are
 * disabled. */

// Author: R. Abrante Delgado
// Date: 26/05/2021

  /* The Teensy 3.2 I/O pins are connected to the GEN4-FT812-43T as follows:
      Digital
        Pin 0 = n/c  (out)      Pin 8    = n/c
        Pin 1 = n/c  (in)       Pin 9    = n/c
        Pin 2 = n/c             Pin 10   = CS#  (out)
        Pin 3 = INT# (in)       Pin 11   = MOSI (out)
        Pin 4 = PD#  (out)      Pin 12   = MISO (in)
        Pin 5 = n/c             Pin 13   = SCK  (out)
        pin 6 = n/c             Pin GND  = GND
        pin 7 = n/c             Pin AREF = n/c
        
      Analog and power headers = n/c
  */

/* Include Files */

#include <Arduino.h>    // Arduino definitions
#include <SPI.h>        // Arduino SPI Library definitions
#include "FT812.h"      // FT812 register, memory and command values
#include "FT81x.h"
#include "TFT.h"




extern unsigned long ramDisplayList;
extern  unsigned int SPI_CS;      // SPI chip select
extern unsigned int SPI_PD;      // Power Down from Teensy to FT812


void setup() {
  int i,n,d;  
  #ifdef ARDUINO_NANO
    //triggerPin = 2;     
    //SPI_INT = 3;       
    SPI_CS = 9;         
    SPI_PD= 3;
  #else 
    SPI_CS= 9;
    SPI_PD= 3;
  #endif
  


  Serial.begin(9600);   // Initialize UART for debugging messages
  SPI.begin();                        // Start SPI settings
  delay(3000);
  d=80;
  n=3000/(d*2);
  pinMode(LED_BUILTIN, OUTPUT);
  for(i=0;i<n;i++){
     digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
     delay(d);                       // wait for a second
     digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
     delay(d);
     }   
  Serial.println("\n\n\n 00");
  pinMode(triggerPin, OUTPUT);        // Oscilloscope triggering
  pinMode(SPI_INT, INPUT_PULLUP);     // FT812 interrupt output
  pinMode(SPI_PD, OUTPUT);            // FT812 Power Down (reset) input
  pinMode(SPI_CS, OUTPUT);            // FT812 SPI bus CS# input
   
  digitalWrite(triggerPin, LOW);      // Initialize the oscilloscope trigger
  digitalWrite(SPI_CS, HIGH);         // Set CS# high to start - SPI inactive
  digitalWrite(SPI_PD, HIGH);         // Set PD# high to start
  delay(20);                          // Wait a few MS before waking the FT812
  Serial.println(" 0");
  /* Wake-up FT812 */
  digitalWrite(SPI_PD, LOW);          // 1) lower PD#
  delay(20);                          // 2) hold for 20ms
  digitalWrite(SPI_PD, HIGH);         // 3) raise PD#
  delay(20);                          // 4) wait before sending any commands
  Serial.println(" 1");
  ft812cmdWrite(FT812_ACTIVE);        // Start FT812
  delay(300);                         // Give some time to process
  
  // Now FT812 can accept commands at up to 30MHz clock on SPI bus
  // This application leaves the SPI bus at 10MHz
  Serial.println(" 2 ");
  while (i != 0x7C) // Read ID register - is it 0x7C?
  {  i=ft812memRead8(REG_ID);
    // If we don't get 0x7C, the interface isn't working - halt with inf. loop
   Serial.println(i);
   }
  Serial.println("3 ");
  ft812memWrite8(REG_PCLK, ZERO);       // Set PCLK to zero
                                        // Don't clock the LCD until later
  ft812memWrite8(REG_PWM_DUTY, ZERO);   // Turn off backlight
  /* End of Wake-up FT800 */
  init_TFT_display();//Initialize Display 
  Serial.println("4 ");
  /* Configure Touch and Audio - not used in this example, so disable both */
  ft812memWrite8(REG_TOUCH_MODE, ZERO);       // Disable touch
  ft812memWrite16(REG_TOUCH_RZTHRESH, ZERO);  // Eliminate any false touches
  
  ft812memWrite8(REG_VOL_PB, ZERO);           // turn recorded volume down
  ft812memWrite8(REG_VOL_SOUND, ZERO);        // turn synthesizer volume down
  ft812memWrite16(REG_SOUND, 0x6000);         // set synthesizer to mute
  /* End of Configure Touch and Audio */
  Serial.println(" 5 ");
  /* Write Initial Display List & Enable Display */
  ramDisplayList = RAM_DL;                            // start of Display List
  // Clear Color RGB   00000010 RRRRRRRR GGGGGGGG BBBBBBBB
  // (R/G/B = Colour values) default zero / black
  ft812memWrite32(ramDisplayList, DL_CLEAR_RGB);      
  ramDisplayList += 4;                                // point to next location
  // Clear 00100110 -------- -------- -----CST
  // (C/S/T define which parameters to clear)
  ft812memWrite32(ramDisplayList, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));  
  ramDisplayList += 4;                                // point to next location
  // DISPLAY command 00000000 00000000 00000000 00000000 (end of display list)
  ft812memWrite32(ramDisplayList, DL_DISPLAY);                                
  // 00000000 00000000 00000000 000000SS  (SS bits define when render occurs)
  ft812memWrite32(REG_DLSWAP, DLSWAP_FRAME);                                  
  // Nothing is being displayed yet... the pixel clock is still 0x00
  ramDisplayList = RAM_DL;          // Reset Display List pointer for next list
   Serial.println("6 ");
  // Read the FT800 GPIO register for a read/modify/write operation
  ft812Gpio = ft812memRead8(REG_GPIO);
  // set bit 7 of FT800 GPIO register (DISP) - others are inputs
  ft812Gpio = ft812Gpio | 0x80;
  // Enable the DISP signal to the LCD panel        
  ft812memWrite8(REG_GPIO, ft812Gpio);
  // Now start clocking data to the LCD panel
  ft812memWrite8(REG_PCLK, lcdPclk);   
  Serial.println("7 ");
  for(int duty = 0; duty <= 128; duty++)
  {
    // Turn on backlight - ramp up slowly to full brighness
    ft812memWrite8(REG_PWM_DUTY, duty);   
    delay(10);
  }
  Serial.println("8 ");
  /* End of Write Initial Display List & Enable Display */

} // END of setup()


/******************************************************************************
 * Function:        void loop(void)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        Repetitive activities - Draw a dot on the LCD
 *                  Continually change colors from white to red and back
 * Note:            Default Arduino function
 *****************************************************************************/
void loop() 
{
  Serial.write("Inicia loop\n");
  uint32_t offset = 0;
  ft812memWrite8(REG_ROTATE, 1);
  ft812memWrite32(RAM_DL + offset, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));offset += 4; //cmdOffset = incCMDOffset(cmdOffset, 4); // Start the display list      
  ft812memWrite32(RAM_DL + offset, DL_BEGIN | BITMAPS );offset += 4;// bitmap=1
  ft812memWrite32(RAM_DL + offset, VERTEX2II(220,110,31,'F'));offset += 4; 
  ft812memWrite32(RAM_DL + offset, VERTEX2II(244,110,31,'T'));offset += 4; 
  ft812memWrite32(RAM_DL + offset, VERTEX2II(270,110,31,'D'));offset += 4; 
  ft812memWrite32(RAM_DL + offset, VERTEX2II(299,110,31,'I'));offset += 4; 
  ft812memWrite32(RAM_DL + offset, DL_END);offset += 4;

  ft812memWrite32(RAM_DL + offset, COLOR_RGB(128,0,0));offset += 4;//cambia color a rojo;
  ft812memWrite32(RAM_DL + offset, POINT_SIZE(5*16));offset += 4;// pone el tamaño del punto a 20 pixeles de radio
  ft812memWrite32(RAM_DL + offset, DL_BEGIN | POINTS );offset += 4; //POINTS=2, start drawing points
  ft812memWrite32(RAM_DL + offset, VERTEX2F(30*16,17*6));offset += 4; //red point 
  ft812memWrite32(RAM_DL + offset, COLOR_RGB(0,128,0));offset += 4;//cambia color a rojo;
  ft812memWrite32(RAM_DL + offset, POINT_SIZE(8*16));offset += 4;// pone el tamaño del punto a 20 pixeles de radio
  ft812memWrite32(RAM_DL + offset, VERTEX2F(90*16,17*6));offset += 4;
  
  ft812memWrite32(RAM_DL + offset, COLOR_RGB(0,0,128));offset += 4;//cambia color a rojo;
  ft812memWrite32(RAM_DL + offset, POINT_SIZE(10*16));offset += 4;// pone el tamaño del punto a 20 pixeles de radio
  ft812memWrite32(RAM_DL + offset, VERTEX2F(30*16,51*6));offset += 4;
  
  ft812memWrite32(RAM_DL + offset, COLOR_RGB(128,128,0));offset += 4;//cambia color a rojo;
  ft812memWrite32(RAM_DL + offset, POINT_SIZE(13*16));offset += 4;// pone el tamaño del punto a 20 pixeles de radio
  ft812memWrite32(RAM_DL + offset, VERTEX2F(90*16,51*6));offset += 4;
  
  ft812memWrite32(RAM_DL + offset, DL_BEGIN | LINES );offset += 4; //POINTS=2, start drawing points
  ft812memWrite32(RAM_DL + offset, COLOR_RGB(255,100,255));offset += 4;//cambia color a rojo;
  ft812memWrite32(DL_LINE_WIDTH , 255*16 );
  ft812memWrite32(RAM_DL + offset, VERTEX2F(400*16,0*16));offset += 4;
  ft812memWrite32(RAM_DL + offset, VERTEX2F(400*16,463*6));offset += 4;
 


  ft812memWrite32(RAM_DL + offset, DL_END);offset += 4;

  ft812memWrite32(RAM_DL + offset, DL_DISPLAY);offset += 4;// Final de la lista//display the image. End the display list. All the commands following this command will be ignored. 
  ft812memWrite8(REG_DLSWAP, DLSWAP_FRAME);  // // Cambia la lista activa o DLSWAP_LINE si prefieres

  Serial.write("fin de loop\n");
  delay(3000);         // Wait for animation to end (around 2.5 seconds)
  while(1){
    
  }
} // End of loop()-------------------------------------------------------------------

/** EOF GEN4.ino ****************************************/
