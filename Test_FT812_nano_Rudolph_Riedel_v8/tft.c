/*
@file    tft.c
@brief   TFT handling functions for EVE_Test project
@version 1.25
@date    2025-04-20
@author  Rudolph Riedel
@section History

1.17
- replaced the UTF-8 font with a freshly generated one and adjusted the parameters for the .xfont file
1.18
- several minor changes
1.19
- removed most of the history
- changed a couple of "while (EVE_busy()) {};" lines to "EVE_execute_cmd();"
- renamed PINK to MAGENTA
1.20
- several minor changes
1.21
- several minor changes
1.22
- added touch calibration values for EVE_GD3X
1.23
- added using EVE_calibrate_write() / EVE_calibrate_read()
- improved the calibration output for screens of higher resolutions
- changed TFT_display() to not use the special _burst() functions between EVE_start_cmd_burst() and EVE_end_cmd_burst(),
  this does the same, is a tiny bit slower, but should work better as a first example
- replaced several EVE_cmd_dl() calls with calls to dedicated functions
- added a second version of TFT_display(), one is for AVR and uses the EVE_cmd_xxx_burst() functions,
  the second is for all other architectures which do not benefit as much from using these functions,
1.25
- first minimum changes for BT820
 */

#include "EVE.h"
#include "EVE_supplemental.h"
#include "tft_data.h"
#include "tft.h"
#include "colores.h"
#include "TFTdisplay.h"

#define TEST_UTF8 0


/* some pre-definded colors */
/*#define RED     0xff0000UL
#define ORANGE  0xffa500UL
#define GREEN   0x00ff00UL
#define BLUE    0x0000ffUL
#define BLUE_1  0x5dade2L
#define YELLOW  0xffff00UL
#define MAGENTA 0xff00ffUL
#define PURPLE  0x800080UL
#define WHITE   0xffffffUL
#define BLACK   0x000000UL
*/

/* memory-map defines */
#define MEM_FONT 0x000f7e00 /* the .xfont file for the UTF-8 font is copied here */
#define MEM_LOGO 0x000f8000 /* start-address of logo, needs 6272 bytes of memory */
#define MEM_PIC1 0x000fa000 /* start of 100x100 pixel test image, ARGB565, needs 20000 bytes of memory */

#define MEM_DL_STATIC (EVE_RAM_G_SIZE - 4096) /* 0xff000 - start-address of the static part of the display-list, upper 4k of gfx-mem */

uint32_t num_dl_static = 0; /* amount of bytes in the static part of our display-list */
uint8_t tft_active = 0;
//uint16_t num_profile_a = 0;
//uint16_t num_profile_b = 0;
uint16_t toggle_state = 0;
uint16_t horas,minutos,segundos,mseg,display_list_size = 0;

#define LAYOUT_Y1 66


void touch_calibrate(void);
void initStaticBackground(void);


