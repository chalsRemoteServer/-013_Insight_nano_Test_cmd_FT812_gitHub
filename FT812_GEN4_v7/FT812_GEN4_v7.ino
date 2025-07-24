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
#include "Config_Hardware.h"
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
  #ifdef DEBUG_RS232
    Serial.begin(9600);   // Initialize UART for debugging messages
  #endif
  SPI.begin();                        // Start SPI settings
  delay(3000);
  d=80;
  n=3000/(d*2);
  pinMode(LED_BUILTIN, OUTPUT);   
  init_WakeUp_FT800_a();
  init_TFT_display_b();//Initialize Display 
  Serial.println("4 ");
  /* Configure Touch and Audio - not used in this example, so disable both */
  ft812memWrite8(REG_TOUCH_MODE, ZERO);       // Disable touch
  ft812memWrite16(REG_TOUCH_RZTHRESH, ZERO);  // Eliminate any false touches
  
  ft812memWrite8(REG_VOL_PB, ZERO);           // turn recorded volume down
  ft812memWrite8(REG_VOL_SOUND, ZERO);        // turn synthesizer volume down
  ft812memWrite16(REG_SOUND, 0x6000);         // set synthesizer to mute  
  /* End of Configure Touch and Audio */
  Serial.println(" 5 ");
   init_Vars_TFT_display();
   Serial.println("6 ");
   init_Parametros_TFT_display();
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
