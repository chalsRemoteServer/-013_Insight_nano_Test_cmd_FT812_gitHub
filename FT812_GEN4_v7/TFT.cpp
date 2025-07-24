#include <Arduino.h>    // Arduino definitions
#include <SPI.h>        // Arduino SPI Library definitions
#include "TFT.h"
#include "FT812.h"      // FT812 register, memory and command values
#include "Config_Hardware.h"


/* Global Variables */

// Arduino pins - others defined by Serial and SPI libraries
unsigned int triggerPin;  // Oscilloscope/logic analyzer trigger
unsigned int SPI_INT;     // Interrupt from FT812 to Teensy
unsigned int SPI_PD;      // Power Down from Teensy to FT812
unsigned int SPI_CS;      // SPI chip select

// SPI Settings
SPISettings GEN4(SPI_SPEED, MSBFIRST, SPI_MODE0);

// LCD display parameters
unsigned int lcdWidth;        // Active width of LCD display
unsigned int lcdHeight;       // Active height of LCD display
unsigned int lcdHcycle;       // Total number of clocks per line
unsigned int lcdHoffset;      // Start of active line
unsigned int lcdHsync0;       // Start of horizontal sync pulse
unsigned int lcdHsync1;       // End of horizontal sync pulse
unsigned int lcdVcycle;       // Total number of lines per screen
unsigned int lcdVoffset;      // Start of active screen
unsigned int lcdVsync0;       // Start of vertical sync pulse
unsigned int lcdVsync1;       // End of vertical sync pulse
unsigned char lcdPclk;        // Pixel Clock
unsigned char lcdSwizzle;     // Define RGB output pins
unsigned char lcdPclkpol;     // Define active edge of PCLK

unsigned long ramDisplayList = RAM_DL;    // Beginning of display list memory
unsigned long ramCommandBuffer = RAM_CMD; // Beginning of graphics cmd memory

unsigned int cmdBufferRd = 0x0000;    // Used to navigate command ring buffer
unsigned int cmdBufferWr = 0x0000;    // Used to navigate command ring buffer
unsigned int cmdOffset = 0x0000;      // Used to navigate command rung buffer
unsigned char ft812Gpio;              // Used for FT812 GPIO register


/******************************************************************************
   Function:        void ft812memWritexx(ftAddress, ftDataxx, ftLength)
   PreCondition:    None
   Input:           ftAddress = FT812 memory space address
                    ftDataxx = a byte, int or long to send
   Output:          None
   Side Effects:    None
   Overview:        Writes FT812 internal address space
   Note:            "xx" is one of 8, 16 or 32
 *****************************************************************************/
void ft812memWrite8(unsigned long ftAddress, unsigned char ftData8)
{
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE);  
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer(ftData8);                          // Send data byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
}

void ft812memWrite16(unsigned long ftAddress, unsigned int ftData16)
{
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE);  
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer((char)(ftData16));                 // Send data low byte
  SPI.transfer((char)(ftData16 >> 8));            // Send data high byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
}

void ft812memWrite32(unsigned long ftAddress, unsigned long ftData32)
{
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_WRITE);  
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer((char)(ftData32));                 // Send data low byte
  SPI.transfer((char)(ftData32 >> 8));
  SPI.transfer((char)(ftData32 >> 16));
  SPI.transfer((char)(ftData32 >> 24));           // Send data high byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
}


/******************************************************************************
 * Function:        unsigned char ft812memReadxx(ftAddress, ftLength)
 * PreCondition:    None
 * Input:           ftAddress = FT812 memory space address
 * Output:          ftDataxx (byte, int or long)
 * Side Effects:    None
 * Overview:        Reads FT812 internal address space
 * Note:            "xx" is one of 8, 16 or 32
 *****************************************************************************/
unsigned char ft812memRead8(unsigned long ftAddress)
{
  unsigned char ftData8 = ZERO;
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ);   
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer(ZERO);                             // Send dummy byte
    ftData8 = SPI.transfer(ZERO);                 // Read data byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
  return ftData8;                                 // Return byte read
}

unsigned int ft812memRead16(unsigned long ftAddress)
{
  unsigned int ftData16;
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ);
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer(ZERO);                             // Send dummy byte
    ftData16 = (SPI.transfer(ZERO));              // Read low byte
    ftData16 = ((SPI.transfer(ZERO) << 8) | ftData16);  // Read high byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
  return ftData16;                                // Return integer read
}

