/** @file     uGxButton.c
    @author   Lyga Maksim
    @version  V0.5
    @date     21.01.2016
 */

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 ***********************************************************************************************/

#include "uGxObject.h"
#include "uGxDriver.h"

/* КОНСТАНТЫ
 ***********************************************************************************************/

/* Глобальные переменные
 ***********************************************************************************************/

/* Локальные переменные
 ***********************************************************************************************/

// Структура окна
static uGx_ObjForm_t Desktop;

/* ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
 ***********************************************************************************************/

/* ОПИСАНИЕ ФУНКЦИЙ
 ***********************************************************************************************/

// Функция инициализации графической библиотеки
void uGx_Init(void)
{
	// Инициализация драйвера
  uGx_Driver_Init();
	// Очистка графического экрана
	uGx_Driver_ClrScr(0x0000);
	
	Desktop.Form.Property.ID      = 0;
	Desktop.Form.Property.Owner   = NULL;
	Desktop.Form.Property.Next    = NULL;
	Desktop.Form.Property.Prev    = NULL;
	Desktop.Form.Property.Type    = uGX_TYPE_FORM;
	Desktop.Form.Property.Visible = uGX_OBJ_VISIBLE;
	Desktop.Form.Property.State   = uGX_STATE_ENABLE;
	Desktop.Form.Property.Draw    = uGX_DRAW_FULL;
	Desktop.Form.Property.Focus   = uGX_FOCUS_DISABLE;
	Desktop.Form.Property.Left    = 0;
	Desktop.Form.Property.Top     = 0;
	Desktop.Form.Property.Width   = uGx_Driver_GetMaxX();
	Desktop.Form.Property.Height  = uGx_Driver_GetMaxY();
	Desktop.Form.Collection       = NULL;
	Desktop.Border.Style          = uGX_BORDER_NONE;
	Desktop.Border.Color          = 0x0000;
	Desktop.FocusObj              = NULL;	
	Desktop.Color                 = 0x0000;
}

// Функция добавления графического объекта
void uGx_Object_Add(uGx_ObjСontainer_t *pOwner, uGx_ObjHeader_t *pObject)
{
	uGx_ObjHeader_t *pObj;

	// Проверка на корректность указателя	
	if (pObject != NULL) 
	{	
	  pObject->Next  = NULL;
		
	  if (pOwner != NULL) 
    {
	    pObject->Owner = &(pOwner->Property);
		  pObj           = pOwner->Collection;
	  }
	  else 
	  {
		  pObject->Owner = &(Desktop.Form.Property);
	  	pObj           = Desktop.Form.Collection;
	  }
		
	  // Проверка на существование объектов в списке
	  if (pObj != NULL)
	  {
		  // Если объекты в списке существуют, ищем последний
		  while (pObj->Next != NULL)
		  {
		    pObj = pObj->Next;
		  }
		  pObj->Next    = pObject;
		  pObject->Prev = pObj;		
	  }		
	  else
	  {
		  // Если объект первый
		  pObject->Prev = NULL;		
	    pOwner->Collection = pObject;
		}
	}
}

// Функция отображения графического объекта
void uGx_Object_Show(uGx_ObjHeader_t *pObject)
{
	// Проверка на корректность указателя	
	if (pObject != NULL) 
	{
		// Если объект невидимый
		if (!pObject->Visible)
		{
			pObject->Visible = uGX_OBJ_VISIBLE;
			pObject->Draw    = uGX_DRAW_FULL;
													 
			// Если объект контейнер
			if (pObject->Type)
			{
			  uGx_ObjСontainer_t *pContainer = (uGx_ObjСontainer_t *)pObject;
				uGx_ObjHeader_t    *pObj       = pContainer->Collection;
        
				// Отображаем все объекты в контейнере
	      while (pObj != NULL)
				{
				  uGx_Object_Show(pObj);
		      pObj = pObj->Next;
	      }
		  }
		}
	}
}

// Функция скрытия графического объекта
void uGx_Object_Hide(uGx_ObjHeader_t *pObject)
{
	// Проверка на корректность указателя	
	if (pObject != NULL) 
	{
		// Если объект видимый
		if (pObject->Visible)
		{
			uGx_ObjHeader_t *pOwner = (uGx_ObjHeader_t *)pObject->Owner;
			
			pObject->Visible = uGX_OBJ_INVISIBLE;
			// Если владелец объекта не скрыт
			if (pOwner->Visible) {pObject->Draw  = uGX_DRAW_FULL;}
			                else {pObject->Draw  = uGX_DRAW_NONE;}
			// Если объект находится в фокусе, то сбрасываем состояние								
			if (pObject->State > uGX_STATE_ENABLE)
			{
				// Здесь надо добавить сброс фокуса на форме !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				pObject->State = uGX_STATE_ENABLE;
			}	
																		 
			// Если объект контейнер
			if (pObject->Type)
			{
			  uGx_ObjСontainer_t *pContainer = (uGx_ObjСontainer_t *)pObject;
				uGx_ObjHeader_t    *pObj       = pContainer->Collection;
        
				// Скрываем все объекты в контейнере
	      while (pObj != NULL)
				{
				  uGx_Object_Hide(pObj);
		      pObj = pObj->Next;
	      }
		  }
		}
	}
}

