#include <graphics.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <process.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define start_size   4
#define start_speed  100
#define ESC_button   27
#define upward       80
#define downward     72
#define left         75
#define right        77
#define snake_size   8   					//radius of every circle
#define dis          (snake_size * 2)         //space between circles
#define food_size    6

typedef struct{
	long long int x, y;
}cord;

long long int length_body, ESC, gameover, Score, speed, dir;
cord snake[500], apple;
char SCORE[40] = "0";

void generate_food() {
	apple.x = (rand() % (getmaxx() - 20)), apple.y = (rand() % (getmaxy() - 20));
	if (apple.x < 20)
		apple.x += 20;
	if (apple.y <= 40)
		apple.y += 40;
	apple.x += (dis - (apple.x % dis));
	apple.y += (dis - (apple.y % dis));
}

void update_food() {

	setcolor (BLACK);
	setfillstyle(SOLID_FILL, BLACK);
	circle(apple.x, apple.y, food_size);
	floodfill(apple.x, apple.y, BLACK);
	generate_food();
	setcolor (RED);
	setfillstyle(SOLID_FILL, RED);
	circle(apple.x, apple.y, food_size);
	floodfill(apple.x, apple.y, RED);
}

void score() {
	if (abs(snake[0].x - apple.x) <= (food_size * 2)
			&& abs(snake[0].y - apple.y) <= (food_size * 2)) {
		length_body++;
		Score++;
		update_food();
		if (Score % 3 == 0)
			speed -= 10;
		setcolor (RED);
		setfillstyle(SOLID_FILL, BLACK);
		rectangle(440, 4, 600, 20);
		floodfill(500, 10, RED);
		setcolor (BLACK);
		rectangle(440, 4, 600, 20);
		setcolor (BLUE);
		sprintf(SCORE, " %d", Score);
		outtextxy(440, 4, SCORE);
	}
}

void border_bulding() {
	int i;
	setcolor (BLUE);
	for ( i = 0; i < 2; i++) {
		line(i, 0, i, getmaxy());
		line(0, i + 21, getmaxx(), i + 21);
		line(0, i, getmaxx(), i);
		line(getmaxx(), getmaxy() - i, 0, getmaxy() - i);
		line(getmaxx() - i, getmaxy(), getmaxx() - i, 0);
	}
}

void display_body() {
	int i;
	setcolor (GREEN);
	setfillstyle(SOLID_FILL, GREEN);
	for (i = length_body - 1; i >= 0; i--) {
		circle(snake[i].x, snake[i].y, snake_size);
		floodfill(snake[i].x, snake[i].y, GREEN);
	}
}

void begin() {
	int j = 0;
	int i;
	speed = start_speed;
	Score = 0;
	gameover = 0;
	ESC = 0;
	dir = right;
	apple.x = 100;
	apple.y = 100;
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
	setcolor (BLUE);
	outtextxy(40, 230, "Press any key to play");
	getch();
	getch();

	setbkcolor (BLACK);
	cleardevice();
	length_body = start_size;

	for ( i = length_body; i >= 0; i--, j++) {
		snake[i].x = 20 + ( dis * j);
		snake[i].y = 240;
	}

	border_bulding();
	update_food();
	display_body();

	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	setcolor(BLUE);
	outtextxy(350, 5, "SCORE: 0 ");
}

void shift_move() {
	int i;
	for (i = length_body; i > 0; i--) {
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}
}

void update_screen() {
	setcolor (GREEN);
	setfillstyle(SOLID_FILL, GREEN);

	circle(snake[0].x, snake[0].y, snake_size);
	floodfill(snake[0].x, snake[0].y, GREEN);

	setcolor (BLACK);
	setfillstyle(SOLID_FILL, BLACK);

	circle(snake[length_body].x, snake[length_body].y, snake_size); //remove last circle in  the snake
	floodfill(snake[length_body].x, snake[length_body].y, BLACK);

	setcolor(GREEN);
	setfillstyle(SOLID_FILL, GREEN);

	circle(snake[length_body - 1].x, snake[length_body - 1].y, snake_size);
	floodfill(snake[length_body - 1].x, snake[length_body - 1].y, GREEN);
}

void play() {
	char c = '0';
	int j;
	update_screen();
	shift_move();
	delay(speed);
	while (kbhit())
		c = getch();
	if (c != '0') {
		if (c == ESC_button)
			ESC = 1;

		else if ((c == left && dir == right)
		|| (c == right && dir == left)
				|| (c == upward && dir == downward)
				|| (c == downward && dir == upward))
			;
		else if (c == upward)
			dir = upward;
		else if (c == left)
			dir = left;
		else if (c == right)
			dir = right;
		else if (c == downward)
			dir = downward;
	}

	if (dir == upward)
		snake[0].y += dis;
	else if (dir == downward)
		snake[0].y -= dis;
	else if (dir == left)
		snake[0].x -= dis;
	else if (dir == right)
		snake[0].x += dis;

	for (j = length_body; j > 0; j--) {
		if (snake[0].x == snake[j].x && snake[0].y == snake[j].y)
			gameover = 1;
		if (abs(snake[j].x - apple.x) < (snake_size + food_size)
				&& abs(snake[j].y - apple.y) < (snake_size + food_size)) {
			update_food();
			setcolor (WHITE);
			setfillstyle(SOLID_FILL, GREEN);
			circle(snake[j].x, snake[j].y, snake_size);
			floodfill(snake[j].x, snake[j].y,WHITE);
			setcolor (GREEN);
			circle(snake[j].x, snake[j].y, snake_size);
		}
	}

	if (snake[0].x >= (getmaxx() - snake_size)
			|| snake[0].y >= (getmaxy() - snake_size)
			|| snake[0].x <= (snake_size) || snake[0].y <= (21 + snake_size))
		gameover = 1;

	score();
}

int main() {
    int big;
	big = initwindow(1900,1000, "Big");
   setcurrentwindow(big);

	srand(time( NULL));

	begin();

	while (1) {
		play();
		if (gameover) {
			cleardevice();
			settextstyle(DEFAULT_FONT, HORIZ_DIR, 7);
			setcolor (RED);
			outtextxy(1900/2-200, 500, "GAME  OVER");
			delay(1000);
			break;
		} else if (ESC) {
			cleardevice();
			settextstyle(DEFAULT_FONT, HORIZ_DIR, 7);
			setcolor (GREEN);
			outtextxy(70, 200, "EXIT GAME");
			delay(1000);
			break;
		}
	}
	closegraph();
	return 0;
}
