/**
@file    TFTdisplay.h
@brief   contains FT812  functions
@version 2.3
@date    2025-08-1
@author  Christian Lara

@section info

At least for Arm Cortex-M0 and Cortex-M4 I have fastest execution with -O2.
The c-standard is C99.

@section LICENSE

MIT License

Copyright (c) 2016-2025 Christian Lara

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


@section History


-add 2025-Agt-1 fucion display grafica signal, funcion de despliegue de grafica


- added EVE_cmd_pclkfreq()
- put back writing of REG_CSSPREAD as it needs to be deactivated for higher frequencies
- added the configuration of the second PLL for the pixel clock in BT817/BT818 to EVE_init() in case the display config
has EVE_PCLK_FREQ defined
- replaced BT81X_ENABLE with "EVE_GEN > 2"
- removed FT81X_ENABLE as FT81x already is the lowest supported chip revision now
- removed the formerly as deprected marked EVE_get_touch_tag()
- changed EVE_color_rgb() to use a 32 bit value like the rest of the color commands
- removed the meta-commands EVE_cmd_point(), EVE_cmd_line() and EVE_cmd_rect()
- split all display-list commands into two functions: EVE_cmd_XXX() and EVE_cmd_XXX_burst()
- switched from using EVE_RAM_CMD + cmdOffset to REG_CMDB_WRITE
- as a side effect from switching to REG_CMDB_WRITE, every coprocessor command is automatically executed now
- renamed EVE_LIB_GetProps() back to EVE_cmd_getprops() since it does not do anything special to justify a special name
- added helper function EVE_memWrite_sram_buffer()
- added EVE_cmd_bitmap_transform() and EVE_cmd_bitmap_transform_burst()
- added zero-pointer protection to commands using block_transfer()
- added EVE_cmd_playvideo()
- changed EVE_cmd_setfont() to a display-list command and added EVE_cmd_setfont_burst()
- changed EVE_cmd_setfont2() to a display-list command and added EVE_cmd_setfont2_burst()
- added EVE_cmd_videoframe()
- restructured: functions are sorted by chip-generation and within their group in alphabetical order
- reimplementedEVE_cmd_getmatrix() again, it needs to read values, not write them
- added EVE_cmd_fontcache() and EVE_cmd_fontcachequery()
- added EVE_cmd_calibratesub()
- added EVE_cmd_animframeram(), EVE_cmd_animframeram_burst(), EVE_cmd_animstartram(), EVE_cmd_animstartram_burst()
- added EVE_cmd_apilevel(), EVE_cmd_apilevel_burst()
- added EVE_cmd_calllist(), EVE_cmd_calllist_burst()
- added EVE_cmd_hsf(), EVE_cmd_hsf_burst()
- added EVE_cmd_linetime()
- added EVE_cmd_newlist(), EVE_cmd_newlist_burst()
- added EVE_cmd_runanim(), EVE_cmd_runanim_burst()
- added a safeguard to EVE_start_cmd_burst() to protect it from overlapping transfers with DMA and segmented lists
- used spi_transmit_32() to shorten this file by around 600 lines with no functional change
- removed the history from before 4.0
- removed a couple of spi_transmit_32() calls from EVE_cmd_getptr() to make it work again
- Bugfix: EVE_cmd_setfont2_burst() was using CMD_SETFONT instead of CMD_SETFONT2
- removed a check for cmd_burst from EVE_cmd_getimage() as it is in the group of commands that are not used for display
lists
- moved EVE_cmd_newlist() to the group of commands that are not used for display lists
- removed EVE_cmd_newlist_burst()
- renamed spi_flash_write() to private_block_write() and made it static
- renamed EVE_write_string() to private_string_write() and made it static
- made EVE_start_command() static
- Bugfix: ESP8266 needs 32 bit alignment for 32 bit pointers,
    changed private_string_write() for burst-mode to read 8-bit values
- Bugfix: somehow messed up private_string_write() for burst-mode
    but only for 8-Bit controllers
- changed EVE_memRead8(), EVE_memRead16() and EVE_memRead32() to use
    spi_transmit_32() for the initial address+zero byte transfer
    This speeds up ESP32/ESP8266 by several us, has no measureable effect
    for ATSAMD51 and is a little slower for AVR.
- Bugfix: not sure why but setting private_block_write() to static broke it, without "static" it works
- Bugfix: EVE_cmd_flashspirx() was using CMD_FLASHREAD
- fixed a warning in EVE_init() when compiling for EVE4
- renamed internal function EVE_begin_cmd() to eve_begin_cmd() and made it static
- changed all the EVE_start_command() calls to eve_begin_cmd() calls following the report on Github from
  Michael Wachs that these are identical - they weren't prior to V5
- removed EVE_start_command()
- Bugfix: EVE_init() was only checking the first two bits of REG_CPURESET and ignored the bit for the audio-engine, not
an issue but not correct either.
- fixed a few clang-tidy warnings
- fixed a few cppcheck warnings
- fixed a few CERT warnings
- converted all TABs to SPACEs
- made EVE_TOUCH_RZTHRESH in EVE_init() optional to a) remove it from EVE_config.h and b) make it configureable
externally
- changed EVE_init() to write 1200U to REG_TOUCH_RZTHRESH if EVE_TOUCH_RZTHRESH is not defined
- changed EVE_init() to return E_OK = 0x00 in case of success and more meaningfull values in case of failure
- changed EVE_busy() to return EVE_IS_BUSY if EVE is busy and E_OK = 0x00 if EVE is not busy - no real change in
functionality
- finally removed EVE_cmd_start() after setting it to deprecatd with the first 5.0 release
- renamed EVE_cmd_execute() to EVE_execute_cmd() to be more consistent, this is is not an EVE command
- changed EVE_init_flash() to return E_OK in case of success and more meaningfull values in case of failure
- added the return-value of EVE_FIFO_HALF_EMPTY to EVE_busy() to indicate there is more than 2048 bytes available
- minor cleanup, less break and else statements
- added the burst code back into all the functions for which there is a _burst version, this allows to use the version
without the traling _burst in the name when exceution speed is not an issue - e.g. with all targets supporting DMA
- removed the 4.0 history
- added the optional parameter EVE_ROTATE as define to EVE_init() to allow for screen rotation during init
    thanks for the idea to AndrejValand on Github!
- added the optional parameter EVE_BACKLIGHT_PWM to EVE_init() to allow setting the backlight during init
- modified EVE_calibrate_manual() to work better with bar type displays
- fixed a large number of MISRA-C issues - mostly more casts for explicit type conversion and more brackets
- changed the varargs versions of cmd_button, cmd_text and cmd_toggle to use an array of uint32_t values to comply with MISRA-C
- basic maintenance: checked for violations of white space and indent rules
- more linter fixes for minor issues like variables shorter than 3 characters
- added EVE_color_a() / EVE_color_a_burst()
- more minor tweaks and fixes to make the static analyzer happy
- changed the burst variant of private_string_write() back to the older and faster version
- refactoring of EVE_init() to single return
- added prototype for EVE_write_display_parameters()
- added EVE_memRead_sram_buffer()
- Bugfix issue #81: neither DISP or the pixel clock are enabled for EVE4 configurations not using EVE_PCLK_FREQ.
    thanks for the report to grados73 on Github!
- added a few support lines for the Gameduino GD3X to EVE_init()
- switched from using CMD_PCLKFREQ to writing to REG_PCLK_FREQ directly
- added define EVE_SET_REG_PCLK_2X to set REG_PCLK_2X to 1 when necessary
- Bugfix: EVE_init() did not set the audio engine to "mute" as intended, but to "silent"
- Bugfix: EVE_busy() returns E_NOT_OK now on coprocessor faults.
    thanks for the report to Z0ld3n on Github!
- Fix: reworked EVE_busy() to return EVE_FAULT_RECOVERED on deteced coprocessor faults,
    removed the flash commands from the fault recovery sequence as these are project specific.
- added EVE_get_and_reset_fault_state() to check if EVE_busy() triggered a fault recovery
- added notes on how to use to EVE_cmd_setfont2() and EVE_cmd_romfont()
- new optional parameter in EVE_init(): EVE_BACKLIGHT_FREQ
- fixed a couple of minor issues from static code analysis
- reworked the burst part of private_string_write() to be less complex
- renamed chipid references to regid as suggested by #93 on github
- Bugfix: broke transfers of buffers larger than 3840 when fixing issues from static code analysis
- changed a number of function parameters from signed to unsigned following the
    updated BT81x series programming guide V2.4
- did another linter pass and fixed some things
- started to improve the embedded documentation
- added more documentation
- added EVE_alpha_func(), EVE_alpha_func_burst()
- dropped CMD_SYNC a tier from EVE3 to EVE2
- added EVE_bitmap_handle() / EVE_bitmap_handle_burst()
- added EVE_bitmap_layout() / EVE_bitmap_layout_burst()
- added EVE_bitmap_layout_h() / EVE_bitmap_layout_h_burst()
- added EVE_b
- added "const" statements for BARR-C:2018 / CERT C compliance
*/
#ifndef _TFTDISPLAY_H_
#define _TFTDISPLAY_H_

#ifdef __cplusplus
extern "C" {
#endif

void display_Grafica_Signal_ejes(void);
void display_letras_de_Grafica_Signal(void);
void display_Graphica_Signal(void);
void display_Puntos(uint8_t status);
void display_btn_Select(uint8_t punto);
void display_Selector_de_Puntos(uint8_t status,uint8_t punto);


   

#ifdef __cplusplus
}
#endif
#endif