

#include "EVE.h"
#include "colores.h"
#include "TFTdisplay.h"


/* Pinta las lineas que forma el plano 
 cartesiano +X+Y del PortalInicio-punto1*/
void display_Grafica_Signal_ejes(void){
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

/*despliega las letras de la grafica de Portal inicio del punto-1 */
void display_letras_de_Grafica_Signal(void){
    EVE_cmd_text_bold(45,Y_NUMS_GRAPH, USER_FONT_SIZE, 0, "300");
    EVE_cmd_text_bold(45,Y_NUMS_GRAPH+GAP2*1, USER_FONT_SIZE, 0, "200");
    EVE_cmd_text_bold(45,Y_NUMS_GRAPH+GAP2*2, USER_FONT_SIZE, 0, "100");
    EVE_cmd_text_bold(45+28,Y_NUMS_GRAPH+GAP2*3, USER_FONT_SIZE,0, "0");
    EVE_cmd_text_bold(X_LabelParameter-50, Y_METAL_SIGNAL+GAP1*4, USER_FONT_SIZE, 0, "0s");
    EVE_cmd_text_bold(X_LabelParameter-200, Y_METAL_SIGNAL+GAP1*4, USER_FONT_SIZE, 0, "-60s");
}//fin display de letras de graficas signal-----------------------

//Despliega la seleccion de Seleccion de Puntos
/* pametro status: 0:Ninguno {Boton Apretado Presionado:[1:Izquierdo,2:derecho]} 
   parameter: Puntos: {1-6} <-Punto seleccionado */
void display_Selector_de_Puntos(uint8_t status,uint8_t punto){
     display_btn_Select(status);
     display_Puntos(punto);
}// fin de seleccion de Puntos-----------------------------------

/*despliega  los puntos gris y verde la seccion seleccion de Puntos de Portal Inicio*/
void display_Puntos(uint8_t status){
      
}//fin de dispaly de puntos-----------------------------------------------------------------

/* Despliega los botones de seleccion de puntos de portal inicio*/
void display_btn_Select(uint8_t punto){


}


//Pinta la Grafica de Señal de Portal Inicio del Punto-1
void display_Graphica_Signal(void){
    display_letras_de_Grafica_Signal();//dibuja las letras de la grafica
    display_Grafica_Signal_ejes();  //dibujar los ejes de la grafica
}//fin.------------------------------------------------------------   

/**Despliega el texto de los parametros del Punto uno al lado de la grafica */
void display_Param_Punto_1(void){
    EVE_cmd_text_bold(X_LabelParameter,Y_METAL_SIGNAL , USER_FONT_SIZE, 0, "Metal signal");
    EVE_cmd_text_bold(X_LabelParameter, Y_METAL_SIGNAL+GAP1*1, USER_FONT_SIZE, 0, "0%");
    EVE_cmd_text_bold(X_LabelParameter, Y_METAL_SIGNAL+GAP1*2, USER_FONT_SIZE, 0, "Audicheck");
    EVE_cmd_text_bold(X_LabelParameter, Y_METAL_SIGNAL+GAP1*3, USER_FONT_SIZE, 0, "External");
}//fin de display parametros punto 1-------------------------------------------------------------

/** simula un reloj actualizando las variables del tiempo
   la interrupcion es cada 25mseg "segun"*/
void simulador_de_reloj(uint16_t *horas,uint16_t *minutos,uint16_t *segundos,uint16_t *mseg){
    *mseg += 25;
    if (*mseg >= 1000) {
        *mseg -= 1000;(*segundos)++;
        if (*segundos >= 60) {
            *segundos = 0;(*minutos)++;
            if (*minutos >= 60) {
               *minutos = 0;(*horas)++;
               if (*horas >= 24) {
                     *horas = 0;}}}}
}//fin de simulador de reloj------------------------------------------------------------------------


/* fin de despliegue de tiempo den pantlla */
void display_Reloj(uint16_t horas,uint16_t minutos,uint16_t segundos,uint16_t mseg){
const     
    EVE_color_rgb_burst(BLACK);
    EVE_cmd_number_burst(580, 13, 22, EVE_OPT_CENTERX, horas); /* number of bytes written to the display-list by the command co-pro */
    EVE_cmd_text_bold(580,Y_METAL_SIGNAL ,24, 0, " : ");
    EVE_cmd_text_bold(580,Y_METAL_SIGNAL ,24, 0, " : ");
    EVE_cmd_text_bold(580,Y_METAL_SIGNAL ,24, 0, " : ");
    EVE_cmd_number_burst(704, 15, 22, EVE_OPT_CENTERX, minutos); /* duration in us of TFT_loop() for the touch-event part */
    EVE_cmd_text_bold(X_LabelParameter,Y_METAL_SIGNAL ,22, 0, " : ");
    EVE_cmd_text_bold(X_LabelParameter,Y_METAL_SIGNAL ,22, 0, " : ");
    EVE_cmd_text_bold(X_LabelParameter,Y_METAL_SIGNAL ,22, 0, " : ");
    EVE_cmd_number_burst(554, 10, 22, EVE_OPT_CENTERX, segundos); /* duration in us of TFT_loop() for the display-list part */
    EVE_cmd_text_bold(X_LabelParameter,Y_METAL_SIGNAL ,22, 0, " : ");
    EVE_cmd_text_bold(X_LabelParameter,Y_METAL_SIGNAL ,22, 0, " : ");
    EVE_cmd_text_bold(X_LabelParameter,Y_METAL_SIGNAL ,22, 0, " : ");
    EVE_cmd_number_burst(6+0, 10+0, 20, EVE_OPT_CENTERX, mseg); /* duration in us of TFT_loop() for the display-list part */
    EVE_cmd_number_burst(554+1, 10+1, 20, EVE_OPT_CENTERX, mseg);

}//fin de despliegue de tiempo en pantalla----------------------------------------