void touch_calibrate(void)
{
/* send pre-recorded touch calibration values, depending on the display the code is compiled for */
/* do not trust these values, I got these from calibrating one display */

#if defined (VM820C_1024600)
    EVE_calibrate_write(0x0000fd55, 0xfffffc64, 0x0006f496, 0xfffffef1, 0x000106f4, 0xfff794c4);
#endif

#if defined (EVE_CFAF240400C1_030SC)
    EVE_calibrate_write(0x0000ed11, 0x00001139, 0xfff76809, 0x00000000, 0x00010690, 0xfffadf2e);
#endif

#if defined (EVE_CFAF320240F_035T)
    EVE_calibrate_write(0x00005614, 0x0000009e, 0xfff43422, 0x0000001d, 0xffffbda4, 0x00f8f2ef);
#endif

#if defined (EVE_CFAF480128A0_039TC)
    EVE_calibrate_write(0x00010485, 0x0000017f, 0xfffb0bd3, 0x00000073, 0x0000e293, 0x00069904);
#endif

#if defined (EVE_CFAF800480E0_050SC)
    EVE_calibrate_write(0x000107f9, 0xffffff8c, 0xfff451ae, 0x000000d2, 0x0000feac, 0xfffcfaaf);
#endif

#if defined (EVE_CFAF800480E1_050SC_A2)
    EVE_calibrate_write(0x00010603, 0x0000007A, 0xFFFF0525, 0x0000060B, 0x00010E13, 0xFFE8813F);
#endif

#if defined (EVE_PAF90)
    EVE_calibrate_write(0x00000159, 0x0001019c, 0xfff93625, 0x00010157, 0x00000000, 0x0000c101);
#endif

#if defined (EVE_RiTFT43)
    EVE_calibrate_write(0x000062cd, 0xfffffe45, 0xfff45e0a, 0x000001a3, 0x00005b33, 0xFFFbb870);
#endif

#if defined (EVE_EVE2_38)
    EVE_calibrate_write(0x00007bed, 0x000001b0, 0xfff60aa5, 0x00000095, 0xffffdcda, 0x00829c08);
#endif

#if defined (EVE_EVE2_35G) ||  defined (EVE_EVE3_35G)
    EVE_calibrate_write(0x000109E4, 0x000007A6, 0xFFEC1EBA, 0x0000072C, 0x0001096A, 0xFFF469CF);
#endif

#if defined (EVE_EVE2_43G) ||  defined (EVE_EVE3_43G)
    EVE_calibrate_write(0x0000a1ff, 0x00000680, 0xffe54cc2, 0xffffff53, 0x0000912c, 0xfffe628d);
#endif

#if defined (EVE_EVE2_50G) || defined (EVE_EVE3_50G)
    EVE_calibrate_write(0x000109E4, 0x000007A6, 0xFFEC1EBA, 0x0000072C, 0x0001096A, 0xFFF469CF);
#endif

#if defined (EVE_EVE2_70G)
    EVE_calibrate_write(0x000105BC, 0xFFFFFA8A, 0x00004670, 0xFFFFFF75, 0x00010074, 0xFFFF14C8);
#endif

#if defined (EVE_NHD_35)
    EVE_calibrate_write(0x0000f78b, 0x00000427, 0xfffcedf8, 0xfffffba4, 0x0000f756, 0x0009279e);
#endif

#if defined (EVE_RVT70)
    EVE_calibrate_write(0x000074df, 0x000000e6, 0xfffd5474, 0x000001af, 0x00007e79, 0xffe9a63c);
#endif

#if defined (EVE_FT811CB_HY50HD)
    EVE_calibrate_write(66353, 712, 4293876677, 4294966157, 67516, 418276);
#endif

#if defined (EVE_ADAM101)
    EVE_calibrate_write(0x000101E3, 0x00000114, 0xFFF5EEBA, 0xFFFFFF5E, 0x00010226, 0x0000C783);
#endif

#if defined (EVE_GD3X)
    EVE_calibrate_write(0x0000D09D, 0xFFFFFE27, 0xFFF0838A, 0xFFFFFF72, 0xFFFF7D2B, 0x01F3096A);
#endif

/* activate this if you are using a module for the first time or if you need to re-calibrate it */
/* write down the numbers on the screen and either place them in one of the pre-defined blocks above or make a new block */
#if 0
    /* calibrate touch and displays values to screen */

#if 1
    uint8_t font_size = 27;
    uint8_t mod_posx = 0;
    uint8_t mod_posy = 0;

    if (EVE_HSIZE > 480)
    {
        font_size = 29;
        mod_posx = 130;
        mod_posy = 10;
    }

    EVE_cmd_dlstart();
    EVE_clear_color_rgb(BLACK);
    EVE_clear(1, 1, 1);
    EVE_cmd_text((EVE_HSIZE/2), 50, font_size, EVE_OPT_CENTER, "Please tap on the dot.");
    EVE_cmd_calibrate();
    EVE_display();
    EVE_cmd_swap();
    EVE_execute_cmd();
#else
    EVE_calibrate_manual(EVE_HSIZE, EVE_VSIZE);
#endif

    uint32_t touch_a;
    uint32_t touch_b;
    uint32_t touch_c;
    uint32_t touch_d;
    uint32_t touch_e;
    uint32_t touch_f;

    EVE_calibrate_read(&touch_a, &touch_b, &touch_c, &touch_d, &touch_e, &touch_f);

    EVE_cmd_dlstart();
    EVE_clear_color_rgb(BLACK);
    EVE_clear(1, 1, 1);
    EVE_tag(0);

    EVE_cmd_text(5, 10, font_size, 0, "TOUCH_TRANSFORM_A:");
    EVE_cmd_text(5, 30+(mod_posy * 1), font_size, 0, "TOUCH_TRANSFORM_B:");
    EVE_cmd_text(5, 50+(mod_posy * 2), font_size, 0, "TOUCH_TRANSFORM_C:");
    EVE_cmd_text(5, 70+(mod_posy * 3), font_size, 0, "TOUCH_TRANSFORM_D:");
    EVE_cmd_text(5, 90+(mod_posy * 4), font_size, 0, "TOUCH_TRANSFORM_E:");
    EVE_cmd_text(5, 110+(mod_posy * 5), font_size, 0, "TOUCH_TRANSFORM_F:");

    EVE_cmd_setbase(16L);
    EVE_cmd_number(290+mod_posx, 10, font_size, EVE_OPT_RIGHTX|8, touch_a);
    EVE_cmd_number(290+mod_posx, 30+(mod_posy * 1), font_size, EVE_OPT_RIGHTX|8, touch_b);
    EVE_cmd_number(290+mod_posx, 50+(mod_posy * 2), font_size, EVE_OPT_RIGHTX|8, touch_c);
    EVE_cmd_number(290+mod_posx, 70+(mod_posy * 3), font_size, EVE_OPT_RIGHTX|8, touch_d);
    EVE_cmd_number(290+mod_posx, 90+(mod_posy * 4), font_size, EVE_OPT_RIGHTX|8, touch_e);
    EVE_cmd_number(290+mod_posx, 110+(mod_posy * 5), font_size, EVE_OPT_RIGHTX|8, touch_f);

    EVE_display();
    EVE_cmd_swap();
    EVE_execute_cmd();

    while(1);
#endif
}


