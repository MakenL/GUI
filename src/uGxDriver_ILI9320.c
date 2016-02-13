/** @file    uGxDriver_ILI9325.c
    @author   Lyga Maksim
    @version  V1.0
    @date     13.06.2015
 */

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 *************************************************************************************************/
 
#include "system_stm32f103x.h"
#include "uGxDriver.h"

/* МАСТЕР КОНФИГУРАЦИИ KEIL
 *************************************************************************************************/
	 
//********************** <<< Use Configuration Wizard in Context Menu >>> **********************

//<h> ILI9325 Configuration
//  <o0> Screen orientation
//              <0=> Portrait
//              <1=> Landscape
//  <o1> Screen height <1-1024>
//  <o2> Screen weight <1-1024>
//</h> End of Terminal Configuration

#define __ILI9325_ORIENTATION   1 ///< Ориентация экрана
#define __ILI9325_MAX_HEIGHT  320 ///< высота экрана в пикселях
#define __ILI9325_MAX_WEIGHT  240 ///< ширина экрана в пикселях

//**************************** <<< end of configuration section >>> ****************************

/* КОНСТАНТЫ
 *************************************************************************************************/

/// GPIOE1->LCD - сброс LCD	
#define ILI9325_RESET_PORT        GPIOE
#define ILI9325_RESET_PIN         1
	
/// GPIOD13->LCD - управление подсветкой LCD
#define ILI9325_BACKLIGHT_PORT    GPIOD
#define ILI9325_BACKLIGHT_PIN     13

// Адреса регистров ILI9320
#define ILI9325_OSC_START         ((uint16_t)0x0000)
#define ILI9325_DRV_OUTPUT1       ((uint16_t)0x0001)
#define ILI9325_DRV_WAVE          ((uint16_t)0x0002)
#define ILI9325_ENTRY_MODE        ((uint16_t)0x0003)
#define ILI9325_RESIZE            ((uint16_t)0x0004)
#define ILI9325_DISPLAY1          ((uint16_t)0x0007)
#define ILI9325_DISPLAY2          ((uint16_t)0x0008)
#define ILI9325_DISPLAY3          ((uint16_t)0x0009)
#define ILI9325_DISPLAY4          ((uint16_t)0x000A)
#define ILI9325_RGB_INTERFACE1    ((uint16_t)0x000C)
#define ILI9325_FRM_MARKER        ((uint16_t)0x000D)
#define ILI9325_RGB_INTERFACE2    ((uint16_t)0x000F)
#define ILI9325_POWER1            ((uint16_t)0x0010)
#define ILI9325_POWER2            ((uint16_t)0x0011)
#define ILI9325_POWER3            ((uint16_t)0x0012)
#define ILI9325_POWER4            ((uint16_t)0x0013)
#define ILI9325_HORZ_ADDRESS      ((uint16_t)0x0020)
#define ILI9325_VERT_ADDRESS      ((uint16_t)0x0021)
#define ILI9325_RW_DATA           ((uint16_t)0x0022)
#define ILI9325_POWER7            ((uint16_t)0x0029)
#define ILI9325_FRM_COLOR         ((uint16_t)0x002B)
#define ILI9325_GAMMA1            ((uint16_t)0x0030)
#define ILI9325_GAMMA2            ((uint16_t)0x0031)
#define ILI9325_GAMMA3            ((uint16_t)0x0032)
#define ILI9325_GAMMA4            ((uint16_t)0x0035)
#define ILI9325_GAMMA5            ((uint16_t)0x0036)
#define ILI9325_GAMMA6            ((uint16_t)0x0037)
#define ILI9325_GAMMA7            ((uint16_t)0x0038)
#define ILI9325_GAMMA8            ((uint16_t)0x0039)
#define ILI9325_GAMMA9            ((uint16_t)0x003C)
#define ILI9325_GAMMA10           ((uint16_t)0x003D)
#define ILI9325_HORZ_START        ((uint16_t)0x0050)
#define ILI9325_HORZ_END          ((uint16_t)0x0051)
#define ILI9325_VERT_START        ((uint16_t)0x0052)
#define ILI9325_VERT_END          ((uint16_t)0x0053)
#define ILI9325_DRV_OUTPUT2       ((uint16_t)0x0060)
#define ILI9325_BASE_IMAGE        ((uint16_t)0x0061)
#define ILI9325_VERT_SCROLL       ((uint16_t)0x006A)
#define ILI9325_PIMG1_POS         ((uint16_t)0x0080)
#define ILI9325_PIMG1_START       ((uint16_t)0x0081)
#define ILI9325_PIMG1_END         ((uint16_t)0x0082)
#define ILI9325_PIMG2_POS         ((uint16_t)0x0083)
#define ILI9325_PIMG2_START       ((uint16_t)0x0084)
#define ILI9325_PIMG2_END         ((uint16_t)0x0085)
#define ILI9325_PANEL_INTERFACE1  ((uint16_t)0x0090)
#define ILI9325_PANEL_INTERFACE2  ((uint16_t)0x0092)
#define ILI9325_PANEL_INTERFACE3  ((uint16_t)0x0093)
#define ILI9325_PANEL_INTERFACE4  ((uint16_t)0x0095)
#define ILI9325_PANEL_INTERFACE5  ((uint16_t)0x0097)
#define ILI9325_PANEL_INTERFACE6  ((uint16_t)0x0098)


