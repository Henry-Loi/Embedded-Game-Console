
#ifndef _TOUCH_H_
#define _TOUCH_H_

/* reference: https://www.bilibili.com/read/cv27031518/?spm_id_from=333.999.0.0 */

#include "main.h"

#define TOUCH_PID  "911"
#define TOUCH_PID1 "9147"
#define TOUCH_PID2 "9271"
#define TOUCH_PID3 "1158"

#define MAX_TOUCH_POINTS 5

#define REG_CTRL   0x8040
#define REG_PID	   0x8140
#define REG_TPINFO 0x814E
#define REG_TP1	   0x8150
#define REG_TP2	   0x8158
#define REG_TP3	   0x8160
#define REG_TP4	   0x8168
#define REG_TP5	   0x8170

#define TPINFO_MASK_STA 0x80
#define TPINFO_MASK_CNT 0x0F

static const uint16_t touch_tp_reg[MAX_TOUCH_POINTS] = {
	REG_TP1, REG_TP2, REG_TP3, REG_TP4, REG_TP5,
};

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t size;
} TouchPoint_t;

typedef struct {
	uint32_t pressed_state;
	uint32_t last_pressed_state;
	uint8_t clicked;
	uint8_t num;
	TouchPoint_t point[MAX_TOUCH_POINTS];
} TouchFeedback_t;


extern TouchFeedback_t touch_feedback;

void touch_update(void);
void touch_init(void);
#endif /* _TOUCH_H_ */
