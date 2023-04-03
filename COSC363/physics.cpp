#include "objects.h"
#include <iostream>
#include <fstream>
#include <climits>
#include <list>
#include <vector>
#include <stdlib.h>
#include <math.h> 
#include "physics.h"
using namespace std;




bool collided(ball* ball1, ball* ball2)
{
	float distance = sqrt(((ball2->ballX - ball1->ballX) * (ball2->ballX - ball1->ballX) + ((ball2->ballY - ball1->ballY) * (ball2->ballY - ball1->ballY))));
	return distance <= 0.3;
}

void simulateCollision(ball* ball1, ball* ball2)
{
	float distance = sqrt(((ball2->ballX - ball1->ballX) * (ball2->ballX - ball1->ballX) + ((ball2->ballY - ball1->ballY) * (ball2->ballY - ball1->ballY))));
	float M1 = ball1->ballM;
	float M2 = ball2->ballM;
	ball1->ballM = M2 * ball1->energyLoss;
	ball2->ballM = M1 * ball2->energyLoss;
	if (distance < 0.3) {
		while(distance < 0.3) {
			ball2->ballAngle -= 0.05;
			ball1->ballAngle += 0.05;
			ball1->update();
			ball2->update();
			distance = sqrt(((ball2->ballX - ball1->ballX) * (ball2->ballX - ball1->ballX) + ((ball2->ballY - ball1->ballY) * (ball2->ballY - ball1->ballY))));
		}
	}
	
}

void checkCollisions(ball* ball1, ball* ball2, ball* ball3, ball* ball4, ball* ball5)
{
	if (abs(ball1->ballM) > 0) {	
		if (collided(ball1, ball2)) {
			// collision
			simulateCollision(ball1, ball2);
		}
	}
	if (abs(ball2->ballM) > 0) {
		if (collided(ball2, ball3)) {
			// collision
			simulateCollision(ball2, ball3);
		}
	}
	if (abs(ball3->ballM) > 0) {
		if (collided(ball3, ball4)) {
			// collision
			simulateCollision(ball3, ball4);
		}
	}
	if (abs(ball4->ballM) > 0) {
		if (collided(ball4, ball5)) {
			// collision
			simulateCollision(ball4, ball5);
		}
	}
}