// Константы для настройки модуля FSMC
#define FSMC_BANK1_NORSRAM1                    ((uint32_t)0x00000000)
#define FSMC_AccessMode_B                      ((uint32_t)0x10000000)
#define FSMC_DataAddressMux_Disable            ((uint32_t)0x00000000)
#define FSMC_MemoryType_SRAM                   ((uint32_t)0x00000000)
#define FSMC_MemoryDataWidth_16b               ((uint32_t)0x00000010)
#define FSMC_BurstAccessMode_Disable           ((uint32_t)0x00000000)
#define FSMC_WaitSignalPolarity_Low            ((uint32_t)0x00000000)
#define FSMC_WrapMode_Disable                  ((uint32_t)0x00000000)
#define FSMC_WaitSignalActive_BeforeWaitState  ((uint32_t)0x00000000)
#define FSMC_WriteOperation_Enable             ((uint32_t)0x00001000)
#define FSMC_WaitSignal_Disable                ((uint32_t)0x00000000)
#define FSMC_ExtendedMode_Disable              ((uint32_t)0x00000000)
#define FSMC_AsynchronousWait_Disable          ((uint32_t)0x00000000)
#define FSMC_WriteBurst_Disable                ((uint32_t)0x00000000)

#define FSMC_AddressSetupTime                  ((uint32_t)0x00000001)
#define FSMC_AddressHoldTime                   ((uint32_t)0x00000000)
#define FSMC_DataSetupTime                     ((uint32_t)0x00000005)
#define FSMC_BusTurnAroundDuration             ((uint32_t)0x00000000)
#define FSMC_CLKDivision                       ((uint32_t)0x00000000)
#define FSMC_DataLatency                       ((uint32_t)0x00000000)

#define FSMC_ACCESS_REGISTER      (*((volatile uint16_t*) 0x60000000))
#define FSMC_ACCESS_VALUE         (*((volatile uint16_t*) 0x60020000))

/* Глобальные переменные
*************************************************************************************************/	

/* Локальные переменные
*************************************************************************************************/

/* ОПИСАНИЕ INLINE ФУНКЦИЙ
*************************************************************************************************/

__inline static void uGx_Driver_WriteReg(uint16_t Register, uint16_t Value) 
{
  FSMC_ACCESS_REGISTER = Register; 
  FSMC_ACCESS_VALUE    = Value;
}

__inline static uint16_t uGx_Driver_ReadReg(uint16_t Register) 
{
  FSMC_ACCESS_REGISTER = Register; 
  return FSMC_ACCESS_VALUE;
}

__inline static void uGx_Driver_PutPixel(uint16_t Color) 
{
  FSMC_ACCESS_VALUE = Color;
}

__inline static void uGx_Driver_PrepareGRAM(void) 
{
  FSMC_ACCESS_REGISTER = ILI9325_RW_DATA;
}

/* ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
*************************************************************************************************/	

// Функция устанавливает текущие границы окна вывода
void uGx_Driver_SetClipRgn(uint16_t PosX_Begin, uint16_t PosY_Begin, uint16_t PosX_End, uint16_t PosY_End);
// Функция сбрасывает текущие границы окна вывода
void uGx_Driver_RstClipRgn(void);

/* ОПИСАНИЕ ФУНКЦИЙ
*************************************************************************************************/

/** @brief Функция инициализации экрана.

    Функция производит:
    - инициализацию модуля FSMC;
    - сброс контроллера ILI9320;
    - инициализацию контроллера ILI9320;
    - включение подсветки экрана.
 */
