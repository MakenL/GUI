/** @file     uGxObject.h
    @author   Lyga Maksim
    @version  V0.5
    @date     21.01.2016
    @brief    
 */

#ifndef __uGX_OBJECT_H
#define __uGX_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 ***********************************************************************************************/

#include "uGxMessage.h"
#include "uGxFont.h"
#include "uGxImage.h"
	
	
/* МАСТЕР КОНФИГУРАЦИИ
 ***********************************************************************************************/

/* КОНСТАНТЫ
 ***********************************************************************************************/

/* МАКРОСЫ
 ***********************************************************************************************/

/* ОПРЕДЕЛЕНИЕ ТИПОВ
 ***********************************************************************************************/

// Статусы состояния графических объектов
typedef enum
{
  uGX_TYPE_WIDGET    = 0, ///< графический объект скрыт
  uGX_TYPE_CONTEINER = 1, ///< графический объект скрыт	
  uGX_TYPE_FORM      = 2  ///< графический объект выключен	
} uGx_ObjType_t;

typedef enum
{
  uGX_OBJ_INVISIBLE = 0, ///< графический объект скрыт
  uGX_OBJ_VISIBLE   = 1, ///< графический объект скрыт	
} uGx_ObjVisible_t;

// Статусы состояния графических объектов
typedef enum
{
  uGX_STATE_DISABLE = 0, ///< графический объект выключен
  uGX_STATE_ENABLE  = 1, ///< графический объект включен
  uGX_STATE_FOCUS   = 2, ///< графический объект находится в фокусе
  uGX_STATE_ACTION  = 3  ///< графический объект исполняется
} uGx_ObjState_t;

// Статусы отображения графических объектов
typedef enum
{
  uGX_DRAW_NONE     = 0, ///< графический объект не требует перересовки
  uGX_DRAW_UPDATE   = 1, ///< графический объект должен быть обновлен	
  uGX_DRAW_FULL     = 2  ///< графический объект должен быть перересован полностью
} uGx_ObjDraw_t;	

// Возможность переместить на графический объект фокус
typedef enum
{
  uGX_FOCUS_DISABLE  = 0, ///< на графический объект не возможно переместить фокус  
  uGX_FOCUS_ENABLE   = 1  ///< на графический объект воможно переместить фокус
} uGx_ObjFocus_t;

typedef enum
{
  uGX_ALIGN_LEFT   = 0,
  uGX_ALIGN_RIGHT  = 1,
	uGX_ALIGN_CENTER = 2
} uGx_Alignment_t;

typedef enum
{
  uGX_LAYOUT_TOP    = 0,
  uGX_LAYOUT_BOTTOM = 1,
	uGX_LAYOUT_CENTER = 2
} uGx_Layout_t;

typedef enum
{
  uGX_BORDER_NONE   = 0, ///< на графический объект не возможно переместить фокус  
  uGX_BORDER_SINGLE = 1  ///< на графический объект воможно переместить фокус
} uGx_BorderStyle_t;


// Структура графического объекта
typedef struct
{
	uGx_BorderStyle_t Style;   // обрамление рамкой объекта
	uint16_t          Color;
} uGx_ObjBorder_t;

// Структура графического объекта
typedef struct
{
  char             *Value;      // указатель на текст
  uGxFont_t        *Font;       // указатель на шрифт
	uGx_Alignment_t   Alignment;  // выравнивание текста по горизонтали
	uGx_Layout_t      Layout;     // выравнивание текста по вертикали	
	uint16_t          Color;
} uGx_ObjCaption_t;

// Структура графического объекта
typedef struct
{
  uGxImage_t       *Value;              // указатель на изображение
	uGx_Alignment_t   Alignment;          // выравнивание изображения по горизонтали
	uGx_Layout_t      Layout;             // выравнивание изображения по вертикали	
} uGx_ObjImage_t;

typedef struct
{
  uint16_t  ID;             // Unique id assigned for referencing.
	void            *Owner;
	void            *Next;       // A pointer to the next object.
	void            *Prev;       // A pointer to the prev object.
	
	uGx_ObjType_t    Type;
	uGx_ObjVisible_t Visible;
	uGx_ObjState_t   State;
	uGx_ObjDraw_t    Draw;
	uGx_ObjFocus_t   Focus;
	
	uint16_t         Left;           // Left position of the Object.
  uint16_t         Top;            // Top position of the Object.
  uint16_t         Width;          // Right position of the Object.
  uint16_t         Height;         // Bottom position of the Object.
	
	void  (*DrawObj)(void *pObject);    // function pointer to the object's draw function.
} uGx_ObjHeader_t;


// Структура окна
typedef struct 
{
	uGx_ObjHeader_t   Property; 
  uGx_ObjHeader_t  *Collection;    // указатель на первый графический объект
} uGx_ObjСontainer_t;

// Структура окна
typedef struct 
{
	uGx_ObjСontainer_t  Form; 
	uGx_ObjHeader_t    *FocusObj;
	uGx_ObjBorder_t     Border;
	uint16_t            Color;
} uGx_ObjForm_t;

/* ОПРЕДЕЛЕНИЕ ГЛОБАЛЬНЫХ ПЕРЕМЕННЫХ
 ***********************************************************************************************/

/* ПРОТОТИПЫ ГЛОБАЛЬНЫХ ФУНКЦИЙ
 ***********************************************************************************************/

// Функция инициализации графической библиотеки
void uGx_Init(void);
// Функция перерисовки графических объектов
void uGx_Draw(void);

// Функция добавления графического объекта
void uGx_Object_Add(uGx_ObjСontainer_t *pOwner, uGx_ObjHeader_t *pObject);
// Функция выключения графического объекта
void uGx_Object_ChangeState(uGx_ObjHeader_t *pObject, uGx_ObjState_t State, uGx_ObjDraw_t  Draw);

// Функция включения графического объекта
void uGx_Object_Enable(uGx_ObjHeader_t *pObject);
// Функция выключения графического объекта
void uGx_Object_Disable(uGx_ObjHeader_t *pObject);
// Функция отображения графического объекта
void uGx_Object_Show(uGx_ObjHeader_t *pObject);
// Функция скрытия графического объекта
void uGx_Object_Hide(uGx_ObjHeader_t *pObject);
// Функция обновления графического объекта
void uGx_Object_Update(uGx_ObjHeader_t *pObject);

// Функция установки фокуса на графический объект
void uGx_Object_SetFocus(uGx_ObjHeader_t *pObject);
// Функция поиска следующего графического объекта
uGx_ObjHeader_t* uGx_Object_GetNextFocus(uGx_ObjForm_t *pForm);
// Функция поиска предыдущего графический объект
uGx_ObjHeader_t* uGx_Object_GetPrevFocus(uGx_ObjForm_t *pForm);


/* ОПИСАНИЕ INLINE ФУНКЦИЙ
 ***********************************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
/*
** КОНЕЦ ФАЙЛА
***********************************************************************************************/
