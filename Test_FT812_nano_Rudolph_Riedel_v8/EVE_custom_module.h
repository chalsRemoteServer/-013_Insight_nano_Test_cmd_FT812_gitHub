#ifndef EVE_CUSTOM_MODULE_H
#define EVE_CUSTOM_MODULE_H


//CONSTANTES DE ESTADO 
#define  ON 1
#define OFF 0

//CONSTANTES PANTALLA ROTACION Rotacion de la pantalla completa+++++++++++++++
#define  LANDSCAPE 0x00 // sin inversion ni mirror
#define  PORTRAIT  0x02 // rotado 90°
#define  INVERT    0x01 //invertido vertical
#define  MIRRORED  0x08 // 
//*CONSTANTES CONFIG PRESICION DE PIXELES******************************************************
#define _FRAC_PRES_PIX0  0 //   1    unidad de precisión   Rango: -16384 a 16383   e.g. VERTEX2F(100, 200) = (100px, 200px)
#define _FRAC_PRES_PIX1  1 // 1/2    unidad de precisión   Rango: -8192 a 8191     e.g. VERTEX2F(100*2, 200*2) = (100.0px, 200.0px)
#define _FRAC_PRES_PIX2  2 // 1/4    unidad de precisión   Rango: -4096 a 4095     e.g. VERTEX2F(100*4, 200*4) = (100.0px, 200.0px)
#define _FRAC_PRES_PIX3  3 // 1/8    unidad de precisión   Rango: -2048 a 2047     e.g. VERTEX2F(100*8, 200*8) = (100.0px, 200.0px)
#define _FRAC_PRES_PIX4  4 // 1/16   unidad de precisión   Rango: -1024 a 1023     e.g. VERTEX2F(100*16, 200*16) = (100.0px, 200.0px)
#define _FRAC_PRESICION _FRAC_PRES_PIX0
#if _FRAC_PRESICION == 0
  #define PRESICION_ 1
#elif _FRAC_PRESICION == 1
  #define PRESICION_ 2
#elif _FRAC_PRESICION == 2
  #define PRESICION_ 4
#elif _FRAC_PRESICION == 3
  #define PRESICION_ 8
#elif _FRAC_PRESICION == 4
  #define PRESICION_ 16
#endif
//----------------------------------------------------------------------------------------------------------------


#define EVE_ROTATE INVERT    // invertir o no la pantalla 
#define PRESICION  PRESICION_  //PRESICION PIXEL

#endif