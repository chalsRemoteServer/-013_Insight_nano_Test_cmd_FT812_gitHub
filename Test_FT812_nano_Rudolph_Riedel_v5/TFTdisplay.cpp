

#include "EVE.h"
#include "colores.h"
#include "TFTdisplay.h"


/* Pinta las lineas que forma el plano 
 cartesiano +X+Y del PortalInicio-punto1*/
void display_Grafica_Signal(void){
    EVE_begin(EVE_RECTS);//LINEA VERTICAL
    EVE_line_width(3U*PRESICION); /* size is in 1/16 pixel */
    EVE_color_rgb(CHARCOAL_GRAY);
    EVE_vertex2f(X_VERT_GRAPH_P0*PRESICION,Y_VERT_GRAPH_P0*PRESICION); /* set to 0 / 0 */
    EVE_vertex2f(X_VERT_GRAPH_P1*PRESICION,Y_VERT_GRAPH_P1*PRESICION);
    //**LINEA HORIZONTAL-----------------
    EVE_vertex2f(X_VERT_GRAPH_P1*PRESICION,Y_VERT_GRAPH_P1*PRESICION); /* set to 0 / 0 */
    EVE_vertex2f(X_VERT_GRAPH_P2*PRESICION,Y_VERT_GRAPH_P1*PRESICION);
    EVE_end();
}//-------------------------------------------------------


