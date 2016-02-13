/** @file     uGxSwitchButton.h
    @author   Lyga Maksim
    @version  V0.5
    @date     21.01.2016
    @brief    
 */

#ifndef __uGX_SWITCH_BUTTON_H
#define __uGX_SWITCH_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 ***********************************************************************************************/

#include "uGxObject.h"
#include "uGxFont.h"
	
/* МАСТЕР КОНФИГУРАЦИИ
 ***********************************************************************************************/

/* КОНСТАНТЫ
 ***********************************************************************************************/

/* МАКРОСЫ
 ***********************************************************************************************/

/* ОПРЕДЕЛЕНИЕ ТИПОВ
 ***********************************************************************************************/

// Структура графического объекта
typedef struct
{
  uGx_ObjHeader_t   Header;         // заголовок графического объекта
  uint8_t           Value;
  char             *CaptionValue1;  // указатель на текст
  char             *CaptionValue2;  // указатель на текст
  uGxFont_t        *CaptionFont;    // указатель на шрифт
  uGx_Alignment_t   Alignment;      // выравнивание текста по горизонтали
  uGx_Layout_t      Layout;         // выравнивание текста по вертикали
  uint16_t          BgndColor;
  uint16_t          BorderColorDisabled;
  uint16_t          BorderColorEnabled;
  uint16_t          BorderColorFocused;
  uint16_t          ButtonColorDisabled;
  uint16_t          ButtonColorEnabled;
  uint16_t          ButtonColorAction;
  uint16_t          CaptionColorDisabled;
  uint16_t          CaptionColorValue1;
  uint16_t          CaptionColorValue2;
} uGxSwitchButton_t;

/* ОПРЕДЕЛЕНИЕ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ
 ***********************************************************************************************/

/* ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
 ***********************************************************************************************/

// Функция создания объекта
void uGx_SwitchButton_Add(uGxSwitchButton_t *pSwitchButton, // указатель на графический объект
                          uGx_ObjState_t     State,         // начальный статус состояния объекта
                          uint8_t            Value,         // начальное состояние объекта
                          char              *CaptionValue1, // указатель на надпись объекта
                          char              *CaptionValue2, // указатель на надпись объекта
                          uGxFont_t         *pFont,         // указатель на шрифт
									        uGx_Alignment_t    Alignment,     // выравнивание текста по горизонтали
									        uGx_Layout_t       Layout,        // выравнивание текста по вертикали	
                          uint16_t           Left,          // координата X верхнего левого угла объекта
                          uint16_t           Top,           // координата Y верхнего левого угла объекта
									        uint16_t           Width,         // ширина объекта
                          uint16_t           Height);       // высота объекта

// Функция установки цветовой схемы объекта
void uGx_SwitchButton_SetColor(uGxSwitchButton_t *pSwitchButton,     // указатель на графический объект
                               uint16_t          BgndColor,
                               uint16_t          BorderColorDisabled,
                               uint16_t          BorderColorEnabled,
                               uint16_t          BorderColorFocused,
                               uint16_t          ButtonColorDisabled,
                               uint16_t          ButtonColorEnabled,
                               uint16_t          ButtonColorAction,
                               uint16_t          CaptionColorDisabled,
                               uint16_t          CaptionColorValue1,
                               uint16_t          CaptionColorValue2);

// Функция установки надписи объекта
void uGx_SwitchButton_SetCaption(uGxSwitchButton_t *pLabel,          // указатель на графический объект
                                 char              *pCaptionValue1,
																 char              *pCaptionValue2); // указатель на надпись объекта

// Функция перерисовки объекта
void uGx_SwitchButton_Draw(void *pObject);
	
/* ОПИСАНИЕ INLINE ФУНКЦИЙ
 ***********************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
/*
** КОНЕЦ ФАЙЛА
***********************************************************************************************/