void uGx_Driver_Init(void)
{
	// включить тактирование модуля FSMC
	BIT_BAND_REG(RCC->AHBENR, RCC_AHBENR_FSMCEN) = SET;
  // Bank1 NOR/SRAM control register configuration
  FSMC_Bank1->BTCR[FSMC_BANK1_NORSRAM1] = (uint32_t)FSMC_DataAddressMux_Disable |
                                                    FSMC_MemoryType_SRAM |
                                                    FSMC_MemoryDataWidth_16b |
                                                    FSMC_BurstAccessMode_Disable |
                                                    FSMC_AsynchronousWait_Disable |
                                                    FSMC_WaitSignalPolarity_Low |
                                                    FSMC_WrapMode_Disable |
                                                    FSMC_WaitSignalActive_BeforeWaitState |
                                                    FSMC_WriteOperation_Enable |
                                                    FSMC_WaitSignal_Disable |
                                                    FSMC_ExtendedMode_Disable |
                                                    FSMC_WriteBurst_Disable;
	// Bank1 NOR/SRAM timing register configuration
  FSMC_Bank1->BTCR[FSMC_BANK1_NORSRAM1 + 1] =(uint32_t)FSMC_AddressSetupTime |
                                                      (FSMC_AddressHoldTime << 4) |
                                                      (FSMC_DataSetupTime << 8) |
                                                      (FSMC_BusTurnAroundDuration << 16) |
                                                      (FSMC_CLKDivision << 20) |
                                                      (FSMC_DataLatency << 24) |
                                                       FSMC_AccessMode_B;
  // Bank1 NOR/SRAM timing register for write configuration, if extended mode is used
  FSMC_Bank1E->BWTR[FSMC_BANK1_NORSRAM1] = 0x0FFFFFFF;
  FSMC_Bank1->BTCR[FSMC_BANK1_NORSRAM1] |= FSMC_BCR1_MBKEN;	
	
	GPIO_RESET(ILI9325_RESET_PORT,ILI9325_RESET_PIN);
  DelayMs(50);
  GPIO_SET(ILI9325_RESET_PORT,ILI9325_RESET_PIN);
  DelayMs(50);

  uGx_Driver_WriteReg(0x00E3, 0x3008); // Set internal timing
  uGx_Driver_WriteReg(0x00E7, 0x0012); // Set internal timing
  uGx_Driver_WriteReg(0x00EF, 0x1231); // Set internal timing
  
  uGx_Driver_WriteReg(ILI9325_OSC_START, 0x0001);      // Start Oscillation
  uGx_Driver_WriteReg(ILI9325_DRV_OUTPUT1, 0x0100);    // set SS and SM bit
  uGx_Driver_WriteReg(ILI9325_DRV_WAVE, 0x0700);       // set 1 line inversion
#if (__ILI9325_ORIENTATION)
  uGx_Driver_WriteReg(ILI9325_ENTRY_MODE, 0x1038);     // set GRAM write direction and BGR=0,262K colors,1 transfers/pixel.	
#else
  uGx_Driver_WriteReg(ILI9325_ENTRY_MODE, 0x1030);     // set GRAM write direction and BGR=0,262K colors,1 transfers/pixel.
#endif
  uGx_Driver_WriteReg(ILI9325_RESIZE, 0x0000);         // Resize register
  uGx_Driver_WriteReg(ILI9325_DISPLAY2, 0x0202);       // set the back porch and front porch
  uGx_Driver_WriteReg(ILI9325_DISPLAY3, 0x0000);       // set non-display area refresh cycle ISC[3:0]
  uGx_Driver_WriteReg(ILI9325_DISPLAY4, 0x0000);       // FMARK function
  uGx_Driver_WriteReg(ILI9325_RGB_INTERFACE1, 0x0000); // RGB interface setting
  uGx_Driver_WriteReg(ILI9325_FRM_MARKER, 0x0000);     // Frame marker Position
  uGx_Driver_WriteReg(ILI9325_RGB_INTERFACE2, 0x0000); // RGB interface polarity
	// Power On sequence
  uGx_Driver_WriteReg(ILI9325_POWER1, 0x0000);         // SAP, BT[3:0], AP, DSTB, SLP, STB
  uGx_Driver_WriteReg(ILI9325_POWER2, 0x0007);         // DC1[2:0], DC0[2:0], VC[2:0]
  uGx_Driver_WriteReg(ILI9325_POWER3, 0x0000);         // VREG1OUT voltage
  uGx_Driver_WriteReg(ILI9325_POWER4, 0x0000);         // VDV[4:0] for VCOM amplitude
  DelayMs(200);
  uGx_Driver_WriteReg(ILI9325_POWER1, 0x1690);         // SAP, BT[3:0], AP, DSTB, SLP, STB
  uGx_Driver_WriteReg(ILI9325_POWER2, 0x0227);         // R11h=0x0221 at VCI=3.3V, DC1[2:0], DC0[2:0], VC[2:0]
  DelayMs(50);
  uGx_Driver_WriteReg(ILI9325_POWER3, 0x001C);         // External reference voltage= Vci;
  DelayMs(50);

  uGx_Driver_WriteReg(ILI9325_POWER4, 0x1800);         // R13=1200 when R12=009D;VDV[4:0] for VCOM amplitude
  uGx_Driver_WriteReg(ILI9325_POWER7, 0x001C);         // R29=000C when R12=009D;VCM[5:0] for VCOMH
  uGx_Driver_WriteReg(ILI9325_FRM_COLOR, 0x000D);      // Frame Rate = 91Hz
  DelayMs(50);
	
	uGx_Driver_WriteReg(ILI9325_HORZ_ADDRESS, 0x0000);	 // GRAM horizontal Address
	uGx_Driver_WriteReg(ILI9325_VERT_ADDRESS, 0x0000);   // GRAM Vertical Address
	
  // Adjust the Gamma Curve
  uGx_Driver_WriteReg(ILI9325_GAMMA1, 0x0007);
  uGx_Driver_WriteReg(ILI9325_GAMMA2, 0x0302);
  uGx_Driver_WriteReg(ILI9325_GAMMA3, 0x0105);
  uGx_Driver_WriteReg(ILI9325_GAMMA4, 0x0206);
  uGx_Driver_WriteReg(ILI9325_GAMMA5, 0x0808);
  uGx_Driver_WriteReg(ILI9325_GAMMA6, 0x0206);
  uGx_Driver_WriteReg(ILI9325_GAMMA7, 0x0504);
  uGx_Driver_WriteReg(ILI9325_GAMMA8, 0x0007);
  uGx_Driver_WriteReg(ILI9325_GAMMA9, 0x0105);
  uGx_Driver_WriteReg(ILI9325_GAMMA10, 0x0808);

	// Set RAM area
  uGx_Driver_WriteReg(ILI9325_HORZ_START, 0x0000);               // Horizontal GRAM Start Address
  uGx_Driver_WriteReg(ILI9325_HORZ_END, __ILI9325_MAX_WEIGHT-1); // Horizontal GRAM End Address
  uGx_Driver_WriteReg(ILI9325_VERT_START, 0x0000);               // Vertical GRAM Start Address
  uGx_Driver_WriteReg(ILI9325_VERT_END, __ILI9325_MAX_HEIGHT-1); // Vertical GRAM Start Address
#if (__ILI9325_ORIENTATION)
  uGx_Driver_WriteReg(ILI9325_DRV_OUTPUT2, 0x2700);       // Gate Scan Line
#else
  uGx_Driver_WriteReg(ILI9325_DRV_OUTPUT2, 0xA700);       // Gate Scan Line
#endif
  uGx_Driver_WriteReg(ILI9325_BASE_IMAGE, 0x0001);        // NDL,VLE, REV
  uGx_Driver_WriteReg(ILI9325_VERT_SCROLL, 0x0000);       // set scrolling line

  // Partial Display Control
  uGx_Driver_WriteReg(ILI9325_PIMG1_POS, 0x0000);
  uGx_Driver_WriteReg(ILI9325_PIMG1_START, 0x0000);
  uGx_Driver_WriteReg(ILI9325_PIMG1_END, 0x0000);
  uGx_Driver_WriteReg(ILI9325_PIMG2_POS, 0x0000);
  uGx_Driver_WriteReg(ILI9325_PIMG2_START, 0x0000);
  uGx_Driver_WriteReg(ILI9325_PIMG2_END, 0x0000);
  // Panel Control
  uGx_Driver_WriteReg(ILI9325_PANEL_INTERFACE1, 0x0010);
  uGx_Driver_WriteReg(ILI9325_PANEL_INTERFACE2, 0x0000);
  uGx_Driver_WriteReg(ILI9325_PANEL_INTERFACE3, 0x0003);
  uGx_Driver_WriteReg(ILI9325_PANEL_INTERFACE4, 0x0110);
  uGx_Driver_WriteReg(ILI9325_PANEL_INTERFACE5, 0x0000);
  uGx_Driver_WriteReg(ILI9325_PANEL_INTERFACE6, 0x0000);

	// Команда включить дисплей
  uGx_Driver_WriteReg(ILI9325_DISPLAY1, 0x0133);
  // Включить подсветку
  GPIO_RESET(ILI9325_BACKLIGHT_PORT,ILI9325_BACKLIGHT_PIN);
	// Подготовка к записи данных в GRAM
  uGx_Driver_PrepareGRAM();
}