void initStaticBackground(void){
    EVE_cmd_dlstart(); /* Start the display list */
    EVE_tag(0); /* tag = 0 - do not use the following objects for touch-detection */
    EVE_cmd_bgcolor(WHITE); /* light grey */
    EVE_vertex_format(_FRAC_PRESICION); /* set to 0 - reduce precision for VERTEX2F to 1 pixel instead of 1/16 pixel default */

    /* PANEL TOP_PRTAL INICIO,,---------- draw a rectangle on top */
    EVE_begin(EVE_RECTS);
    EVE_line_width(1U*PRESICION); /* size is in 1/16 pixel */
    EVE_color_rgb(COLOR_PANEL_TOP_PORTAL_INICIO);
    EVE_vertex2f(0, 0); /* set to 0 / 0 */
    EVE_vertex2f(EVE_HSIZE*PRESICION,Y_PANEL_TOP*PRESICION);
    EVE_end();

    /* display the logo */
    EVE_color_rgb(BLUE);
    EVE_begin(EVE_BITMAPS);
    EVE_cmd_setbitmap(MEM_LOGO, EVE_ARGB1555, 56U, 56U);
    EVE_vertex2f(X_LOGO*PRESICION,Y_LOGO*PRESICION); //EVE_HSIZE - 78, 5);
    EVE_end();

    /* draw a black line to separate things */
    EVE_color_rgb(DARK_ORANGE);
    EVE_begin(EVE_LINES);
    EVE_vertex2f(0,Y_PANEL_TOP*PRESICION);
    EVE_vertex2f(EVE_HSIZE*PRESICION,Y_PANEL_TOP*PRESICION);
    EVE_end();

    EVE_color_rgb(BLACK); 
    EVE_cmd_text(X_USER*PRESICION, Y_USER*PRESICION, USER_FONT_SIZE, EVE_OPT_CENTERX, "Ningun Usuario");
    EVE_cmd_text(X_USER*PRESICION+1, Y_USER*PRESICION+1, USER_FONT_SIZE, EVE_OPT_CENTERX, "Ningun Usuario");
    

    /* add the static text to the list */
    
    EVE_cmd_text_bold(80,Y_PANEL_TOP+25, PRODUCT_FONT_SIZE, 0, "1: Product 1");
    display_Param_Punto_1();
    display_Graphica_Signal();//Dibuja la Grafica de Signal.
    display_Selector_de_Puntos(0,1);//dibuja la Parte de Seleccion de Puntos

    EVE_execute_cmd();
    num_dl_static = EVE_memRead16(REG_CMD_DL);
    EVE_cmd_memcpy(MEM_DL_STATIC, EVE_RAM_DL, num_dl_static);
    EVE_execute_cmd();

}//--------------------------------------------------------------


