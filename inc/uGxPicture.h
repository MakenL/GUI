/** @file     uGxPicture.h
    @author   Lyga Maksim
    @version  V0.5
    @date     21.01.2016
    @brief    
 */

#ifndef __uGX_PICTURE_H
#define __uGX_PICTURE_H

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
  uGx_ObjHeader_t   Header;     // заголовок графического объекта
  uGx_ObjImage_t    Image;      // изображение
	uGx_ObjBorder_t   Border;     // рамка объекта
	uint16_t          Color;      // цвет объекта
} uGxPicture_t;

/* ОПРЕДЕЛЕНИЕ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ
 ***********************************************************************************************/

/* ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
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
                     uint16_t          Height);     // высота объекта

// Функция установки цветовой схемы объекта
void uGx_Picture_SetColor(uGxPicture_t *pPicture,     // указатель на графический объект
                          uint16_t      BgndColor,    // 
									        uint16_t      BorderColor); // 

// Функция установки изображения объекта
void uGx_Picture_SetCaption(uGxPicture_t *pPicture,   // указатель на графический объект
                            uGxImage_t   *pImage);    // указатель на изображение

// Функция перерисовки объекта
void uGx_Picture_Draw(void *pObject);

/* ОПИСАНИЕ INLINE ФУНКЦИЙ
 ***********************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
/*
** КОНЕЦ ФАЙЛА
***********************************************************************************************/