/** @brief Функция возвращает размер экрана по горизонтали.
    @return размер экрана по горизонтали (ось X)
 */
uint16_t uGx_Driver_GetMaxX(void)
{
#if (__ILI9325_ORIENTATION)
  return __ILI9325_MAX_HEIGHT;
#else
  return __ILI9325_MAX_WEIGHT;
#endif
}

/** @brief Функция возвращает размер экрана по вертикали.
    @return размер экрана по вертикали (ось Y)
 */
uint16_t uGx_Driver_GetMaxY(void)
{
#if (__ILI9325_ORIENTATION)
  return __ILI9325_MAX_WEIGHT;
#else
  return __ILI9325_MAX_HEIGHT;
#endif
	
}

/** @brief Функция устанавливает курсор по заданным абсолютным координатам
    @param[in] PosX координата X экрана
    @param[in] PosY координата Y экрана
 */
void uGx_Driver_SetCursor(uint16_t PosX, uint16_t PosY)
{
#if (__ILI9325_ORIENTATION)
	uGx_Driver_WriteReg(ILI9325_HORZ_ADDRESS, PosY); // установка координаты Y
	uGx_Driver_WriteReg(ILI9325_VERT_ADDRESS, PosX); // установка координаты X
#else
	uGx_Driver_WriteReg(ILI9325_HORZ_ADDRESS, PosX); // установка координаты X
	uGx_Driver_WriteReg(ILI9325_VERT_ADDRESS, PosY); // установка координаты Y
#endif
  // Подготовка к записи данных в GRAM
  uGx_Driver_PrepareGRAM();
}

