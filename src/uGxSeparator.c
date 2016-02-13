/** @file     uGxSeparator.c
    @author   Lyga Maksim
    @version  V0.5
    @date     21.01.2016
 */

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 ***********************************************************************************************/

#include "uGxSeparator.h"
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
void uGx_Separator_Add(uGxSeparator_t *pSeparator, // указатель на графический объект
                       uGx_ObjState_t  State,      // начальный статус состояния объекта
                       uint16_t        Left,       // координата X верхнего левого угла объекта
                       uint16_t        Top,        // координата Y верхнего левого угла объекта
									     uint16_t        Width,      // ширина объекта
                       uint16_t        Height)     // высота объекта
{
  // проверка на корректность указателей
	if (pSeparator != NULL)
	{
		// проверка на корректность размеров объекта
		if ((Width > 0) && (Height > 0))
		{
      // проверка на корректность координат объекта
		  if (((Left + Width - 1) < uGx_Driver_GetMaxX()) && ((Top + Height -1) < uGx_Driver_GetMaxY()))
		  {
		    pSeparator->Header.ID   = 0;
			  
				if (State == uGX_STATE_HIDE) 
        {
				  pSeparator->Header.State = uGX_STATE_HIDE;
					pSeparator->Header.Draw  = uGX_DRAW_NONE;
				}
				else 
        {
				  pSeparator->Header.State = uGX_STATE_ENABLE;
				  pSeparator->Header.Draw  = uGX_DRAW_FULL;
				}
		
		    pSeparator->Header.Focus    = uGX_FOCUS_DISABLE;
		    pSeparator->Header.Left     = Left;
		    pSeparator->Header.Top      = Top;
        pSeparator->Header.Width    = Width;
        pSeparator->Header.Height   = Height;																				
		    pSeparator->Header.Obj_Draw = uGx_Separator_Draw;
														 
				pSeparator->BgndColor       = 0x0000;
				pSeparator->SeparatorColor  = 0x0000;

        uGx_Object_Add(&(pSeparator->Header));
      }
    }
	}
}

// Функция установки цветовой схемы объекта
void uGx_Separator_SetColor(uGxSeparator_t *pSeparator,      // указатель на графический объект
                            uint16_t        BgndColor,       // координата Y верхнего левого угла объекта
                            uint16_t        SeparatorColor)  // высота объекта
{
	// проверка на корректность указателей
	if (pSeparator != NULL)
	{
		pSeparator->BgndColor      = BgndColor;
		pSeparator->SeparatorColor = SeparatorColor;
		pSeparator->Header.Draw    = uGX_DRAW_FULL;
	}
}

// Функция перерисовки объекта
void uGx_Separator_Draw(void *pObject)
{
	// проверка на корректность указателей
	if (pObject != NULL)
	{		
	  uGxSeparator_t *pSeparator = (uGxSeparator_t *)pObject;
		
		uGx_Driver_SetCursor(pSeparator->Header.Left, pSeparator->Header.Top);
		
	  if (pSeparator->Header.State)
		{
			// Если объект видимый
			pSeparator->Header.State = uGX_STATE_ENABLE;
		  uGx_Driver_FillRect(pSeparator->Header.Width, pSeparator->Header.Height, pSeparator->SeparatorColor);			
		}
		else
		{
			// Если объект скрытый
		  uGx_Driver_FillRect(pSeparator->Header.Width, pSeparator->Header.Height, pSeparator->BgndColor);		
		}

		pSeparator->Header.Draw = uGX_DRAW_NONE;
	}	// if проверка на корректность указателей		
}

/* КОНЕЦ ФАЙЛА
 ***********************************************************************************************/
