#include "tetris.h"

#include "delay.h"
// #include "gui.h"
// #include "menu_control.h"
// #include "test.h"
// #include "uart.h"
#include "lcd.h"

#define u8	uint8_t
#define u16 uint16_t
u16 POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;

/**********************************************************************************/
// Tetris��������λ���ж�
u8 BeCode[32][24] = {
	/*   0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23*/
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********0**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********1**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********2**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********3**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********4**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********5**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********6**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********7**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********8**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********9**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********10**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********11**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********12**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********13**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********14**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********15**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********16**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********17**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********18**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********19**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********20**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********21**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********21**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********23**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********24**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********25**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********26**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********27**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********28**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********29**********//
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, //**********30**********//
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, //**********31**********//
};

/**********************************************************************************/
//�������Ʊ���
int TETRIS_KEY_RECEIVER = 0;
static int TETRIS_X_POSITION = 110;
static int TETRIS_Y_POSITION = 40;
static int RandomNumber = 2;
extern int Go_Down;
static int CreatNewBlock = 300;
static int YourLevel = 0;
static int YourScore = 0;
int previews[50] = {2, 8, 7, 9, 2, 3, 1, 2, 7, 4, 8, 7, 14, 8, 17, 8, 6, 18, 9, 8, 10, 5, 10};
int CountPreview = 1;
/***************************************
//����Ϸ�߽�
***************************************/
void Tetris_Boundary(void) {
	tft_fill_rect(0, 0, 240, 10, BROWN);
	tft_fill_rect(0, 0, 10, 310, BROWN);
	tft_fill_rect(240 - 10, 0, 240, 310, BROWN);
	tft_fill_rect(0, 310 - 10, 240, 310, BROWN);
}
/**************************************
//��һ��10X10���صķ���
*************************************/
void DrawBlock(u16 x, u16 y) {
	u8 i, n;
	for (i = 1; i <= 8; i++) {
		for (n = 1; n <= 8; n++) {
			tft_draw_point((x + i), (y - n), GRAY);
		}
	}
	for (i = 0; i <= 9; i++) {
		tft_draw_point((x + i), y, BLACK);
		tft_draw_point((x + i), (y - 9), BLACK);
		tft_draw_point(x, (y - i), BLACK);
		tft_draw_point((x + 9), (y - i), BLACK);
	}
}
/*****************************************
//ɾ��һ��һ��10X10���صķ���
*****************************************/
void DeleteBlock(u16 x, u16 y) {
	u8 i, n;
	for (i = 0; i <= 9; i++) {
		for (n = 0; n <= 9; n++) {
			tft_draw_point((x + i), (y - n), WHITE);
		}
	}
}
/*************************************************
//���������ͼ��?
*************************************************/
void DrawShape(u16 x, u16 y, u8 what) {
	switch (what) {
		case 1: {
			DrawBlock(x, y);
			DrawBlock(x + 10, y);
			DrawBlock(x, y - 10);
			DrawBlock(x + 10, y - 10);
		} break;
		case 2: {
			DrawBlock(x, y);
			DrawBlock(x + 10, y);
			DrawBlock(x + 20, y);
			DrawBlock(x + 30, y);
		} break;
		case 3: {
			DrawBlock(x, y);
			DrawBlock(x, y - 10);
			DrawBlock(x, y - 20);
			DrawBlock(x, y - 30);
		} break;
		case 4: {
			DrawBlock(x, y);
			DrawBlock(x + 10, y - 10);
			DrawBlock(x + 10, y);
			DrawBlock(x + 20, y);
		} break;
		case 5: {
			DrawBlock(x, y);
			DrawBlock(x + 10, y - 10);
			DrawBlock(x, y - 10);
			DrawBlock(x, y - 20);
		} break;
		case 6: {
			DrawBlock(x, y);
			DrawBlock(x - 10, y - 10);
			DrawBlock(x, y - 10);
			DrawBlock(x, y - 20);
		} break;
		case 7: {
			DrawBlock(x - 10, y - 10);
			DrawBlock(x, y);
			DrawBlock(x, y - 10);
			DrawBlock(x + 10, y - 10);
		} break;
		case 8: {
			DrawBlock(x, y);
			DrawBlock(x, y - 10);
			DrawBlock(x, y - 20);
			DrawBlock(x + 10, y);
		} break;
		case 9: {
			DrawBlock(x, y);
			DrawBlock(x, y - 10);
			DrawBlock(x + 10, y - 10);
			DrawBlock(x + 20, y - 10);
		} break;
		case 10: {
			DrawBlock(x, y);
			DrawBlock(x, y - 10);
			DrawBlock(x, y - 20);
			DrawBlock(x - 10, y - 20);
		} break;
		case 11: {
			DrawBlock(x, y);
			DrawBlock(x + 10, y);
			DrawBlock(x + 20, y);
			DrawBlock(x + 20, y - 10);
		} break;
		case 12: {
			DrawBlock(x, y);
			DrawBlock(x + 10, y);
			DrawBlock(x + 10, y - 10);
			DrawBlock(x + 10, y - 20);
		} break;
		case 13: {
			DrawBlock(x, y);
			DrawBlock(x + 10, y);
			DrawBlock(x, y - 10);
			DrawBlock(x, y - 20);
		} break;
		case 14: {
			DrawBlock(x, y);
			DrawBlock(x, y - 10);
			DrawBlock(x, y - 20);
			DrawBlock(x + 10, y - 20);
		} break;
		case 15: {
			DrawBlock(x, y);
			DrawBlock(x, y - 10);
			DrawBlock(x + 10, y);
			DrawBlock(x + 20, y);
		} break;
		case 16: {
			DrawBlock(x, y);
			DrawBlock(x, y - 10);
			DrawBlock(x + 10, y - 10);
			DrawBlock(x + 10, y - 20);
		} break;
		case 17: {
			DrawBlock(x, y);
			DrawBlock(x + 10, y);
			DrawBlock(x, y - 10);
			DrawBlock(x - 10, y - 10);
		} break;
		case 18: {
			DrawBlock(x, y);
			DrawBlock(x, y - 10);
			DrawBlock(x - 10, y - 10);
			DrawBlock(x - 10, y - 20);
		} break;
		case 19: {
			DrawBlock(x, y);
			DrawBlock(x + 10, y);
			DrawBlock(x + 10, y - 10);
			DrawBlock(x + 20, y - 10);
		} break;
	}
}
/*************************************************
//�����������ͼ��?
*************************************************/
void DeleteShape(u16 x, u16 y, u8 what) {
	switch (what) {
		case 1: {
			DeleteBlock(x, y);
			DeleteBlock(x + 10, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x + 10, y - 10);
		} break;
		case 2: {
			DeleteBlock(x, y);
			DeleteBlock(x + 10, y);
			DeleteBlock(x + 20, y);
			DeleteBlock(x + 30, y);
		} break;
		case 3: {
			DeleteBlock(x, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x, y - 20);
			DeleteBlock(x, y - 30);
		} break;
		case 4: {
			DeleteBlock(x, y);
			DeleteBlock(x + 10, y - 10);
			DeleteBlock(x + 10, y);
			DeleteBlock(x + 20, y);
		} break;
		case 5: {
			DeleteBlock(x, y);
			DeleteBlock(x + 10, y - 10);
			DeleteBlock(x, y - 10);
			DeleteBlock(x, y - 20);
		} break;
		case 6: {
			DeleteBlock(x, y);
			DeleteBlock(x - 10, y - 10);
			DeleteBlock(x, y - 10);
			DeleteBlock(x, y - 20);
		} break;
		case 7: {
			DeleteBlock(x - 10, y - 10);
			DeleteBlock(x, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x + 10, y - 10);
		} break;
		case 8: {
			DeleteBlock(x, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x, y - 20);
			DeleteBlock(x + 10, y);
		} break;
		case 9: {
			DeleteBlock(x, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x + 10, y - 10);
			DeleteBlock(x + 20, y - 10);
		} break;
		case 10: {
			DeleteBlock(x, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x, y - 20);
			DeleteBlock(x - 10, y - 20);
		} break;
		case 11: {
			DeleteBlock(x, y);
			DeleteBlock(x + 10, y);
			DeleteBlock(x + 20, y);
			DeleteBlock(x + 20, y - 10);
		} break;
		case 12: {
			DeleteBlock(x, y);
			DeleteBlock(x + 10, y);
			DeleteBlock(x + 10, y - 10);
			DeleteBlock(x + 10, y - 20);
		} break;
		case 13: {
			DeleteBlock(x, y);
			DeleteBlock(x + 10, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x, y - 20);
		} break;
		case 14: {
			DeleteBlock(x, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x, y - 20);
			DeleteBlock(x + 10, y - 20);
		} break;
		case 15: {
			DeleteBlock(x, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x + 10, y);
			DeleteBlock(x + 20, y);
		} break;
		case 16: {
			DeleteBlock(x, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x + 10, y - 10);
			DeleteBlock(x + 10, y - 20);
		} break;
		case 17: {
			DeleteBlock(x, y);
			DeleteBlock(x + 10, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x - 10, y - 10);
		} break;
		case 18: {
			DeleteBlock(x, y);
			DeleteBlock(x, y - 10);
			DeleteBlock(x - 10, y - 10);
			DeleteBlock(x - 10, y - 20);
		} break;
		case 19: {
			DeleteBlock(x, y);
			DeleteBlock(x + 10, y);
			DeleteBlock(x + 10, y - 10);
			DeleteBlock(x + 20, y - 10);
		} break;
	}
}
/*************************************************
����һ��״̬������
*************************************************/
void Draw_a_zhuangtai(u16 x, u16 y) { BeCode[y / 10][x / 10] = 1; }
/*************************************************
ɾ��һ��״̬������
*************************************************/
void Deal_a_zhuangtai(u16 x, u16 y) { BeCode[y / 10][x / 10] = 0; }
/*************************************************
����Ӧ��ͼ��״̬
*************************************************/
void DrawShapeState(u16 x, u16 y, u8 what) {
	switch (what) {
		case 1: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x + 10, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x + 10, y - 10);
		} break;
		case 2: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x + 10, y);
			Draw_a_zhuangtai(x + 20, y);
			Draw_a_zhuangtai(x + 30, y);
		} break;
		case 3: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x, y - 20);
			Draw_a_zhuangtai(x, y - 30);
		} break;
		case 4: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x + 10, y - 10);
			Draw_a_zhuangtai(x + 10, y);
			Draw_a_zhuangtai(x + 20, y);
		} break;
		case 5: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x + 10, y - 10);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x, y - 20);
		} break;
		case 6: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x - 10, y - 10);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x, y - 20);
		} break;
		case 7: {
			Draw_a_zhuangtai(x - 10, y - 10);
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x + 10, y - 10);
		} break;
		case 8: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x, y - 20);
			Draw_a_zhuangtai(x + 10, y);
		} break;
		case 9: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x + 10, y - 10);
			Draw_a_zhuangtai(x + 20, y - 10);
		} break;
		case 10: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x, y - 20);
			Draw_a_zhuangtai(x - 10, y - 20);
		} break;
		case 11: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x + 10, y);
			Draw_a_zhuangtai(x + 20, y);
			Draw_a_zhuangtai(x + 20, y - 10);
		} break;
		case 12: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x + 10, y);
			Draw_a_zhuangtai(x + 10, y - 10);
			Draw_a_zhuangtai(x + 10, y - 20);
		} break;
		case 13: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x + 10, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x, y - 20);
		} break;
		case 14: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x, y - 20);
			Draw_a_zhuangtai(x + 10, y - 20);
		} break;
		case 15: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x + 10, y);
			Draw_a_zhuangtai(x + 20, y);
		} break;
		case 16: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x + 10, y - 10);
			Draw_a_zhuangtai(x + 10, y - 20);
		} break;
		case 17: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x + 10, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x - 10, y - 10);
		} break;
		case 18: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x, y - 10);
			Draw_a_zhuangtai(x - 10, y - 10);
			Draw_a_zhuangtai(x - 10, y - 20);
		} break;
		case 19: {
			Draw_a_zhuangtai(x, y);
			Draw_a_zhuangtai(x + 10, y);
			Draw_a_zhuangtai(x + 10, y - 10);
			Draw_a_zhuangtai(x + 20, y - 10);
		} break;
	}
}
/*************************************************
ɾ����Ӧ��ͼ��״̬
*************************************************/
void DeleteShapeState(u16 x, u16 y, u8 what) {
	switch (what) {
		case 1: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x + 10, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x + 10, y - 10);
		} break;
		case 2: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x + 10, y);
			Deal_a_zhuangtai(x + 20, y);
			Deal_a_zhuangtai(x + 30, y);
		} break;
		case 3: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x, y - 20);
			Deal_a_zhuangtai(x, y - 30);
		} break;
		case 4: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x + 10, y - 10);
			Deal_a_zhuangtai(x + 10, y);
			Deal_a_zhuangtai(x + 20, y);
		} break;
		case 5: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x + 10, y - 10);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x, y - 20);
		} break;
		case 6: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x - 10, y - 10);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x, y - 20);
		} break;
		case 7: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x - 10, y - 10);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x + 10, y - 10);
		} break;
		case 8: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x, y - 20);
			Deal_a_zhuangtai(x + 10, y);
		} break;
		case 9: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x + 10, y - 10);
			Deal_a_zhuangtai(x + 20, y - 10);
		} break;
		case 10: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x, y - 20);
			Deal_a_zhuangtai(x - 10, y - 20);
		} break;
		case 11: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x + 10, y);
			Deal_a_zhuangtai(x + 20, y);
			Deal_a_zhuangtai(x + 20, y - 10);
		} break;
		case 12: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x + 10, y);
			Deal_a_zhuangtai(x + 10, y - 10);
			Deal_a_zhuangtai(x + 10, y - 20);
		} break;
		case 13: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x + 10, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x, y - 20);
		} break;
		case 14: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x, y - 20);
			Deal_a_zhuangtai(x + 10, y - 20);
		} break;
		case 15: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x + 10, y);
			Deal_a_zhuangtai(x + 20, y);
		} break;
		case 16: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x + 10, y - 10);
			Deal_a_zhuangtai(x + 10, y - 20);
		} break;
		case 17: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x + 10, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x - 10, y - 10);
		} break;
		case 18: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x, y - 10);
			Deal_a_zhuangtai(x - 10, y - 10);
			Deal_a_zhuangtai(x - 10, y - 20);
		} break;
		case 19: {
			Deal_a_zhuangtai(x, y);
			Deal_a_zhuangtai(x + 10, y);
			Deal_a_zhuangtai(x + 10, y - 10);
			Deal_a_zhuangtai(x + 20, y - 10);
		} break;
	}
}
/********************************************************
����˹�����?����
*********************************************************/
void TransFormer(u16 x, u16 y) {
	switch (RandomNumber) {
		case 1: break;
		case 2: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber++;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 3: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber--;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 4: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber++;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 5: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber += 2;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 6: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber -= 2;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 7: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber -= 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 8: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber += 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 9: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber += 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 10: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber += 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 11: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber -= 3;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 12: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber += 3;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 13: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber -= 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 14: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber -= 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 15: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber -= 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 16: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber += 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 17: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber -= 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 18: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber += 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
		case 19: {
			DeleteShape(x, y, RandomNumber);
			DeleteShapeState(x, y, RandomNumber);
			RandomNumber -= 1;
			DrawShape(x, y, RandomNumber);
			DrawShapeState(x, y, RandomNumber);
		} break;
	}
}
/*************************************
��������ͼ���˶�
*************************************/
void Tetris_Game_Control(void) {
	//�ж���һ�������Ƿ񵽴��ˣ����һ��?
	//������������һ�У����²���һ��������ţ���ʹ�÷���ӵ�һ�е�ĳ��λ�ó���

	// TETRIS_KEY_RECEIVER = Menu_Control_Read2(); //��ȡ����ֵ
	TETRIS_KEY_RECEIVER = 0;	 //��ȡ����ֵ
	switch (TETRIS_KEY_RECEIVER) //���ݰ���ֵƥ�亯��
	{
		case 2: {
			delay(200);
			TransFormer(TETRIS_X_POSITION, TETRIS_Y_POSITION);
			// Usart1_Send(BeCode[0],768);
		} break;
		case 10: //�����ƶ�һ����λ
		{
			delay(200);
			DeleteShape(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			DeleteShapeState(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			TETRIS_Y_POSITION += 10;
			DrawShape(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			DrawShapeState(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			// Usart1_Send(BeCode[0],768);
		} break;
		case 5: //�����ƶ�һ����λ
		{
			delay(200);
			DeleteShape(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			DeleteShapeState(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			switch (RandomNumber) {
				case 1:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 10) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 2:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						TETRIS_X_POSITION -= 10;
					break;
				case 3:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 30) / 10][(TETRIS_X_POSITION - 10) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 4:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						TETRIS_X_POSITION -= 10;
					break;
				case 5:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION - 10) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 6:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 20) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 7:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 20) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 8:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION - 10) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 9:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 10) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 10:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION - 20) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 11:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						TETRIS_X_POSITION -= 10;
					break;
				case 12:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						TETRIS_X_POSITION -= 10;
					break;
				case 13:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 30) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 14:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION - 10) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 15:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 10) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 16:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 10) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 17:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 20) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 18:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION - 20) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION - 20) / 10] != 1))
						TETRIS_X_POSITION -= 10;
					break;
				case 19:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION - 10) / 10] != 1)
						TETRIS_X_POSITION -= 10;
					break;
			}
			DrawShape(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			DrawShapeState(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			// Usart1_Send(BeCode[0],768);
		} break;
		case 7: //�����ƶ�һ����λ
		{
			delay(200);
			DeleteShape(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			DeleteShapeState(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);

			switch (RandomNumber) {
				case 1:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 20) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 20) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 2:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 40) / 10] != 1)
						TETRIS_X_POSITION += 10;
					break;
				case 3:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 30) / 10][(TETRIS_X_POSITION + 10) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 4:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 30) / 10] != 1)
						TETRIS_X_POSITION += 10;
					break;
				case 5:
					if ((BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 20) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION) / 10][(TETRIS_X_POSITION + 10) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 6:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION + 10) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 7:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 20) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 8:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 20) / 10] != 1)
						TETRIS_X_POSITION += 10;
					break;
				case 9:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 30) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 10:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION + 10) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 11:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 30) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 30) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 12:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION + 10) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 13:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 10) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 14:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION + 20) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 15:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 30) / 10] != 1)
						TETRIS_X_POSITION += 10;
					break;
				case 16:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 20) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 20) / 10][(TETRIS_X_POSITION + 20) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 17:
					if (BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 20) / 10] != 1)
						TETRIS_X_POSITION += 10;
					break;
				case 18:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 10) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 10) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
				case 19:
					if ((BeCode[TETRIS_Y_POSITION / 10][(TETRIS_X_POSITION + 20) / 10] != 1)
						|| (BeCode[(TETRIS_Y_POSITION - 10) / 10][(TETRIS_X_POSITION + 30) / 10] != 1))
						TETRIS_X_POSITION += 10;
					break;
			}
			DrawShape(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			DrawShapeState(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
			// Usart1_Send(BeCode[0],768);
		} break;
		case 16:
			NVIC_SystemReset();
			break;
			//		default :
			//		{
			//			delay(200);
			//			DeleteShape(TETRIS_X_POSITION,TETRIS_Y_POSITION,RandomNumber);
			//			DeleteShapeState(TETRIS_X_POSITION,TETRIS_Y_POSITION,RandomNumber);
			//			TETRIS_Y_POSITION += Go_Down;
			//			DrawShape(TETRIS_X_POSITION,TETRIS_Y_POSITION,RandomNumber);
			//			DrawShapeState(TETRIS_X_POSITION,TETRIS_Y_POSITION,RandomNumber);
			//			//Usart1_Send(BeCode[0],768);
			//		}break;
	}
}
/************************************
���ͼ���?���Ƿ�����ײ
************************************/
void Whether_Impact_Happened(u16 x, u16 y, u8 what) {
	switch (what) {
		case 1:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y + 10) / 10][(x + 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 2:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y + 10) / 10][(x + 10) / 10] == 1)
				|| (BeCode[(y + 10) / 10][(x + 20) / 10] == 1) || (BeCode[(y + 10) / 10][(x + 30) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 3:
			if (BeCode[(y + 10) / 10][x / 10] == 1)
				CreatNewBlock = 500;
			break;
		case 4:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y + 10) / 10][(x + 10) / 10] == 1)
				|| (BeCode[(y + 10) / 10][(x + 20) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 5:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y) / 10][(x + 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 6:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y) / 10][(x - 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 7:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y) / 10][(x - 10) / 10] == 1)
				|| (BeCode[(y) / 10][(x + 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 8:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y + 10) / 10][(x + 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 9:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y) / 10][(x + 10) / 10] == 1)
				|| (BeCode[(y) / 10][(x + 20) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 10:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y - 10) / 10][(x - 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 11:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y + 10) / 10][(x + 10) / 10] == 1)
				|| (BeCode[(y + 10) / 10][(x + 20) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 12:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y + 10) / 10][(x + 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 13:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y) / 10][(x - 10) / 10] == 1)
				|| (BeCode[(y) / 10][(x - 20) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 14:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y - 10) / 10][(x + 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 15:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y + 10) / 10][(x + 10) / 10] == 1)
				|| (BeCode[(y + 10) / 10][(x + 20) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 16:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y) / 10][(x + 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 17:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y + 10) / 10][(x + 10) / 10] == 1)
				|| (BeCode[(y) / 10][(x - 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 18:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y) / 10][(x - 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
		case 19:
			if ((BeCode[(y + 10) / 10][x / 10] == 1) || (BeCode[(y + 10) / 10][(x + 10) / 10] == 1)
				|| (BeCode[(y) / 10][(x + 10) / 10] == 1))
				CreatNewBlock = 500;
			break;
	}
}
/***************************************
�����һ�б������������,������
***************************************/
/***********************************
���ƺ���
***********************************************/
void Line_Move(u16 y) //�����N+1�ж���1
{
	u8 i;
	y = y / 10;
	for (i = 1; i < 23; i++) //�ƶ�
	{
		//����if������һ������һ�е�ͼ����״̬���滻
		if (BeCode[y][i] == 1) {
			BeCode[y][i] = BeCode[y + 1][i];
			BeCode[y + 1][i] = 1;
		}
		if (BeCode[y][i] == 0) {
			BeCode[y][i] = BeCode[y + 1][i];
			BeCode[y + 1][i] = 0;
		}
	}
}
/*
ת��ȫΪ1�е�״̬����
ͬʱɾ��LCD��ʾ���ϵ�һ��
*/
void Delete_Line(u16 y) {
	u8 i;
	y = y / 10;
	for (i = 1; i < 23; i++) {
		BeCode[y][i] = 0;
		DeleteBlock((i)*10, y * 10);
	}
}
/*****************************************
��������������
*****************************************/
void Earase(void) {
	u8 Refresh_Lcd_x, Refresh_Lcd_y;
	u8 n;
	for (n = 29; n > 0; n--) {
		if (n >= 29) {
			n = 29;
		}
		if ((BeCode[n + 1][1] && BeCode[n + 1][2] && BeCode[n + 1][3] && BeCode[n + 1][4] && BeCode[n + 1][5]
			 && BeCode[n + 1][6] && BeCode[n + 1][7] && BeCode[n + 1][8] && BeCode[n + 1][9] && BeCode[n + 1][10]
			 && BeCode[n + 1][11] && BeCode[n + 1][12] && BeCode[n + 1][13] && BeCode[n + 1][14] && BeCode[n + 1][15]
			 && BeCode[n + 1][16] && BeCode[n + 1][17] && BeCode[n + 1][18] && BeCode[n + 1][19] && BeCode[n + 1][20]
			 && BeCode[n + 1][21] && BeCode[n + 1][22])

			&& //�����N�в���1�������У���N+1���ǳ���1����
			!(BeCode[n][1] && BeCode[n][2] && BeCode[n][3] && BeCode[n][4] && BeCode[n][5] && BeCode[n][6]
			  && BeCode[n][7] && BeCode[n][8] && BeCode[n][9] && BeCode[n][10] && BeCode[n][11] && BeCode[n][12]
			  && BeCode[n][13] && BeCode[n][14] && BeCode[n][15] && BeCode[n][16] && BeCode[n][17] && BeCode[n][18]
			  && BeCode[n][19] && BeCode[n][20] && BeCode[n][21] && BeCode[n][22])) {
			Line_Move(10 * n); //�������ƺ���
			n = n + 2;
		}
		if (( //�����N�ж�Ϊ0
				!BeCode[n][1] && !BeCode[n][2] && !BeCode[n][3] && !BeCode[n][4] && !BeCode[n][5] && !BeCode[n][6]
				&& !BeCode[n][7] && !BeCode[n][8] && !BeCode[n][9] && !BeCode[n][10] && !BeCode[n][11] && !BeCode[n][12]
				&& !BeCode[n][13] && !BeCode[n][14] && !BeCode[n][15] && !BeCode[n][16] && !BeCode[n][17]
				&& !BeCode[n][18] && !BeCode[n][19] && !BeCode[n][20] && !BeCode[n][21] && !BeCode[n][22])) {
			for (n = 0; n < 31; n++) {
				if (( //�����һ�ж��?1��
						BeCode[n][1] && BeCode[n][2] && BeCode[n][3] && BeCode[n][4] && BeCode[n][5] && BeCode[n][6]
						&& BeCode[n][7] && BeCode[n][8] && BeCode[n][9] && BeCode[n][10] && BeCode[n][11]
						&& BeCode[n][12] && BeCode[n][13] && BeCode[n][14] && BeCode[n][15] && BeCode[n][16]
						&& BeCode[n][17] && BeCode[n][18] && BeCode[n][19] && BeCode[n][20] && BeCode[n][21]
						&& BeCode[n][22])) {
					Delete_Line(n * 10); //��ת����е�״�?��ʹ��Ϊ0
					tft_fill_rect(10, 10, 230, 310, WHITE);
					delay(1);
					for (Refresh_Lcd_y = 1; Refresh_Lcd_y < 31; Refresh_Lcd_y++) {
						for (Refresh_Lcd_x = 1; Refresh_Lcd_x < 23; Refresh_Lcd_x++) {
							if (BeCode[Refresh_Lcd_y][Refresh_Lcd_x] == 1) {
								DrawBlock(Refresh_Lcd_x * 10, Refresh_Lcd_y * 10);
							}
						}
					}
					YourScore++;
					if ((YourScore % 5) == 0) {
						YourLevel++;
					}
				}
			}
			break;
		}
	}
}
/***************************************
//��ʾ������ȼ�?
***************************************/
void DisplayScore(void) {
	tft_fill_rect(240, 0, 320, 5, BLUE);
	tft_fill_rect(320 - 5, 0, 320, 310, BLUE);
	tft_fill_rect(240, 155, 320, 160, BLUE);
	tft_fill_rect(240, 0, 245, 310, BLUE);
	tft_fill_rect(240, 310 - 5, 320, 310, BLUE);
	POINT_COLOR = BLUE;
	tft_prints(250, 170, 16, " LEVEL:", 1);
	// LCD_ShowNum_New(250, 200, RED, WHITE, 4, YourLevel, 16, 0);

	tft_prints(250, 230, 16, " SCORE:", 1);
	// LCD_ShowNum_New(250, 260, RED, WHITE, 4, YourScore, 16, 0);
	tft_prints(40, 330, 16, " KEY2:Change Shape", 1);
	tft_prints(40, 350, 16, " KEY0:Move Down", 1);
	tft_prints(40, 370, 16, " KEY5:Move Left", 1);
	tft_prints(40, 390, 16, " KEY7:Move Right", 1);
}
/***************************************
Ԥ����һ��ͼ��
***************************************/
void PreviewNext(void) {
	POINT_COLOR = RED;
	tft_prints(250, 40, 16, " NEXT:", 1);
	if (CreatNewBlock == 300) {
		CreatNewBlock = 0;
		CountPreview = rand() % 20;
		if (CountPreview == 0)
			CountPreview = 2;
		DrawShape(265, 120, CountPreview);
	}
	if (CreatNewBlock > 400) {
		DeleteShape(265, 120, CountPreview);
		CreatNewBlock = 0;
		TETRIS_Y_POSITION = 40;
		TETRIS_X_POSITION = 110;
		RandomNumber = CountPreview;
		CountPreview = rand() % 20;
		if (CountPreview == 0)
			CountPreview = 2;
		DrawShape(265, 120, CountPreview);
	}
}

/***************************************
��Ϸ������ں���?
***************************************/
int i;
unsigned char count[1000];
void tetris_thread(void* par) {
	Tetris_Boundary(); //������Ϸ�ı߽���
	while (1) {
		osDelay(4);
		PreviewNext();		   // Ԥ����һ��ͼ��
		Tetris_Game_Control(); // TETRIS�İ�������
		Whether_Impact_Happened(TETRIS_X_POSITION, TETRIS_Y_POSITION, RandomNumber);
		// tft_prints(0, 0, "test");
		// tft_update();
	}
	// //�ж��Ƿ�������ײ Earase();
	// //�������� DisplayScore(); //��ʾ������ȼ�?
}
