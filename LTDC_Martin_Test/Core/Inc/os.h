#ifndef OS_H
#define OS_H

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

#endif