// Функция выключения графического объекта
void uGx_Object_Disable(uGx_ObjHeader_t *pObject)
{
	// Проверка на корректность указателя	
	if (pObject != NULL) 
	{
		// Проверка на текущее состояние объекта
		if (pObject->State)
		{
			// Если объект находится в фокусе, то сбрасываем состояние								
			if (pObject->State > uGX_STATE_ENABLE)
			{
				// Здесь надо добавить сброс фокуса на форме !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
			
			// Если объект контейнер
			if (pObject->Type)
			{
			  uGx_ObjСontainer_t *pContainer = (uGx_ObjСontainer_t *)pObject;
				uGx_ObjHeader_t    *pObj       = pContainer->Collection;
        
				// Скрываем все объекты в контейнере
	      while (pObj != NULL)
				{
				  uGx_Object_Hide(pObj);
		      pObj = pObj->Next;
	      }
		  }
			
			pObject->State = uGX_STATE_DISABLE;
			pObject->Draw  = uGX_DRAW_FULL;
		}
	}
}

// Функция включения графического объекта
void uGx_Object_Enable(uGx_ObjHeader_t *pObject)
{
	// Проверка на корректность указателя	
	if (pObject != NULL) 
	{
		// Если текущее состояние объекта выключен
		if (pObject->State < uGX_STATE_ENABLE)
		{
			pObject->State = uGX_STATE_ENABLE;
			pObject->Draw  = uGX_DRAW_FULL;
			
			// Если объект контейнер
			if (pObject->Type)
			{
			  uGx_ObjСontainer_t *pContainer = (uGx_ObjСontainer_t *)pObject;
				uGx_ObjHeader_t    *pObj       = pContainer->Collection;
        
				// Обновляем все объекты в контейнере
	      while (pObj != NULL)
				{
				  uGx_Object_Update(pObj);
		      pObj = pObj->Next;
	      }
		  }
		}
	}
}

// Функция обновления графического объекта
void uGx_Object_Update(uGx_ObjHeader_t *pObject)
{
	// Проверка на корректность указателя	
	if (pObject != NULL)
	{
		// Если объект не скрыт	
		if (pObject->Visible)
		{
			pObject->Draw = uGX_DRAW_FULL;
			
			// Если объект контейнер
			if (pObject->Type)
			{
			  uGx_ObjСontainer_t *pContainer = (uGx_ObjСontainer_t *)pObject;
				uGx_ObjHeader_t    *pObj       = pContainer->Collection;
        
				// Обновляем все объекты в контейнере
	      while (pObj != NULL)
				{
				  uGx_Object_Update(pObj);
		      pObj = pObj->Next;
	      }
		  }
	  }
	}
}

// Функция установки фокуса на графический объект
void uGx_Object_SetFocus(uGx_ObjHeader_t *pObject)
{
	// Проверка на корректность указателя	
	if (pObject != NULL)
	{
		// Если на объект возможно переместить фокус и он не в фокусе	
		if ((pObject->Focus) && (pObject->State != uGX_STATE_FOCUS))
		{	
			// Если у объекта есть владелец
			if (pObject->Owner != NULL)
			{
			  uGx_ObjHeader_t *pObj = (uGx_ObjHeader_t *)pObject->Owner;
			  // Поиск формы - владельца
			  while ((pObj->Type != uGX_TYPE_FORM) && (pObj->Owner != NULL))
			  {
			    pObj = pObj->Owner;
			  }
			
				// Если найденный объект - форма
			  if (pObj->Type == uGX_TYPE_FORM)
			  {
			    uGx_ObjForm_t *pForm = (uGx_ObjForm_t *)pObj;
				  
					// Сброс фокуса с предыдущего объекта
		      if (pForm->FocusObj != NULL)
		      {
				    pForm->FocusObj->State = uGX_STATE_ENABLE;
			      if (pForm->FocusObj->Draw == uGX_DRAW_NONE) {pForm->FocusObj->Draw = uGX_DRAW_UPDATE;}
		      }
				
		      pObject->State = uGX_STATE_FOCUS;
					if (pObject->Draw == uGX_DRAW_NONE) {pObject->Draw = uGX_DRAW_UPDATE;}
		      pForm->FocusObj = pObject;
				}
		  }
		}
	}
}


// Функция поиска следующего графического объекта
uGx_ObjHeader_t* uGx_Object_GetNextFocus(uGx_ObjForm_t *pForm)
{
	uGx_ObjHeader_t *pObj = NULL;
	uint8_t          Find = 0;
	
	// Проверка на корректность указателя	
	if (pForm != NULL)
	{
		if (pForm->FocusObj != NULL) {pObj = pForm->FocusObj->Next;}
	                          else {pObj = pForm->Form.Collection;}
		
	  while ((pObj != NULL) && (!Find))
	  {
	    if ((pObj->Focus) && (pObj->State == uGX_STATE_ENABLE)) {Find = 1;}
                                                         else	{pObj = pObj->Next;}
	  }	
	}
	
	return pObj;
}

// Функция поиска предыдущего графический объект
uGx_ObjHeader_t* uGx_Object_GetPrevFocus(uGx_ObjForm_t *pForm)
{
	uGx_ObjHeader_t *pObj = NULL;
	uint8_t          Find = 0;
	
	// Проверка на корректность указателя	
	if (pForm != NULL)
	{
		if (pForm->FocusObj != NULL) {pObj = pForm->FocusObj->Prev;}
	                          else {pObj = pForm->Form.Collection;}
	
	  while ((pObj != NULL) && (!Find))
	  {
	    if ((pObj->Focus) && (pObj->State == uGX_STATE_ENABLE)) {Find = 1;}
                                                         else	{pObj = pObj->Prev;}
	  }	
	}
	
	return pObj;
}

/* КОНЕЦ ФАЙЛА
 ***********************************************************************************************/
