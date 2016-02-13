/** @file     uGxPicture.c
    @author   Lyga Maksim
    @version  V0.5
    @date     21.01.2016
 */

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 ***********************************************************************************************/

#include "uGxPicture.h"
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
void uGx_Picture_Add(uGxPicture_t     *pPicture,    // указатекль на графический объект
                     uGx_ObjState_t    State,       // начальный статус состояния объкта
                     uGxImage_t       *pImage,      // указатель на изображение
									   uGx_Alignment_t   Alignment,   // выравнивание изображения по горизонтали
									   uGx_Layout_t      Layout,      // выравнивание изображения по вертикали	
									   uGx_BorderStyle_t BorderStyle, // обрамление рамкой объекта
                     uint16_t          Left,        // координата X верхнего левого угла объекта
                     uint16_t          Top,         // координата Y верхнего левого угла объекта
									   uint16_t          Width,       // ширина объекта
                     uint16_t          Height)      // высота объекта
{
  // проверка на корректность указателей
	if ((pPicture != NULL) && (pImage != NULL))
	{
		// проверка на корректность размеров объекта
		if ((Width > 2) && (Height > 2))
		{
      // проверка на корректность координат объекта
		  if (((Left + Width - 1) < uGx_Driver_GetMaxX()) && ((Top + Height -1) < uGx_Driver_GetMaxY()))
		  {
		    pPicture->Header.ID   = 0;
			  
				if (State == uGX_STATE_HIDE) 
        {
				  pPicture->Header.State = uGX_STATE_HIDE;
					pPicture->Header.Draw  = uGX_DRAW_NONE;
				}
				else 
        {
				  pPicture->Header.State = uGX_STATE_ENABLE;
				  pPicture->Header.Draw  = uGX_DRAW_FULL;
				}
		
		    pPicture->Header.Focus    = uGX_FOCUS_DISABLE;
		    pPicture->Header.Left     = Left;
		    pPicture->Header.Top      = Top;
        pPicture->Header.Width    = Width;
        pPicture->Header.Height   = Height;																				
		    pPicture->Header.Obj_Draw = uGx_Picture_Draw;																	
		    pPicture->Image.Value     = pImage;
		    pPicture->Image.Alignment = Alignment;
				pPicture->Image.Layout    = Layout;
		    pPicture->Border.Style    = BorderStyle;
				pPicture->Border.Color    = 0xFA60;			
				pPicture->Color           = 0x0000;
				
        uGx_Object_Add(&(pPicture->Header));
      }
    }
	}
}

// Функция установки цветовой схемы объекта
void uGx_Picture_SetColor(uGxPicture_t *pPicture,     // указатель на графический объект
                          uint16_t      BgndColor,    // 
									        uint16_t      BorderColor)  // 
{
	// проверка на корректность указателей
	if (pPicture != NULL)
	{
		pPicture->Color        = BgndColor;
		pPicture->Border.Color = BorderColor;
		pPicture->Header.Draw  = uGX_DRAW_FULL;
	}
}

// Функция установки изображения объекта
void uGx_Picture_SetCaption(uGxPicture_t *pPicture,   // указатель на графический объект
                            uGxImage_t   *pImage)     // указатель на изображение
{
	// проверка на корректность указателей
	if ((pPicture != NULL) && (pImage != NULL))
	{
		pPicture->Image.Value = pImage;
		pPicture->Header.Draw = uGX_DRAW_FULL;
	}
}

// Функция перерисовки объекта
void uGx_Picture_Draw(void *pObject)
{
	uint16_t ImageLeft;   // Left position of the Object.
  uint16_t ImageTop;    // Top position of the Object.	
  uint16_t ImageWidth;  // ширина текста
  uint16_t ImageHeight; // высота текста
	
	// проверка на корректность указателей
	if (pObject != NULL)
	{		
	  uGxPicture_t *pPicture = (uGxPicture_t *)pObject;
		
		// Скрытие объекта
		uGx_Driver_SetCursor(pPicture->Header.Left, pPicture->Header.Top);
		uGx_Driver_FillRect(pPicture->Header.Width, pPicture->Header.Height, pPicture->Color);
		
		// Если объект видимый
	  if (pPicture->Header.State)
		{
			pPicture->Header.State = uGX_STATE_ENABLE;
			
			// Отрисовка границ объекта
			if (pPicture->Border.Style)
			{
				uGx_Driver_SetCursor(pPicture->Header.Left, pPicture->Header.Top);
				uGx_Driver_Rect(pPicture->Header.Width, pPicture->Header.Height, pPicture->Border.Color);
			}
			
			// Вычисление ширины изображения		
			ImageWidth = pPicture->Image.Value->Weight;
      // Если изображение помещается в объект по ширине
		  if ((pPicture->Header.Width - 2) >= ImageWidth)
			{
			  // Вычисление позиции изображения по горизонтали
		    switch (pPicture->Image.Alignment)
				{	
          case uGX_ALIGN_LEFT   :
				                         ImageLeft = pPicture->Header.Left + 1;
						  			             break;
          case uGX_ALIGN_RIGHT  :
					  			               ImageLeft  = pPicture->Header.Width - ImageWidth;
				                         ImageLeft += pPicture->Header.Left;
				                         ImageLeft --;
								  	             break;
          case uGX_ALIGN_CENTER :
				                         ImageLeft  = (pPicture->Header.Width - ImageWidth) >> 1;
				                         ImageLeft += pPicture->Header.Left;
					                       break;
				}
			  
				// Вычисление высоты изображения	
        ImageHeight = pPicture->Image.Value->Height;
        // Если изображение помещается в объект по высоте
			  if ((pPicture->Header.Height - 2) >= ImageHeight)
			  {
				  // Вычисление позиции изображения по вертикали
		      switch (pPicture->Image.Layout)
				  {	
            case uGX_LAYOUT_TOP    :
				                            ImageTop = pPicture->Header.Top + 1;
						  			                break;
            case uGX_LAYOUT_BOTTOM :
					  			                  ImageTop  = pPicture->Header.Height - ImageHeight;
				                            ImageTop += pPicture->Header.Top;
				                            ImageTop --;
								  	                break;
            case uGX_LAYOUT_CENTER :
				                            ImageTop  = (pPicture->Header.Height - ImageHeight) >> 1;
				                            ImageTop += pPicture->Header.Top;
					                          break;
				  }

					// Отрисовка изображения
					uGx_Driver_SetCursor(ImageLeft, ImageTop);
					uGx_Driver_Image(pPicture->Image.Value, pPicture->Color);
			  } // if если изображение помещается в объект по высоте
			} // if если изображение помещается в объект по ширине
		} // if если объект видимый
		
		pPicture->Header.Draw = uGX_DRAW_NONE;
	}	// if проверка на корректность указателей		
}
/* КОНЕЦ ФАЙЛА
 ***********************************************************************************************/