/** @brief Функция очистки экрана.

    Функция производит заливку экрана цветом DrawColor и устанавливает курсор в позицию 0,0
    @param[in] DrawColor цвет рисования
 */
void uGx_Driver_ClrScr(uint16_t DrawColor)
{
	uint32_t PixelCount = 0;

	uGx_Driver_RstClipRgn();
	uGx_Driver_SetCursor(0, 0);
	
  for(PixelCount = 0; PixelCount < (__ILI9325_MAX_HEIGHT * __ILI9325_MAX_WEIGHT); PixelCount++)
  {
		uGx_Driver_PutPixel(DrawColor);
  }
	
	uGx_Driver_SetCursor(0, 0);
}

/** @brief Функция рисует линию от текущих координат до указанных конечных.
    @param[in] PosX конечная координата X экрана
    @param[in] PosY конечная координата Y экрана
    @param[in] DrawColor цвет рисования
 */
void uGx_Driver_LineTo(uint16_t PosX, uint16_t PosY, uint16_t DrawColor)
{
	int16_t sDeltaX, sDeltaY, sSignX, sSignY, sErr, sErr2;
  uint16_t LineLength = 0;
	
#if (__ILI9325_ORIENTATION)
	uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты Y
	uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты X
#else
	uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты X
	uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты Y
#endif
	
	// линия горизонтальная
  if (CurrPosX == PosX)
  {
	  if (PosY >= CurrPosY)
    {
		  // вычисление ширины горизонтальной линии
      LineLength = PosY - CurrPosY + 1;
      // отрисовка горизонтальной линии
      uGx_Driver_FillRect(1, LineLength, DrawColor);
			uGx_Driver_SetCursor(PosX, PosY);
    }
    else
    {
		  // вычисление ширины горизонтальной линии
			LineLength = CurrPosY - PosY + 1;
      uGx_Driver_SetCursor(PosX, PosY);
      // отрисовка горизонтальной линии
      uGx_Driver_FillRect(1, LineLength, DrawColor);
    }
  }
  // линия вертикальная
  else if (CurrPosY == PosY)
  {
	  if (PosX >= CurrPosX)
    {
		  // вычисление высоты вертикальной линии
      LineLength = PosX - CurrPosX + 1;
      // отрисовка вертикальной линии
      uGx_Driver_FillRect(LineLength, 1, DrawColor);
			uGx_Driver_SetCursor(PosX, PosY);
    }
    else
    {
		  // вычисление высоты вертикальной линии
      LineLength = CurrPosX - PosX + 1;
      uGx_Driver_SetCursor(PosX, PosY);
      // отрисовка вертикальной линии
      uGx_Driver_FillRect(LineLength, 1, DrawColor);
    }
  }
  // линия наклонная
  else
  {
	  if (PosX > CurrPosX)
    {
		  sSignX  =  1;
      sDeltaX = PosX - CurrPosX;
    }
    else
    {
		  sSignX  = -1;
      sDeltaX = CurrPosX - PosX;
    }
		
    if (PosY > CurrPosY)
    {
		  sSignY  =  1;
      sDeltaY = PosY - CurrPosY;
    }
    else
    {
		  sSignY  = -1;
      sDeltaY = CurrPosY - PosY;
    }
    sErr = sDeltaX - sDeltaY;

    while((CurrPosX != PosX) || (CurrPosY != PosY))
    {
		  uGx_Driver_SetCursor(CurrPosX, CurrPosY);
      uGx_Driver_PutPixel(DrawColor);
      sErr2 = sErr * 2;
      if(sErr2 > -sDeltaY)
      {
		    sErr -= sDeltaY;
        CurrPosX += sSignX;
      }
      if(sErr2 <  sDeltaX)
      {
			  sErr += sDeltaX;
        CurrPosY += sSignY;
      }
    } // while
	} // if
}

/** @brief Функция рисует закрашенный прямоугольник.
    @param[in] Weight ширина прямоугольника
    @param[in] Height высота прямоугольника
    @param[in] DrawColor цвет рисования
 */
