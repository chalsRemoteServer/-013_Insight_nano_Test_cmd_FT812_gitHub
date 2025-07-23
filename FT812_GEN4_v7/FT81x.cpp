#include <stdint.h>
#include "FT81x.h"
#include "FT812.h"
/**Cell number. Cell number is the index of the bitmap with same bitmap layout and
format. For example, for handle 31, the cell 65 means the character "A" in built in font  31.  */
uint32_t VERTEX2II(uint16_t x,uint16_t y,uint8_t handle,uint8_t cell){
  
    x      &= 0x1FF;  // 9 bits // Máscaras para asegurar que los valores están dentro del rango permitido
    y      &= 0x1FF;  // 9 bits
    handle &= 0x1F;   // 5 bits
    cell   &= 0x7F;   // 7 bits
  uint32_t cmd = DL_VERTEX2II            // Opcode
                 | ((uint32_t)x << 21)     // Bits 29–21: X
                 | ((uint32_t)y << 12)     // Bits 20–12: Y
                 | ((uint32_t)handle << 7) // Bits 11–7: Handle
                 | (uint32_t)cell;         // Bits 6–0: Cell
return cmd;
}//fin de vertex 2 ii+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Input one or more vertices using “VERTEX2II” or “VERTEX2F”, which specify the placement of the primitive on the screen
//The VERTEX2F command gives the location of the circle center
/*BITS[31 30]= 0X01=DL_VERTEX2F=0x40000000UL [29 15]=X [14 0]=Y  */
uint32_t VERTEX2F(uint16_t x, uint16_t y) {
    x &= 0x7FFF;  // 15 bits para X
    y &= 0x7FFF;  // 15 bits para Y
    uint32_t cmd = DL_VERTEX2F       // opcode en bits 31-30 = 0x01 << 30
                 | ((uint32_t)x << 15)  // bits 29-15: X
                 | (uint32_t)y;         // bits 14-0 : Y
    return cmd;
}//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



/* DL_COLOR_RGB=0x04000000UL
 BITS[31 24] =0x04 (OPCODE) [23 16]=RED [15 8]=BLUE [7 0]=GREEN  */
uint32_t COLOR_RGB(uint8_t red,uint8_t green,uint8_t blue){
     return DL_COLOR_RGB
         | ((uint32_t)blue << 16)
         | ((uint32_t)green << 8)
         | (uint32_t)red;
}//fin de color rgb ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*Sets the size of drawn points. The width is the distance from the center of the point to the outermost
drawn pixel, in units of 1/16 pixels.
Examples The second point is drawn with a width of 160, for a 10-pixel radius:*/
/* bits[31 24]=DL_POINT_SIZE=0x0D  [23 13]=reserved [12 0]=size */
uint32_t POINT_SIZE(uint16_t size){
    size &= 0x1FFF;  // Limitar a 13 bits por seguridad (0x1FFF = 8191)
    return DL_POINT_SIZE | (uint32_t)size;
}// Specify the radius of points ++++++++++++++++++++++++++++++++++++++++++++