unsigned long ft812memRead32(unsigned long ftAddress)
{
  unsigned long ftData32;
  SPI.beginTransaction(GEN4);                     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);                      // Set CS# low
  // Send Memory Write plus high address byte
  SPI.transfer((char)(ftAddress >> 16) | MEM_READ);   
  SPI.transfer((char)(ftAddress >> 8));           // Send middle address byte
  SPI.transfer((char)(ftAddress));                // Send low address byte
  SPI.transfer(ZERO);                             // Send dummy byte
    ftData32 = (SPI.transfer(ZERO));                  // Read low byte
    ftData32 = (SPI.transfer(ZERO) << 8) | ftData32;
    ftData32 = (SPI.transfer(ZERO) << 16) | ftData32;
    ftData32 = (SPI.transfer(ZERO) << 24) | ftData32; // Read high byte
  digitalWrite(SPI_CS, HIGH);                     // Set CS# high
  SPI.endTransaction();                           // End transaction for GEN4
  return ftData32;                                // Return long read
}


/******************************************************************************
 * Function:        void ft812cmdWrite(ftCommand)
 * PreCondition:    None
 * Input:           None
 * Output:          None
 * Side Effects:    None
 * Overview:        Sends FT812 command
 * Note:            None
 *****************************************************************************/
void ft812cmdWrite(unsigned char ftCommand)
{
  SPI.beginTransaction(GEN4);     // Begin transaction for GEN4
  digitalWrite(SPI_CS, LOW);      // Set CS# low
  SPI.transfer(ftCommand);        // Send command
  SPI.transfer(0x00);             // Commands consist of two more zero bytes
  SPI.transfer(0x00);             // Send last zero byte
  digitalWrite(SPI_CS, HIGH);     // Set CS# high
  SPI.endTransaction();           // End transaction for GEN4
}

/******************************************************************************
 * Function:        void incCMDOffset(currentOffset, commandSize)
 * PreCondition:    None
 *                    starting a command list
 * Input:           currentOffset = graphics processor command list pointer
 *                  commandSize = number of bytes to increment the offset
 * Output:          newOffset = new ring buffer pointer after adding the cmd
 * Side Effects:    None
 * Overview:        Adds commandSize to the currentOffset.  
 *                  Checks for 4K ring-buffer offset roll-over 
 * Note:            None
 *****************************************************************************/
unsigned int incCMDOffset(unsigned int currentOffset, unsigned char commandSize)
{
    unsigned int newOffset;                   // used to hold new offset
    newOffset = currentOffset + commandSize;  // Calculate new offset
    if(newOffset > 4095)                      // If new offset past boundary...
    {
        newOffset = (newOffset - 4096);       // ... roll over pointer
    }
    return newOffset;                         // Return new offset
}//----------------------------------------------------------------------------


void init_TFT_display_b(void){
#ifdef LCD_MICROTIPS      // WQVGA display parameters
    lcdWidth   = 800;   // Active width of LCD display
    lcdHeight  = 480;   // Active height of LCD display
    lcdHcycle  = 928;   // Total number of clocks per line
    lcdHoffset = 88;    // Start of active line
    lcdHsync0  = 0;     // Start of horizontal sync pulse
    lcdHsync1  = 48;    // End of horizontal sync pulse
    lcdVcycle  = 525;   // Total number of lines per screen
    lcdVoffset = 32;    // Start of active screen
    lcdVsync0  = 0;     // Start of vertical sync pulse
    lcdVsync1  = 3;    // End of vertical sync pulse
    lcdPclk    = 2;     // Pixel Clock
    lcdSwizzle = 0;     // Define RGB output pins
    lcdPclkpol = 1;     // Define active edge of PCLK
  #endif
/* Initialize Display */
  ft812memWrite16(REG_HSIZE,   lcdWidth);   // active display width
  ft812memWrite16(REG_HCYCLE,  lcdHcycle);  // total number of clocks per line
  ft812memWrite16(REG_HOFFSET, lcdHoffset); // start of active line
  ft812memWrite16(REG_HSYNC0,  lcdHsync0);  // start of horizontal sync pulse
  ft812memWrite16(REG_HSYNC1,  lcdHsync1);  // end of horizontal sync pulse
  ft812memWrite16(REG_VSIZE,   lcdHeight);  // active display height
  ft812memWrite16(REG_VCYCLE,  lcdVcycle);  // total number of lines per screen
  ft812memWrite16(REG_VOFFSET, lcdVoffset); // start of active screen
  ft812memWrite16(REG_VSYNC0,  lcdVsync0);  // start of vertical sync pulse
  ft812memWrite16(REG_VSYNC1,  lcdVsync1);  // end of vertical sync pulse
  ft812memWrite8(REG_SWIZZLE,  lcdSwizzle); // FT800 output to LCD - pin order
  ft812memWrite8(REG_PCLK_POL, lcdPclkpol); // Data clocked on this PCLK edge
  // Don't set PCLK yet - wait for just after the first display list
  /* End of Initialize Display */


}//-------------------------------------------------------------------------------

