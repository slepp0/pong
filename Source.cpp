#include <iostream>
#include "sl.h"
#include <random>
#include <time.h>
#include <sstream>
#include <vector>w
#define WINDOW_W 1200
#define WINDOW_H 600

// GLOBAL
bool pause = false;

struct specs
{
	int width = 30, height = 150, radius = 30;

	int x = width / 2, 
		y = WINDOW_H / 2;

	int front = (x + width), 
		back = (x - width),
		up = (y + height), 
		down = (y - height);
	int speed = 21;
	bool myTurn;
};

class Player : public specs
{
public:
	float R = 1, G = 1, B = 1;
	static int amountOfPlayer;
	void move(char up, char down)
	{
		if (!pause)
		{
			if (slGetKey(up))
				if(y+(height/2)<WINDOW_H)
					y += speed;
			if (slGetKey(down))
				if(y-(height/2)>0)
					y -= speed;
		}
	}
	void render()
	{
		up = (y + height),
		down = (y - height);
		slPush();
			slSetForeColor(R, G, B, 1);
			slRectangleFill(x, y, width, height);
		slPop();
	}
	Player()
	{
		amountOfPlayer++;
		if (amountOfPlayer == 2)
			x = (WINDOW_W - (width / 2));
	}
};

int Player::amountOfPlayer;

class Ball
{
private:
	int bX = (WINDOW_W / 2), bY = (WINDOW_H / 2), bRadius = 30,
		bSpeedX = 6,
		bSpeedY = 6,
		offset = (bRadius / 2);
	float R = 1, G = 1, B = 1;
	void hitted()
	{
		static int soundPaddleHitted = slLoadWAV("sound/hitted.wav");
		if (slSoundPlaying(soundPaddleHitted))
			slSoundStop(soundPaddleHitted);
		else
			slSoundPlay(slLoadWAV("sound/hitted.wav"));
	}
	void goal()
	{
		static int soundGoal = slLoadWAV("sound/goal.wav");
		if (slSoundPlaying(soundGoal))
			slSoundStop(soundGoal);
		else
			slSoundPlay(soundGoal);
	}
	int score1 = 0;
	int score2 = 0;
	std::stringstream streamScore ; 
public:
	void displayScore()
	{
		slSetForeColor(1, 1, 1, 1);
		streamScore << score1 << " - " << score2;
		slText(WINDOW_W / 2, WINDOW_H / 2, streamScore.str().c_str());
		streamScore.str("");
	}
	void restartBall()
	{
		bX = (WINDOW_W / 2);
		bY = (WINDOW_H / 2);
		bSpeedX = bSpeedY = 6;
	}

	void renderBall(Player* paddle1, Player* paddle2)
	{	
		bX += bSpeedX;
		bY += bSpeedY;
		// P1 
		if (bX < paddle1->front + offset && bY >= paddle1->down && bY <= paddle1->up)
		{	
			hitted();
			paddle1->R = 0;
			bSpeedY++;
			bSpeedX = -bSpeedX++;
		}
		else
		{
			paddle1->R += 0.01;
		}
		// P2
		if (bX + bRadius >= WINDOW_W - 30  && bY >= paddle2->down && bY <= paddle2->up)
		{
			hitted();
			paddle2->R = 0;
			bSpeedX++;
			bSpeedY++;
			bSpeedX = -bSpeedX;
		}
		else
		{
			paddle2->R += 0.01;
		}
		if (Player::amountOfPlayer == 2) {
			// right
			if (bX > WINDOW_W - offset)
			{
				goal();
				paddle2->R = 1;
				paddle2->G = paddle2->B = 0;
				score1++;
				bSpeedX = -bSpeedX;
				restartBall();
			}
			else
			{
				paddle2->G = paddle2->B += 0.01;
			}
		} 
		else
		{

		}
		// left
		if (bX < offset)
		{	
			goal();
			paddle1->R = 1;
			paddle1->G = paddle1->B = 0;
			score2++;
			bSpeedX = -bSpeedX;
			restartBall();
		}
		else
		{
			paddle1->G = paddle1->B += 0.01;
		}
		if (bY > WINDOW_H - offset || bY < offset )
		{
			R = 0;
			G = 1;
			B = 0.3;
			bSpeedY = -bSpeedY;
		}
			slPush();
			slSetForeColor(R, G, B, 1);
			slCircleFill(bX, bY, bRadius, 4);
			slPop();
	}
};
int main()
{
	Ball ball;
	Player p1, p2;
	std::cout << p1.front << " " << p1.up;
	slWindow(WINDOW_W, WINDOW_H, "PONG", false);
	slSetTextAlign(SL_ALIGN_CENTER);
	slSetFont(slLoadFont("font/f1.ttf"), 100);
	while (!slShouldClose())
	{
		ball.displayScore();
		ball.renderBall(&p1, &p2);
		///////////////
		p1.move('W', 'S');
		p1.render();
		////////////
		p2.move('O', 'L');
		p2.render();
		slRender();
	}
}
