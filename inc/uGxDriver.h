/** @file     uGxDriver.h
    @author   Lyga Maksim
    @version  V1.0
    @date     13.06.2015
    @brief    Драйвер LCD контроллера
		
    @details  Модуль реализующий аппаратнозависимые макросы/функции для управления экраном.
              Названия макросов/функций унифицированы для всех контроллеров.
 */

#ifndef __uGx_DRIVER_H
#define __uGx_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 *************************************************************************************************/

#include "uGxImage.h"
#include "uGxFont.h"

/* КОНСТАНТЫ
 *************************************************************************************************/

/* МАКРОСЫ
 ************************************************************************************************/

/* ОПРЕДЕЛЕНИЕ ТИПОВ
 *************************************************************************************************/	 

/* ОПРЕДЕЛЕНИЕ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ
 *************************************************************************************************/

/* ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
 *************************************************************************************************/	

// Функция инициализации экрана
void uGx_Driver_Init(void);
// Функция возвращает размер экрана по оси X
uint16_t uGx_Driver_GetMaxX(void);
// Функция возвращает размер экрана по оси Y
uint16_t uGx_Driver_GetMaxY(void);

// Функция устанавливает курсор по заданным абсолютным координатам
void uGx_Driver_SetCursor(uint16_t PosX, uint16_t PosY);
// Функция очистки экрана
void uGx_Driver_ClrScr(uint16_t DrawColor);

// Функция рисует линию от текущих координат до указанных конечных
void uGx_Driver_LineTo(uint16_t PosX, uint16_t PosY, uint16_t DrawColor);
// Функция рисует закрашенный прямоугольник
void uGx_Driver_FillRect(uint16_t Weight, uint16_t Height, uint16_t DrawColor);
// Функция рисует прямоугольник
void uGx_Driver_Rect(uint16_t Weight, uint16_t Height, uint16_t DrawColor);
// Функция рисует окружность с центром по текущим координатам
void uGx_Driver_Circle(uint16_t Radius, uint16_t DrawColor);
// Функция рисует круг с центром по текущим координатам
void uGx_Driver_FillCircle(uint16_t Radius, uint16_t DrawColor);
// Функция рисует изображение
void uGx_Driver_Image(const uGxImage_t *pImage, uint16_t BgndColor);
// Функция рисует символ
void uGx_Driver_Char(char vChar, const uGxFont_t *pFont, uint16_t BgndColor, uint16_t DrawColor);
// Функция рисует строку символов
void uGx_Driver_String(const char *pString, const uGxFont_t *pFont, uint16_t BgndColor, uint16_t DrawColor);


#ifdef __cplusplus
}
#endif

#endif
/*
** КОНЕЦ ФАЙЛА
***********************************************************************************************/