void uGx_Driver_FillRect(uint16_t Weight, uint16_t Height, uint16_t DrawColor)
{
	uint32_t PixelCount = 0;

#if (__ILI9325_ORIENTATION)
	uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты Y
	uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты X
#else
	uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты X
	uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты Y
#endif
	
	// Устанавка текущих границ окна вывода
  uGx_Driver_SetClipRgn(CurrPosX, CurrPosY, 
	                      CurrPosX + Weight - 1, 
	                      CurrPosY + Height - 1);
  // Прорисовка прямоугольника
  for(PixelCount = 0; PixelCount < Weight * Height; PixelCount++)
  {
		uGx_Driver_PutPixel(DrawColor);
  }
  // Сброс текущих границ окна вывода
  uGx_Driver_RstClipRgn();
}

/** @brief Функция рисует прямоугольник.
    @param[in] Weight ширина прямоугольника
    @param[in] Height высота прямоугольника
    @param[in] DrawColor цвет рисования
 */
void uGx_Driver_Rect(uint16_t Weight, uint16_t Height, uint16_t DrawColor)
{
	uint16_t PosBegX    = 0;
	uint16_t PosEndX    = 0;
	uint16_t PosBegY    = 0;
  uint16_t PosEndY    = 0;
	
#if (__ILI9325_ORIENTATION)
	uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты Y
	uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты X
#else
	uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты X
	uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты Y
#endif

	/* Вычисление конечных координат */
  PosEndX = CurrPosX + Weight - 1;
  PosEndY = CurrPosY + Height - 1;
	
	// Запоминаем начальные координаты
	PosBegX = CurrPosX;
	PosBegY = CurrPosY;
		
	uGx_Driver_LineTo(PosEndX, PosBegY, DrawColor);
	uGx_Driver_LineTo(PosEndX, PosEndY, DrawColor);
  uGx_Driver_LineTo(PosBegX, PosEndY, DrawColor);
	uGx_Driver_LineTo(PosBegX, PosBegY, DrawColor);	
}

/** @brief Функция рисует окружность с центром по текущим координатам.
    @param[in] Radius радиус окружости
    @param[in] DrawColor цвет рисования
 */
void uGx_Driver_Circle(uint16_t Radius, uint16_t DrawColor)
{
	int16_t  sF      = 1 - Radius;
  int16_t  sdFx    = 1;
  int16_t  sdFy    = -2 * Radius;
  int16_t  sX      = 0;
  int16_t  sY      = Radius;

#if (__ILI9325_ORIENTATION)
	uint16_t CenterY = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты Y
	uint16_t CenterX = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты X
#else
	uint16_t CenterX = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты X
	uint16_t CenterY = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты Y
#endif	
	
	uGx_Driver_SetCursor(CenterX, CenterY + Radius);
  uGx_Driver_PutPixel (DrawColor);
  uGx_Driver_SetCursor(CenterX, CenterY - Radius);
  uGx_Driver_PutPixel (DrawColor);
  uGx_Driver_SetCursor(CenterX + Radius, CenterY);
  uGx_Driver_PutPixel (DrawColor);
  uGx_Driver_SetCursor(CenterX - Radius, CenterY);
  uGx_Driver_PutPixel (DrawColor);

  while(sX < sY)
  {
	  if(sF >= 0)
    {
		  sY--;
      sdFy += 2;
      sF += sdFy;
    }
    sX++;
    sdFx += 2;
    sF += sdFx;

    uGx_Driver_SetCursor(CenterX + sX, CenterY + sY);
    uGx_Driver_PutPixel (DrawColor);
    uGx_Driver_SetCursor(CenterX - sX, CenterY + sY);
    uGx_Driver_PutPixel (DrawColor);
    uGx_Driver_SetCursor(CenterX + sX, CenterY - sY);
    uGx_Driver_PutPixel (DrawColor);
    uGx_Driver_SetCursor(CenterX - sX, CenterY - sY);
    uGx_Driver_PutPixel (DrawColor);
    uGx_Driver_SetCursor(CenterX + sY, CenterY + sX);
    uGx_Driver_PutPixel (DrawColor);
    uGx_Driver_SetCursor(CenterX - sY, CenterY + sX);
    uGx_Driver_PutPixel (DrawColor);
    uGx_Driver_SetCursor(CenterX + sY, CenterY - sX);
    uGx_Driver_PutPixel (DrawColor);
    uGx_Driver_SetCursor(CenterX - sY, CenterY - sX);
    uGx_Driver_PutPixel (DrawColor);
  } // while
	uGx_Driver_SetCursor(CenterX, CenterY);
}

/** @brief Функция рисует круг с центром по текущим координатам.
    @param[in] Radius радиус круга
    @param[in] DrawColor цвет рисования
 */
