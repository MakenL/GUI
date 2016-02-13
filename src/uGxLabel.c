/** @file     uGxLabel.c
    @author   Lyga Maksim
    @version  V0.5
    @date     21.01.2016
 */

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 ***********************************************************************************************/

#include "uGxLabel.h"
#include "uGxDriver.h"

/* КОНСТАНТЫ
 ***********************************************************************************************/

/* Глобальные переменные
 ***********************************************************************************************/

/* Локальные переменные
 ***********************************************************************************************/

/* ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
 ***********************************************************************************************/

/* ОПИСАНИЕ ФУНКЦИЙ
 ***********************************************************************************************/

// Функция создания объекта
void uGx_Label_Add(uGxLabel_t       *pLabel,      // указатель на графический объект
                   uGx_ObjState_t    State,       // начальный статус состояния объекта
                   char             *pCaption,    // указатель на надпись объекта
                   uGxFont_t        *pFont,       // указатель на шрифт
									 uGx_Alignment_t   Alignment,   // выравнивание текста по горизонтали
									 uGx_Layout_t      Layout,      // выравнивание текста по вертикали	
									 uGx_BorderStyle_t BorderStyle, // обрамление рамкой объекта
                   uint16_t          Left,        // координата X верхнего левого угла объекта
                   uint16_t          Top,         // координата Y верхнего левого угла объекта
									 uint16_t          Width,       // ширина объекта
                   uint16_t          Height)      // высота объекта
{
  // проверка на корректность указателей
	if ((pLabel != NULL) && (pCaption != NULL) && (pFont != NULL))
	{
		// проверка на корректность размеров объекта
		if ((Width > 2) && (Height > 2))
		{
      // проверка на корректность координат объекта
		  if (((Left + Width - 1) < uGx_Driver_GetMaxX()) && ((Top + Height -1) < uGx_Driver_GetMaxY()))
		  {
		    pLabel->Header.ID   = 0;
			  
				if (State == uGX_STATE_HIDE) 
        {
				  pLabel->Header.State = uGX_STATE_HIDE;
					pLabel->Header.Draw  = uGX_DRAW_NONE;
				}
				else 
        {
				  pLabel->Header.State = uGX_STATE_ENABLE;
				  pLabel->Header.Draw  = uGX_DRAW_FULL;
				}
		
		    pLabel->Header.Focus      = uGX_FOCUS_DISABLE;
		    pLabel->Header.Left       = Left;
		    pLabel->Header.Top        = Top;
        pLabel->Header.Width      = Width;
        pLabel->Header.Height     = Height;																				
		    pLabel->Header.Obj_Draw   = uGx_Label_Draw;
																						
        pLabel->Caption.Value     = pCaption;
		    pLabel->Caption.Font      = pFont;
		    pLabel->Caption.Alignment = Alignment;
				pLabel->Caption.Layout    = Layout;
				pLabel->Caption.Color     = 0xFA60;
				
		    pLabel->Border.Style      = BorderStyle;
				pLabel->Border.Color      = 0xFA60;										 
				pLabel->Color             = 0x0000;

        uGx_Object_Add(&(pLabel->Header));
      }
    }
	}
}

// Функция установки цветовой схемы объекта
void uGx_Label_SetColor(uGxLabel_t *pLabel,       // указатель на графический объект
                        uint16_t    BgndColor,    // 
									      uint16_t    BorderColor,  // 
                        uint16_t    CaptionColor) // 
{
	// проверка на корректность указателей
	if (pLabel != NULL)
	{
		pLabel->Color         = BgndColor;
		pLabel->Border.Color  = BorderColor;
		pLabel->Caption.Color = CaptionColor;
		pLabel->Header.Draw   = uGX_DRAW_FULL;
	}
}

