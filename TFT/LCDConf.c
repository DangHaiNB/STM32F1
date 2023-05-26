/*
----------------------------------------------------------------------
File : LCDConf.c
Purpose : Display controller configuration

Display: HY35A
Controller: SSD1963


---------------------------END-OF-HEADER------------------------------
*/

#include &quot; GUI.h &quot;
#include &quot; GUIDRV_FlexColor.h &quot;
#include &quot; GLCD.h &quot;

/*********************************************************************
*
* Layer configuration
*
**********************************************************************
*/
//
// Physical display size
//
#define XSIZE_PHYS 320
#define YSIZE_PHYS 240

//
// Color conversion
//
#define COLOR_CONVERSION GUICC_565

//
// Display driver
//
#define DISPLAY_DRIVER GUIDRV_FLEXCOLOR

//
// Buffers / VScreens
//
#define NUM_BUFFERS 1
#define NUM_VSCREENS 1

//
// Display orientation
//
#define DISPLAY_ORIENTATION 0
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)
// #define DISPLAY_ORIENTATION (GUI_SWAP_XY | GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_SWAP_XY | GUI_MIRROR_X)

//
// Touch screen
//
#define USE_TOUCH 1
//
// Touch screen calibration
#define TOUCH_X_MIN 0x00E0
#define TOUCH_X_MAX 0x0F40
#define TOUCH_Y_MIN 0x00C0
#define TOUCH_Y_MAX 0x0F60

/*********************************************************************
*
* Configuration checking
*
**********************************************************************
*/
#ifndef XSIZE_PHYS
#error Physical X size of display is not defined!
#endif
#ifndef YSIZE_PHYS
#error Physical Y size of display is not defined!
#endif
#ifndef COLOR_CONVERSION
#error Color conversion not defined!
#endif
#ifndef DISPLAY_DRIVER
#error No display driver defined!
#endif
#ifndef NUM_VSCREENS
#define NUM_VSCREENS 1
#else
#if (NUM_VSCREENS &amp; lt; = 0)
#error At least one screeen needs to be defined!
#endif
#endif
#if (NUM_VSCREENS &amp; gt; 1) &amp; &amp; (NUM_BUFFERS &amp; gt; 1)
#error Virtual screens and multiple buffers are not allowed!
#endif
#ifndef DISPLAY_ORIENTATION
#define DISPLAY_ORIENTATION 0
#endif

#if ((DISPLAY_ORIENTATION &amp; GUI_SWAP_XY) != 0)
#define LANDSCAPE 1
#else
#define LANDSCAPE 0
#endif

#if (LANDSCAPE == 1)
#define WIDTH YSIZE_PHYS /* Screen Width (in pixels) */
#define HEIGHT XSIZE_PHYS /* Screen Hight (in pixels) */
#else
#define WIDTH XSIZE_PHYS /* Screen Width (in pixels) */
#define HEIGHT YSIZE_PHYS /* Screen Hight (in pixels) */
#endif

#if ((DISPLAY_ORIENTATION &amp; GUI_SWAP_XY) != 0)
#if ((DISPLAY_ORIENTATION &amp; GUI_MIRROR_X) != 0)
#define TOUCH_TOP TOUCH_X_MAX
#define TOUCH_BOTTOM TOUCH_X_MIN
#else
#define TOUCH_TOP TOUCH_X_MIN
#define TOUCH_BOTTOM TOUCH_X_MAX
#endif
#if ((DISPLAY_ORIENTATION &amp; GUI_MIRROR_Y) != 0)
#define TOUCH_LEFT TOUCH_Y_MAX
#define TOUCH_RIGHT TOUCH_Y_MIN
#else
#define TOUCH_LEFT TOUCH_Y_MIN
#define TOUCH_RIGHT TOUCH_Y_MAX
#endif
#else
#if ((DISPLAY_ORIENTATION &amp; GUI_MIRROR_X) != 0)
#define TOUCH_LEFT TOUCH_X_MAX
#define TOUCH_RIGHT TOUCH_X_MIN
#else
#define TOUCH_LEFT TOUCH_X_MIN
#define TOUCH_RIGHT TOUCH_X_MAX
#endif
#if ((DISPLAY_ORIENTATION &amp; GUI_MIRROR_Y) != 0)
#define TOUCH_TOP TOUCH_Y_MAX
#define TOUCH_BOTTOM TOUCH_Y_MIN
#else
#define TOUCH_TOP TOUCH_Y_MIN
#define TOUCH_BOTTOM TOUCH_Y_MAX
#endif
#endif

