

/* Declarations */


// FT800 Chip Commands - use with cmdWrite
#define FT812_ACTIVE  0x00      // Initializes FT812
#define FT812_STANDBY 0x41      // Place FT812 in Standby (clk running)
#define FT812_SLEEP   0x42      // Place FT812 in Sleep (clk off)
#define FT812_PWRDOWN 0x50      // Place FT812 in Power Down (core off)
#define FT812_CLKEXT  0x44      // Select external clock source
#define FT812_CLKINT  0x48      // Select internal relaxation oscillator
#define FT812_CLK48M  0x62      // Select 48MHz PLL
#define FT812_CLK36M  0x61      // Select 36MHz PLL
#define FT812_CORERST 0x68      // Reset core - all registers default

// FT812 Memory Commands - use with ft812memWritexx and ft812memReadxx
#define MEM_WRITE 0x80      // FT812 Host Memory Write 
#define MEM_READ  0x00      // FT812 Host Memory Read

// Colors - fully saturated colors defined here
/*#define RED     0xFF0000UL    // Red
#define GREEN   0x00FF00UL    // Green
#define BLUE    0x0000FFUL    // Blue
#define WHITE   0xFFFFFFUL    // White
#define BLACK   0x000000UL   */ // Black

#define RED     0x0000FFUL   // rojo (en orden FT812)
#define GREEN   0x00FF00UL   // verde
#define BLUE    0xFF0000UL   // azul
#define WHITE   0xFFFFFFUL
#define BLACK   0x000000UL
#define YELLOW  0x00FFFFUL   // rojo + verde (amarillo)


void ft812memWrite8(unsigned long ftAddress, unsigned char ftData8);
void ft812memWrite16(unsigned long ftAddress, unsigned int ftData16);
void ft812memWrite32(unsigned long ftAddress, unsigned long ftData32);
unsigned char ft812memRead8(unsigned long ftAddress);
unsigned int ft812memRead16(unsigned long ftAddress);
unsigned long ft812memRead32(unsigned long ftAddress);
void ft812cmdWrite(unsigned char ftCommand);
unsigned int incCMDOffset(unsigned int currentOffset, unsigned char commandSize);
void init_TFT_display_b(void);
void init_Parametros_TFT_display(void);
void init_Vars_TFT_display(void);
void init_SPI_controls_wires_00(void);
void init_Encender_TFT_display_0(void);
void init_Start_TFT_display_1(void);
void init_WakeUp_FT800_a(void);