void init_Parametros_TFT_display(void){
 // Read the FT800 GPIO register for a read/modify/write operation
  ft812Gpio = ft812memRead8(REG_GPIO);
  // set bit 7 of FT800 GPIO register (DISP) - others are inputs
  ft812Gpio = ft812Gpio | 0x80;
  // Enable the DISP signal to the LCD panel        
  ft812memWrite8(REG_GPIO, ft812Gpio);
  // Now start clocking data to the LCD panel
  ft812memWrite8(REG_PCLK, lcdPclk);  
}//fin de parametros del TFT display+-------------------------------------------- 


void init_Vars_TFT_display(void){
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
}//-------------------------------------------------------------------------------

void init_SPI_controls_wires_00(void){
#ifdef DEBUG_RS232
     Serial.println(" 00");
#endif
//pinMode(triggerPin, OUTPUT);        // Oscilloscope triggering
  //pinMode(SPI_INT, INPUT_PULLUP);     // FT812 interrupt output
  pinMode(SPI_PD, OUTPUT);            // FT812 Power Down (reset) input
  pinMode(SPI_CS, OUTPUT);            // FT812 SPI bus CS# input 
  //digitalWrite(triggerPin, LOW);      // Initialize the oscilloscope trigger
  digitalWrite(SPI_CS, HIGH);         // Set CS# high to start - SPI inactive
  digitalWrite(SPI_PD, HIGH);         // Set PD# high to start
  delay(20);                          // Wait a few MS before waking the FT812
#ifdef DEBUG_RS232
  Serial.println(" 0");
#endif
}//fin init spi controls lineas de control de SPI---------------------------------


void  init_Encender_TFT_display_0(void){
  /* Wake-up FT812 */
  digitalWrite(SPI_PD, LOW);          // 1) lower PD#
  delay(20);                          // 2) hold for 20ms
  digitalWrite(SPI_PD, HIGH);         // 3) raise PD#
  delay(20);                          // 4) wait before sending any commands
#ifdef DEBUG_RS232
  Serial.println(" 1");
#endif
}//fin --------------------------------------------------------------------------

void  init_Start_TFT_display_1(void){
  ft812cmdWrite(FT812_ACTIVE);        // Start FT812
  delay(300);                         // Give some time to process
  // Now FT812 can accept commands at up to 30MHz clock on SPI bus
  // This application leaves the SPI bus at 10MHz
#ifdef DEBUG_RS232
  Serial.println(" 2");
#endif
}//fin---------------init_Start_TFT_display-----------------------------------------------------------

//Encender leds de error aqui--------------------------
void init_Read_ID_TFT_display_2(void){
int i;
  while (i != 0x7C) // Read ID register - is it 0x7C?
  {  i=ft812memRead8(REG_ID);// If we don't get 0x7C, the interface isn't working - halt with inf. loop
     /*Serial.println(i);*/}
#ifdef DEBUG_RS232
  Serial.println(" 3");
#endif
}//----------------------------------------------------------------------------

void init_WakeUp_FT800_a(void){
  init_SPI_controls_wires_00();
  init_Encender_TFT_display_0();
  init_Start_TFT_display_1();
  init_Read_ID_TFT_display_2();
  ft812memWrite8(REG_PCLK, ZERO);       // Set PCLK to zero  // Don't clock the LCD until later
  ft812memWrite8(REG_PWM_DUTY, ZERO);   // Turn off backlight
  /* End of Wake-up FT800 */
}//fin void ft800+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