void TFT_init(void){
    if(E_OK == EVE_init()){
        tft_active = 1;horas=12,minutos=0;segundos=0;mseg=0;
        EVE_memWrite32(REG_PWM_DUTY, 0x30);  /* setup backlight, range is from 0 = off to 0x80 = max */
        touch_calibrate();
        EVE_cmd_inflate(MEM_LOGO, logo, sizeof(logo)); /* load logo into gfx-memory and de-compress it */
        EVE_cmd_loadimage(MEM_PIC1, EVE_OPT_NODL, pic, sizeof(pic));
        initStaticBackground();
    }//------------------------------------------------------------------
}//-----------------------------------------------------------------------------------------------------


/* check for touch events and setup vars for TFT_display() */
void TFT_touch(void)
{if(tft_active != 0){
        uint32_t tag;
        static uint8_t toggle_lock = 0;

        if(EVE_IS_BUSY == EVE_busy()){ /* is EVE still processing the last display list? */
             return;}
        display_list_size = EVE_memRead16(REG_CMD_DL); /* debug-information, get the size of the last generated display-list */
        tag = EVE_memRead32(REG_TOUCH_TAG); /* read the value for the first touch point */
        switch(tag){
            case 0:toggle_lock = 0;break;
            case 10:if(0 == toggle_lock){/* use button on top as on/off toggle-switch */
                        toggle_lock = 42;
                        if(0 == toggle_state){
                             toggle_state = EVE_OPT_FLAT;}
                        else{toggle_state = 0;}}
                    break;
            default:break;}}
}//++++++++++++++++++++++++++++++++++++++++++++++++++++++

/*dynamic portion of display-handling, meant to be called every 20ms or more*/
#if defined (__AVR__)
/*we are running on 8-bit without DMA,
 optimize some more by using the special EVE_cmd_xxx_burst() functions*/
void TFT_display(void)
{static int32_t rotate = 0;
    if(tft_active != 0U)
    {EVE_start_cmd_burst(); /* start writing to the cmd-fifo as one stream of bytes, only sending the address once */
     EVE_cmd_dlstart_burst(); /* start the display list */
     EVE_clear_color_rgb_burst(MEDIUM_GRAY); /* set the default clear color to white */
     EVE_clear_burst(1, 1, 1); /* clear the screen - this and the previous prevent artifacts between lists, Attributes are the color, stencil and tag buffers */
     EVE_tag_burst(0); /* no touch */
     EVE_cmd_append_burst(MEM_DL_STATIC, num_dl_static); /* insert static part of display-list from copy in gfx-mem */

    /* display a button  boton de pruebas  mover aqui*/
     EVE_color_rgb_burst(LIME_GREEN);
     EVE_cmd_fgcolor_burst(ORANGE); /* some grey */
     EVE_tag_burst(10); /* assign tag-value '10' to the button that follows */
     EVE_cmd_button_burst(20,115,80,30, 28, toggle_state,"Touch!");
     EVE_tag_burst(0); /* no touch */

    /* display a picture and rotate it when the button on top is activated */
     EVE_cmd_setbitmap_burst(MEM_PIC1, EVE_RGB565, 100U, 100U);


        if(toggle_state != 0U){rotate += 256;}

        EVE_begin_burst(EVE_BITMAPS);
        EVE_vertex2f_burst(EVE_HSIZE - 100, LAYOUT_Y1);
        EVE_end_burst();

        simulador_de_reloj(&horas,&minutos,&segundos,&mseg);
        display_Reloj(horas,minutos,segundos,mseg);
        
        EVE_display_burst(); /* mark the end of the display list */
        EVE_cmd_swap_burst(); /* make this list active */
        EVE_end_cmd_burst(); /* stop writing to the cmd-fifo, the cmd-FIFO will be executed automatically after this or when DMA is done */
    }
}
#else
void TFT_display(void)
{}
#endif