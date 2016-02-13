/** @file     uGxMessage.c
    @author   Lyga Maksim
    @version  V1.0
    @date     01.09.2015
 */

/* ПОДКЛЮЧЕНИЕ ЗАГОЛОВОЧНЫХ ФАЙЛОВ
 *************************************************************************************************/
 
#include "uGxMessage.h"
#include "uGxSetup.h"
#include "system_stm32f103x.h"

/* КОНСТАНТЫ
 *************************************************************************************************/

/* Глобальные переменные
 *************************************************************************************************/	

/* Локальные переменные
 *************************************************************************************************/

/// Структура очереди сообщений графической библиотеки
volatile static struct
{
	uGxMsg_t Msg[__uGX_MSG_SIZE]; ///< кольцевой буфер сообщений
	uint16_t Write;               ///< указатель записи в буфер
	uint16_t Read;                ///< указатель чтения из буфера
  uint16_t Mask;                ///< маска кольцевого буфера сообщений
} uGx_Message;

/* ОПИСАНИЕ INLINE ФУНКЦИЙ
 *************************************************************************************************/

/* ПРОТОТИПЫ ЛОКАЛЬНЫХ ФУНКЦИЙ
 *************************************************************************************************/	

/* ОПИСАНИЕ ФУНКЦИЙ
 *************************************************************************************************/

/** @brief ункция инициализации очереди сообщений.
 */
void uGx_Msg_Init(void)
{
	uGx_Message.Read  = 0;
	uGx_Message.Write = 0;
	uGx_Message.Mask  = __uGX_MSG_SIZE - 1;
}

/** @brief Функция очистки очереди сообщений.
 */
void uGx_Msg_Clear(void)
{
	uint16_t New_Value;

	do
	{
	  New_Value = __LDREXH(&uGx_Message.Write);
	} while(__STREXH(New_Value, &uGx_Message.Read));	
}

/** @brief Функция создания сообщений графической библиотеки.

    @param[in] Source идентификатор источника сообщения
    @param[in] Event  идентификатор типа события
    @param[in] Param1 дополнительный параметр сообщения 1
    @param[in] Param2 дополнительный параметр сообщения 2
 */
void uGx_Msg_Create(uGxMsgSource_t Source, uGxMsgEvent_t Event, uint16_t Param1, uint16_t Param2)
{
	uint16_t Old_Value;
	uint16_t New_Value;

	do
	{
		// Запоминаем текущий индекс и возводим флаг эксклюзивного доступа
	  Old_Value = __LDREXH(&uGx_Message.Write);
		// Вычисление следующего индекса программного буфера
		New_Value = (Old_Value + 1) & uGx_Message.Mask;
		// Проверка на заполненность буфера
	  if (New_Value == uGx_Message.Read)
	  {
		  New_Value = Old_Value;
		}
	} while(__STREXH(New_Value, &uGx_Message.Write));
	
	// Если индексы не равны, значит есть место для записи
	if (Old_Value != New_Value)
	{
    // запись сообщения в буфер
		uGx_Message.Msg[Old_Value].Source = Source;
		uGx_Message.Msg[Old_Value].Event  = Event;
		uGx_Message.Msg[Old_Value].Param1 = Param1;
		uGx_Message.Msg[Old_Value].Param2 = Param2;
	}
}

/** @brief Функция чтения сообщений из буфера.

    @param[in] pMsg указатель на структуру сообщения в которую производится чтение из очереди

    @return    указатель на структуру сообщения
    @return    NULL если очередь пуста или указатель не корректен
 */
uGxMsg_t* uGx_Msg_Read(uGxMsg_t *const pMsg)
{
  // проверка на корректность указателя
  if (pMsg != NULL)
	{
		// проверка на заполненность буфера
	  if (uGx_Message.Read != uGx_Message.Write)
    {
      pMsg->Source = uGx_Message.Msg[uGx_Message.Read].Source;
		  pMsg->Event  = uGx_Message.Msg[uGx_Message.Read].Event;
		  pMsg->Param1 = uGx_Message.Msg[uGx_Message.Read].Param1;
		  pMsg->Param2 = uGx_Message.Msg[uGx_Message.Read].Param2;
		  //установка указателя чтения на следующее место
			uGx_Message.Read = (uGx_Message.Read + 1) & uGx_Message.Mask;
		  
			return pMsg;
	  }
	  else
		{
			// очередь пуста
			return NULL;
		}
  } 
	else
	{
		// не корректный указатель
		return NULL;
	}
}

/* КОНЕЦ ФАЙЛА
 *************************************************************************************************/
