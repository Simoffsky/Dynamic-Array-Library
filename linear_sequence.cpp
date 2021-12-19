#include "linear_sequence.h"
#include<iostream>

extern LSQ_HandleT LSQ_CreateSequence(void) {
	LSQ_HandleT arr = (LSQ_HandleT)malloc(sizeof(DynamicArray) );
	if (arr == NULL) {
		std::cout << "Not Enough Memory, Error..";
		return NULL;
	}
	arr->data = (LSQ_BaseTypeT*)malloc(sizeof (LSQ_BaseTypeT) * 10 );
	if (arr->data == NULL) {
		std::cout << "Not Enough Memory, Error..";
		return NULL;
	}
	arr->size = 0;
	arr->capacity = 10;

	return arr;
}

extern void LSQ_DestroySequence(LSQ_HandleT handle) {
	if (handle->data != NULL)
		free(handle->data);

	free(handle);
	return;
}

//Возвращает размер. -1 если массив неинициализирован
extern LSQ_IntegerIndexT LSQ_GetSize(LSQ_HandleT handle) {
	if (handle == LSQ_HandleInvalid) {
		std::cout << "Array is not Initialized" << std::endl;
		return -1;
	}
	return handle->size;
}

extern int LSQ_IsIteratorDereferencable(LSQ_IteratorT iterator) {
	if ((iterator->index >= (iterator->array->size) ) or (iterator->iter == NULL) or (iterator->index < 0) )
		return 0;

	return 1;

}

extern int LSQ_IsIteratorPastRear(LSQ_IteratorT iterator) {
	LSQ_HandleT arr = iterator->array;
	if ((iterator->iter) == (arr->data + arr->size) )
		return 1;
	
	return 0;
	
}

extern int LSQ_IsIteratorBeforeFirst(LSQ_IteratorT iterator) {
	LSQ_HandleT arr = iterator->array;
	if (iterator->iter == (arr->data - 1) )
		return 1;
	
	return 0;
}

//Разыменовывание итератора(Возвращает ссылку на элемент)
extern LSQ_BaseTypeT* LSQ_DereferenceIterator(LSQ_IteratorT iterator) {
	return iterator->iter;
}

//Следующие 3 функции создают итератор...(Возвращают итератор->NULL в случае ошибки)
extern LSQ_IteratorT LSQ_GetElementByIndex(LSQ_HandleT handle, LSQ_IntegerIndexT index) {
	LSQ_IteratorT iterator = NULL;

	if (index >= handle->size) {
		std::cout << "This Element doesn't exist" << std::endl;
		return iterator;
	}
	iterator = (LSQ_IteratorT)malloc(sizeof(Iterator));
	iterator->index = index;
	iterator->iter = &(handle->data[index]);
	iterator->array = handle;
	return iterator;

}

//Возвращает итератор, указыващий на первый элемент
extern LSQ_IteratorT LSQ_GetFrontElement(LSQ_HandleT handle) {
	LSQ_IteratorT iterator = NULL;
	if (handle->size == 0) {
		std::cout << "Array is Empty!" << std::endl;
		return iterator;
	}
	iterator = (LSQ_IteratorT)malloc(sizeof(Iterator));
	iterator->index = 0;
	iterator->iter = &(handle->data[0]);
	iterator->array = handle;
	return iterator;

}

//Возвращает итератор, ссылающийся на элемент после последнего
extern LSQ_IteratorT LSQ_GetPastRearElement(LSQ_HandleT handle) {
	LSQ_IteratorT iterator = NULL;
	LSQ_IntegerIndexT index_last_plus = handle->data[handle->size];

	if (handle->size == 0) {
		std::cout << "Array is Empty!" << std::endl;
		return iterator;
	}
	iterator = (LSQ_IteratorT)malloc(sizeof(Iterator));
	iterator->index = index_last_plus;
	iterator->iter = &(handle->data[index_last_plus]);
	iterator->array = handle;
	return iterator;
}

//Уничтожает итератор
extern void LSQ_DestroyIterator(LSQ_IteratorT iterator) {
	free(iterator);
	return;
}

//Перемещение итератора на +1
extern void LSQ_AdvanceOneElement(LSQ_IteratorT iterator) {
	iterator->index += 1;
	iterator->iter = &(iterator->array->data[iterator->index]);

	if (iterator->index >= iterator->array->size)
		std::cout << "Iterator is pointing to non-existent element!" << std::endl;
	return;
}

