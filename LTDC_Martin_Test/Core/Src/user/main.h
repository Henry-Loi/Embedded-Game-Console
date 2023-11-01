#pragma once

/* ********************
	Some helper macros
   ******************** */
#define __forceinline __attribute__((__always_inline__)) inline

#define get_ticks() HAL_GetTick()