void uGx_Driver_FillCircle(uint16_t Radius, uint16_t DrawColor)
{
  int16_t  sF   = 1 - Radius;
  int16_t  sdFx = 1;
  int16_t  sdFy = -2 * Radius;
  int16_t  sX   = 0;
  int16_t  sY   = Radius;

#if (__ILI9325_ORIENTATION)
	uint16_t CenterY = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты Y
	uint16_t CenterX = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты X
#else
	uint16_t CenterX = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты X
	uint16_t CenterY = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты Y
#endif	

	uGx_Driver_SetCursor(CenterX + sX, CenterY - sY);
	uGx_Driver_FillRect(1, sY * 2, DrawColor);

  while(sX < sY)
  {
	  if(sF >= 0)
    {
		  sY--;
      sdFy += 2;
      sF += sdFy;
    }
    sX++;
    sdFx += 2;
    sF += sdFx;

		uGx_Driver_SetCursor(CenterX + sX, CenterY - sY);
		uGx_Driver_FillRect(1, sY * 2, DrawColor);
		uGx_Driver_SetCursor(CenterX - sX, CenterY - sY);
		uGx_Driver_FillRect(1, sY * 2, DrawColor);
		uGx_Driver_SetCursor(CenterX + sY, CenterY - sX); 
		uGx_Driver_FillRect(1, sX * 2, DrawColor);
		uGx_Driver_SetCursor(CenterX - sY, CenterY - sX);
		uGx_Driver_FillRect(1, sX * 2, DrawColor);
	} // while
	uGx_Driver_SetCursor(CenterX, CenterY);
}

/** @brief Функция рисует изображение.
    @param[in] pImage указатель на изображение типа uGxImage_t
    @param[in] BgndColor цвет фона
 */
void uGx_Driver_Image(const uGxImage_t *pImage, uint16_t BgndColor)
{
  uint32_t PixelCount = 0;

	if (pImage != NULL)
	{
	  if ((pImage->Weight > 0) && (pImage->Height > 0 ))
	  {
			#if (__ILI9325_ORIENTATION)
	      uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты Y
	      uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты X
      #else
	      uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты X
	      uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты Y
      #endif
			
			// Устанавка текущих границ окна вывода
      uGx_Driver_SetClipRgn(CurrPosX, CurrPosY, 
	                          CurrPosX + pImage->Weight - 1, 
	                          CurrPosY + pImage->Height - 1);
      // Прорисовка изображения
      for(PixelCount = 0; PixelCount < pImage->Weight * pImage->Height; PixelCount++)
      {
		    if (pImage->Bitmap[PixelCount] == (pImage->Transparent)) {uGx_Driver_PutPixel(BgndColor);}
		                                                        else {uGx_Driver_PutPixel(pImage->Bitmap[PixelCount]);}
      }
      // Сброс текущих границ окна вывода
      uGx_Driver_RstClipRgn();
    } 
	}
}

/** @brief Функция рисует символ.
    @param[in] vChar символ
    @param[in] pFont указатель на шрифт
    @param[in] BgndColor цвет фона
    @param[in] DrawColor цвет рисования
 */
void uGx_Driver_Char(char vChar, const uGxFont_t *pFont, uint16_t BgndColor, uint16_t DrawColor)
{
  uint32_t i             = 0;
	uint8_t  j             = 0;
	uint16_t CharWeight    = 0; // Ширина символа
	uint16_t CharHeight    = 0; // Высота символа
	uint16_t begIndBitmaps = 0; // Индекс начала данных символа
	uint16_t endIndBitmaps = 0; // Индекс конца данных символа
	uint8_t  shiftByte     = 8;   
	uint8_t  acumByte      = 8;
	 
	// Проверка на корректность указателя
	if (pFont != NULL)
	{	
	  CharHeight = pFont->Height;
		
		if ((vChar >= pFont->StartChar) && (vChar <= pFont->EndChar))
		{
		  CharWeight = pFont->Descriptor[(vChar - pFont->StartChar) * 2];
		}
	  
	  // Проверка на присутствие символа в шрифте
	  if (CharWeight > 0) 
	  {
			// Вычисление начала данных символа
	    begIndBitmaps = pFont->Descriptor[(vChar - pFont->StartChar) * 2 + 1];
			// Вычисление конца данных символа
	    if (CharWeight < 9)  
      {
			  endIndBitmaps = begIndBitmaps + CharHeight;
			}
		  else if (CharWeight < 17) 
      {
			  endIndBitmaps = begIndBitmaps + CharHeight * 2;
			}
	    else if (CharWeight < 25) 
      {
			  endIndBitmaps = begIndBitmaps+CharHeight * 3;
			}
	    else 
      {
			  endIndBitmaps = begIndBitmaps + CharHeight * 4;
			}
			
			#if (__ILI9325_ORIENTATION)
	      uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты Y
	      uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты X
      #else
	      uint16_t CurrPosX = uGx_Driver_ReadReg(ILI9325_HORZ_ADDRESS); // считывание координаты X
	      uint16_t CurrPosY = uGx_Driver_ReadReg(ILI9325_VERT_ADDRESS); // считывание координаты Y
      #endif
			
 	    uGx_Driver_SetClipRgn(CurrPosX, 
			                      CurrPosY, 
			                      CurrPosX + CharWeight - 1,
			                      CurrPosY + CharHeight - 1);

 	    for(i = begIndBitmaps; i < endIndBitmaps; i++)
	    {
        if (acumByte >= CharWeight)  
		    {
          shiftByte = 8 - (acumByte - CharWeight); 
          acumByte = 8;
        }
		    else
		    {
          shiftByte = 8; 
          acumByte += shiftByte;
        }	
        j = 0;			 
		    while (j < shiftByte)
        {			
		      if (pFont->Bitmap[i] & (0x80 >> j)) 
					{
					  uGx_Driver_PutPixel(DrawColor);
					}
		      else 
          {
					  uGx_Driver_PutPixel(BgndColor);
					}
				  j++;	 
        } // while
	    } // for
			uGx_Driver_RstClipRgn();
			uGx_Driver_SetCursor(CurrPosX + CharWeight, CurrPosY);			
    } // if Проверка на присутствие символа в шрифте
	}	// if Проверка на корректность указателя
}