extern void LSQ_RewindOneElement(LSQ_IteratorT iterator) {
	iterator->index -= 1;
	iterator->iter = &(iterator->array->data[iterator->index]);

	if (iterator->index < 0)
		std::cout << "Iterator is pointing to non-existent element!" << std::endl;
	return;

}


//Перемещает итератор на заданное значение(со знаком!)
extern void LSQ_ShiftPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT shift) {
	iterator->index += (shift);
	iterator->iter = &(iterator->array->data[iterator->index]);
	if (iterator->index < 0 or iterator->index >= iterator->array->size )
		std::cout << "Iterator is pointing to non-existent element!" << std::endl;
	return;
}

//Устанавливает итератор на элемент с указаным индексом.
extern void LSQ_SetPosition(LSQ_IteratorT iterator, LSQ_IntegerIndexT pos) {
	iterator->index = pos;
	iterator->iter = &(iterator->array->data[iterator->index]);
	if (iterator->index < 0 or iterator->index >= iterator->array->size)
		std::cout << "Iterator is pointing to non-existent element!" << std::endl;
	return;
}

//Увеличивает capacity в 2 раза
void refill(LSQ_HandleT handle) {
	handle->capacity *= 2;
	handle->data = (LSQ_BaseTypeT*)realloc(handle->data, sizeof(LSQ_BaseTypeT) * handle->capacity);
	if (handle->data == NULL)
		std::cout << "Memory isn't allocated!!!";

	return;
}

//Вставка в начало.
extern void LSQ_InsertFrontElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
	if (handle->capacity + 1 <= handle->size)
		refill(handle);

	handle->size += 1;

	//Сдвиг вправо
	int tmp = handle->data[handle->size - 1];  
	for (int i = handle->size - 1; i > 0; --i) 
		handle->data[i] = handle->data[i - 1];
	handle->data[handle -> size] = tmp;

	handle->data[0] = element;
	
	
	return;
}

//Вставка в Конец.
extern void LSQ_InsertRearElement(LSQ_HandleT handle, LSQ_BaseTypeT element) {
	if (handle->capacity <= handle->size)
		refill(handle);

	handle->data[handle->size] = element;
	handle->size += 1;
	return;
}

//Вставка в индекс, на который указывает итератор.
extern void LSQ_InsertElementBeforeGiven(LSQ_IteratorT iterator, LSQ_BaseTypeT newElement) {
	if (iterator->array->capacity + 1 <= iterator->array->size)
		refill(iterator->array);
	
	int ind = iterator->index;
	LSQ_HandleT handle = iterator->array;
	handle->size += 1;
	//Сдвиг вправо
	int tmp = handle->data[handle->size - 1];

	for (int i = handle->size - 1; i > ind; --i)
		handle->data[i] = handle->data[i - 1];
	handle->data[handle->size] = tmp;

	handle->data[ind] = newElement;

}

//Удаление первого элемента
extern void LSQ_DeleteFrontElement(LSQ_HandleT handle) {
	if (handle->size == 0) {
		std::cout << "Array is Empty!!" << std::endl;
		return;
	}
	for (int i = 0; i < handle->size - 1; ++i)
		handle->data[i] = handle->data[i + 1];
	handle->size -= 1;
	return;
}

//Удаление последнего элемента
extern void LSQ_DeleteRearElement(LSQ_HandleT handle) {
	if (handle->size == 0) {
		std::cout << "Array is Empty!!" << std::endl;
		return;
	}
	handle->size -= 1;
	return;
}

extern void LSQ_DeleteGivenElement(LSQ_IteratorT iterator) {
	LSQ_HandleT handle = iterator->array;
	int ind = iterator->index;
	

	for (int i = ind; i < handle->size - 1; ++i)
		handle->data[i] = handle->data[i + 1];
	handle->size -= 1;
	return;
}


//Отладочная печать
void print(LSQ_HandleT handle) {
	if (handle->size == 0) {
		std::cout << "Nothing to print!" << std::endl;
		return;
	}
	for (int i = 0; i < handle->size; ++i)
		std::cout << handle->data[i] << ' ';
	std::cout << std::endl;

	return;
}

