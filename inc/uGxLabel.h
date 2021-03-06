/** @file     uGxLabel.h
    @author   Lyga Maksim
    @version  V0.5
    @date     21.01.2016
    @brief    
 */

#ifndef __uGX_LABEL_H
#define __uGX_LABEL_H

#ifdef __cplusplus
extern "C" {
#endif

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 ***********************************************************************************************/

#include "uGxObject.h"
	
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
  uGx_ObjHeader_t   Header;   // заголовок графического объекта
  uGx_ObjCaption_t  Caption;  // текст объекта
	uGx_ObjBorder_t   Border;   // рамка объекта
	uint16_t          Color;    // цвет объекта
} uGxLabel_t;

/* ОПРЕДЕЛЕНИЕ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ
 ***********************************************************************************************/

/* ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
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
                   uint16_t          Height);     // высота объекта

// Функция установки цветовой схемы объекта
void uGx_Label_SetColor(uGxLabel_t *pLabel,       // указатель на графический объект
                        uint16_t    BgndColor,    // координата Y верхнего левого угла объекта
									      uint16_t    BorderColor,  // ширина объекта
                        uint16_t    CaptionColor);// высота объекта

// Функция установки надписи объекта
void uGx_Label_SetCaption(uGxLabel_t *pLabel,     // указатель на графический объект
                          char       *pCaption);  // указатель на надпись объекта

// Функция перерисовки объекта
void uGx_Label_Draw(void *pObject);
	
/* ОПИСАНИЕ INLINE ФУНКЦИЙ
 ***********************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
/*
** КОНЕЦ ФАЙЛА
***********************************************************************************************/