/** @brief Функция рисует строку символов.
    @param[in] pString указатель на строку символов
    @param[in] pFont указатель на шрифт
    @param[in] BgndColor цвет фона
    @param[in] DrawColor цвет рисования
 */
void uGx_Driver_String(const char *pString, const uGxFont_t *pFont, uint16_t BgndColor, uint16_t DrawColor)
{
	if ((pString != NULL) && (pFont !=NULL))
	{
	  while (*pString)
    {
      uGx_Driver_Char(*pString, pFont, BgndColor, DrawColor);
  		pString++;
    }
	}
}

/** @brief Функция устанавливает текущие границы окна вывода.
    @param[in] begPosX - координата X верхнего левого угла окна
    @param[in] begPosY - координата Y верхнего левого угла окна
    @param[in] endPosX - координата X нижнего правого угла окна
    @param[in] endPosY - координата Y нижнего правого угла окна
 */
void uGx_Driver_SetClipRgn(uint16_t PosX_Begin, uint16_t PosY_Begin, uint16_t PosX_End, uint16_t PosY_End)
{
#if (__ILI9325_ORIENTATION)
  uGx_Driver_WriteReg(ILI9325_HORZ_START, PosY_Begin);   // установка начальной координаты X
	uGx_Driver_WriteReg(ILI9325_HORZ_END, PosY_End);	     // установка конечной координаты X
	uGx_Driver_WriteReg(ILI9325_VERT_START, PosX_Begin);	 // установка начальной координаты Y
	uGx_Driver_WriteReg(ILI9325_VERT_END, PosX_End);       // установка конечной координаты Y
	uGx_Driver_WriteReg(ILI9325_HORZ_ADDRESS, PosY_Begin); // установка координаты X курсора
	uGx_Driver_WriteReg(ILI9325_VERT_ADDRESS, PosX_Begin); // установка координаты Y курсора
#else
  uGx_Driver_WriteReg(ILI9325_HORZ_START, PosX_Begin);   // установка начальной координаты X
	uGx_Driver_WriteReg(ILI9325_HORZ_END, PosX_End);	     // установка конечной координаты X
	uGx_Driver_WriteReg(ILI9325_VERT_START, PosY_Begin);	 // установка начальной координаты Y
	uGx_Driver_WriteReg(ILI9325_VERT_END, PosY_End);       // установка конечной координаты Y
	uGx_Driver_WriteReg(ILI9325_HORZ_ADDRESS, PosX_Begin); // установка координаты X курсора
	uGx_Driver_WriteReg(ILI9325_VERT_ADDRESS, PosY_Begin); // установка координаты Y курсора
#endif	
	// подготовка к записи данных в GRAM
  uGx_Driver_PrepareGRAM();
}

/**\brief Функция сбрасывает текущие границы окна вывода.
 */
void uGx_Driver_RstClipRgn(void)
{
	uGx_Driver_WriteReg(ILI9325_HORZ_START, 0x0000);                 // установка начальной координаты Y
	uGx_Driver_WriteReg(ILI9325_HORZ_END, __ILI9325_MAX_WEIGHT - 1); // установка конечной координаты Y
	uGx_Driver_WriteReg(ILI9325_VERT_START, 0x0000);		             // установка начальной координаты X	
	uGx_Driver_WriteReg(ILI9325_VERT_END, __ILI9325_MAX_HEIGHT - 1); // установка конечной координаты X
	// подготовка к записи данных в GRAM
	uGx_Driver_PrepareGRAM();
}

/* КОНЕЦ ФАЙЛА
 *************************************************************************************************/
