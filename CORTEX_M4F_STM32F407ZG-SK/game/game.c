#include<stdio.h>
#include<stdlib.h>
#include<FreeRTOS.h>
#include<task.h>
#include<math.h>
#include<string.h>
#include<main.h>
#include<r3d.h>
#include<time.h>


//ball

uint16_t ballSize = 5;
int16_t ballX = 10;
int16_t ballY = LCD_PIXEL_HEIGHT/2;
int16_t isDead = 0;
int16_t playerGo = 1;

//wall

int16_t wallW = 10;
int16_t wallL = 40;
int16_t wallN = LCD_PIXEL_WIDTH / 10;
int16_t wallY = LCD_PIXEL_HEIGHT / 10;
int16_t wallMove = 0;
int16_t change = 0;

int count =0;

int score = 0;

int posX[8] = {30,50,70,100,150,175,200,220};
int posY[8] = {50,40,100,160,30,250,290,70};

void BallReset(){
	ballX = 10;
	ballY = LCD_PIXEL_HEIGHT/2;
	isDead = 0;
}



void GAME_EventHandler1(){
	if(STM_EVAL_PBGetState(BUTTON_USER)){
		playerGo = 0;

		while(STM_EVAL_PBGetState(BUTTON_USER));

		playerGo = 1;
	}
}

void GAME_EventHandler2(){
	if(isDead == 0){
		wallMove = 1;
	}
}

void GAME_EventHandler3(){
	if(isDead == 1){
		BallReset();
	}
}
void GAME_EventHandler4(){
	count++;
	if(count == 10000){
		change = 1;
		count = 0;
	}

}
static char* itoa(int value, char* result, int base)
{
	if (base < 2 || base > 36) {
		*result = '\0';
		return result;
	}
	char *ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while (value);

	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}
void GAME_Update(){
	
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_DrawFullRect(ballX,ballY,ballSize,ballSize);
	
	if(playerGo)
		ballX -= 0;
	else
		ballX += 5;
	if(ballX < 0)
		ballX = 0;
	else if((ballX + ballSize) >= LCD_PIXEL_WIDTH){
		score += 1;
		ballX = LCD_PIXEL_WIDTH - ballX;
		LCD_ClearLine(LCD_LINE_1);
	}
	int i;
	for(i = 0 ; i < 8 ;i++){
		if((ballX+ballSize >= posX[i]) && (ballX< (posX[i]+wallW)) && (ballY - posY[i] < wallL) && (ballY - posY[i] > 0)){
			score -= 1;
			BallReset();
			LCD_ClearLine(LCD_LINE_1);
		}
	}
	if(wallMove == 1){
		int i;
		int mul;
		int speed = 1;
		for(i = 0 ;i < 8; i++){
			if(change){
				speed = speed * (-1);
				LCD_DrawFullRect(posX[i],posY[i],wallW,wallL);
				posY[i] += speed;
				if(posY[i] >= LCD_PIXEL_HEIGHT){
					posY[i] = 0;
				}
				else if(posY[i] <= 0){
					posY[i] = LCD_PIXEL_HEIGHT;
				}
			}
			else{
				LCD_DrawFullRect(posX[i],posY[i],wallW,wallL);
				posY[i] += speed;
				if(posY[i] >= LCD_PIXEL_HEIGHT)
					posY[i] = 0;
			}
		}
		change = 0;
	}
}


void GAME_Render(){
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawFullRect(ballX,ballY,ballSize,ballSize);
	int i;
	for(i = 0 ;i < 8;i++){
		LCD_DrawFullRect(posX[i],posY[i],wallW,wallL);
	}
	char str[16] = "Score: ";
	itoa(score,str+7,10);
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_SetBackColor(LCD_COLOR_BLACK);
	LCD_DisplayStringLine(LCD_LINE_1,str);

}
