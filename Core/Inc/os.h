#ifndef OS_H
#define OS_H

// clang-format off
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

#include <stdint.h>
// clang-format on

// Tried to make it robocon like? Seems to work so far with led blinky at least
// basically just define thread attributes (priority/stack size)
// and then create the actual thread by linking with the actual task function
// somewhat backwards when compared with our library? but hey it seems to be working I ain't complaining

#define DEFINE_THREAD_ATTR_SIZED(_name, _size, _prio) \
	const osThreadAttr_t _name##_attr = {             \
		.name = "_name",                              \
		.stack_size = _size,                          \
		.priority = (osPriority_t)_prio,              \
	}

#define DEFINE_THREAD_ATTR(_name, _prio) DEFINE_THREAD_ATTR_SIZED(_name, configMINIMAL_STACK_SIZE, _prio)

#define CREATE_THREAD(_name, _handler, _args) osThreadId_t _name##_handle = osThreadNew(_handler, _args, &_name##_attr)

#define STATIC_TASK_STRUCT(StackSize) \
	struct {                          \
		const uint32_t stack_size;    \
		const char* name;             \
		TaskFunction_t taskfunc;      \
		TaskHandle_t handle;          \
		StaticTask_t tcb;             \
		StackType_t stack[StackSize]; \
	}

typedef STATIC_TASK_STRUCT() GenericStaticTaskStruct;

#define CONSTRUCT_THREAD(Func, StackSize) \
	{ StackSize, #Func, Func, 0 }

#define DEFINE_THREAD(Task, Func) DEFINE_THREAD_SIZED(Task, Func, configMINIMAL_STACK_SIZE)

#define DEFINE_THREAD_SIZED(Task, Func, StackSize)              \
	void Func(void*); /*function declaration to avoid include*/ \
	STATIC_TASK_STRUCT(StackSize) Task = CONSTRUCT_THREAD(Func, StackSize);

// create static thread when
#define os_create_thread(Task, Par, Prio) \
	Task.handle = xTaskCreateStatic(Task.taskfunc, Task.name, Task.stack_size, (void*)Par, Prio, Task.stack, &Task.tcb)

#endif