/*********************************************************************
*
* Driver Port functions
*
**********************************************************************
*/
extern void LCD_X_Init(void);
extern void LCD_X_Write00_16(U16 c);
extern void LCD_X_Write01_16(U16 c);
extern U16 LCD_X_Read01_16 (void);
extern void LCD_X_WriteM01_16(U16 * pData, int NumWords);
extern void LCD_X_ReadM01_16 (U16 * pData, int NumWords);

/*********************************************************************
*
* Private code
*
**********************************************************************
*/
//static void delay (int cnt) {
// cnt &amp; lt; &amp; lt; = 15;
// while (cnt--);
//}
//
//static void wr_reg (U16 reg, U16 dat) {
// LCD_X_Write00_16(reg);
// LCD_X_Write01_16(dat);
//}
//
//static U16 rd_reg (U16 reg) {
// return LCD_X_Read01_16();
//}

/*********************************************************************
*
* _InitController
*
* Purpose:
* Initializes the display controller
*/
static void _InitController(void) {
#ifndef WIN32
LCD_X_Init();
LCD_Initializtion();




#endif /* WIN32 */
}

/*********************************************************************
*
* Public code
*
**********************************************************************
*/
/*********************************************************************
*
* LCD_X_Config
*
* Purpose:
* Called during the initialization process in order to set up the
* display driver configuration.
*
*/
void LCD_X_Config(void) {
GUI_DEVICE * pDevice;
GUI_PORT_API PortAPI = {0};
CONFIG_FLEXCOLOR Config = {0};

#if (NUM_BUFFERS &amp; gt; 1)
GUI_MULTIBUF_Config(NUM_BUFFERS);
#endif
//
// Set display driver and color conversion for 1st layer
//
pDevice = GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
//
// Common display driver configuration
//
if (LCD_GetSwapXY()) {
LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
LCD_SetVSizeEx(0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
} else {
LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
}
//
// Configuration of GUIDRV_FLEX_COLOR
//

//
// Set controller and operation mode
//
PortAPI.pfWrite16_A0 = LCD_X_Write00_16;
PortAPI.pfWrite16_A1 = LCD_X_Write01_16;
PortAPI.pfWriteM16_A1 = LCD_X_WriteM01_16;
PortAPI.pfReadM16_A1 = LCD_X_ReadM01_16;
GUIDRV_FlexColor_SetFunc(pDevice, &amp; PortAPI, GUIDRV_FLEXCOLOR_F66709, GUIDRV_FLEXCOLOR_M16C0B16);
//
// Orientation and offset of SEG/COM lines
//
Config.Orientation = DISPLAY_ORIENTATION;
Config.RegEntryMode = 0x08;
GUIDRV_FlexColor_Config(pDevice, &amp; Config);

#if (USE_TOUCH == 1)
//
// Set orientation of touch screen
//
GUI_TOUCH_SetOrientation(DISPLAY_ORIENTATION);
//
// Calibrate touch screen
//
GUI_TOUCH_Calibrate(GUI_COORD_X, 0, WIDTH - 1, TOUCH_LEFT, TOUCH_RIGHT);
GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, HEIGHT - 1, TOUCH_TOP, TOUCH_BOTTOM);
#endif
}

/*********************************************************************
*
* LCD_X_DisplayDriver
*
* Purpose:
* This function is called by the display driver for several purposes.
* To support the according task the routine needs to be adapted to
* the display controller. Please note that the commands marked with
* 'optional' are not cogently required and should only be adapted if
* the display controller supports these features.
*
* Parameter:
* LayerIndex - Index of layer to be configured
* Cmd - Please refer to the details in the switch statement below
* pData - Pointer to a LCD_X_DATA structure
*
* Return Value:
* &amp; lt; -1 - Error
* -1 - Command not handled
* 0 - OK
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
int r;

switch (Cmd) {
//
// Required
//
case LCD_X_INITCONTROLLER: {
//
// Called during the initialization process in order to set up the
// display controller and put it into operation. If the display
// controller is not initialized by any external routine this needs
// to be adapted by the customer...
//
_InitController();
return 0;
}
default:
r = -1;
}
return r;
}

/*************************** End of file ****************************/