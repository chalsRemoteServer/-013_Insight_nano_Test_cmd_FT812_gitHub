#include <Arduino.h>    // Arduino definitions
#include <SPI.h>        // Arduino SPI Library definitions
#include "TFT.h"
#include "FT812.h"      // FT812 register, memory and command values


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


void init_TFT_display(void){
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