// Функция установки надписи объекта
void uGx_Label_SetCaption(uGxLabel_t *pLabel,   // указатель на графический объект
                          char       *pCaption) // указатель на надпись объекта
{
	// проверка на корректность указателей
	if ((pLabel != NULL) && (pCaption != NULL))
	{
		pLabel->Caption.Value = pCaption;
		pLabel->Header.Draw   = uGX_DRAW_FULL;
	}
}

// Функция перерисовки объекта
void uGx_Label_Draw(void *pObject)
{
	uint16_t CaptionLeft;   // Left position of the Object.
  uint16_t CaptionTop;    // Top position of the Object.	
  uint16_t CaptionWidth;  // ширина текста
  uint16_t CaptionHeight; // высота текста
	
	// проверка на корректность указателей
	if (pObject != NULL)
	{		
	  uGxLabel_t *pLabel = (uGxLabel_t *)pObject;
		
		// Скрытие объекта
		uGx_Driver_SetCursor(pLabel->Header.Left, pLabel->Header.Top);
		uGx_Driver_FillRect(pLabel->Header.Width, pLabel->Header.Height, pLabel->Color);
		
		// Если объект видимый
	  if (pLabel->Header.State)
		{
			pLabel->Header.State = uGX_STATE_ENABLE;
			
			// Отрисовка границ объекта
			if (pLabel->Border.Style)
			{
				uGx_Driver_SetCursor(pLabel->Header.Left, pLabel->Header.Top);
				uGx_Driver_Rect(pLabel->Header.Width, pLabel->Header.Height, pLabel->Border.Color);
			}
			
			// Вычисление ширины надписи		
			CaptionWidth = uGx_GetStrWeight(pLabel->Caption.Value, pLabel->Caption.Font);
      // Если надпись помещается в объект по ширине
		  if ((pLabel->Header.Width - 2) >= CaptionWidth)
			{
			  // Вычисление позиции надписи по горизонтали
		    switch (pLabel->Caption.Alignment)
				{	
          case uGX_ALIGN_LEFT   :
				                         CaptionLeft = pLabel->Header.Left + 1;
						  			             break;
          case uGX_ALIGN_RIGHT  :
					  			               CaptionLeft  = pLabel->Header.Width - CaptionWidth;
				                         CaptionLeft += pLabel->Header.Left;
				                         CaptionLeft --;
								  	             break;
          case uGX_ALIGN_CENTER :
				                         CaptionLeft  = (pLabel->Header.Width - CaptionWidth) >> 1;
				                         CaptionLeft += pLabel->Header.Left;
					                       break;
				}
			  
				// Вычисление высоты надписи	
        CaptionHeight = uGx_GetFontHeight(pLabel->Caption.Font);
        // Если надпись помещается в объект по высоте
			  if ((pLabel->Header.Height - 2) >= CaptionHeight)
			  {
				  // Вычисление позиции надписи по вертикали
		      switch (pLabel->Caption.Layout)
				  {	
            case uGX_LAYOUT_TOP    :
				                            CaptionTop = pLabel->Header.Top + 1;
						  			                break;
            case uGX_LAYOUT_BOTTOM :
					  			                  CaptionTop  = pLabel->Header.Height - CaptionHeight;
				                            CaptionTop += pLabel->Header.Top;
				                            CaptionTop --;
								  	                break;
            case uGX_LAYOUT_CENTER :
				                            CaptionTop  = (pLabel->Header.Height - CaptionHeight) >> 1;
				                            CaptionTop += pLabel->Header.Top;
					                          break;
				  }

					// Отрисовка надписи
					uGx_Driver_SetCursor(CaptionLeft, CaptionTop);
					uGx_Driver_String(pLabel->Caption.Value, pLabel->Caption.Font, pLabel->Color, pLabel->Caption.Color);
			  } // if если надпись помещается в объект по высоте
			} // if если надпись помещается в объект по ширине
		} // if если объект видимый
		
		pLabel->Header.Draw = uGX_DRAW_NONE;
	}	// if проверка на корректность указателей		
}

/* КОНЕЦ ФАЙЛА
 ***********************************************************************************************/
