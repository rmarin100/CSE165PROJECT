#include <GL/freeglut.h>
#include <string>
#include <iostream>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//=================================================================================================
// CALLBACKS
//=================================================================================================

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

int life = 1;
float mouseX = 0.0f;
int windowWidth = 1400;
int windowHeight = 700;
float ballCenterX = windowWidth - 60;
float ballCenterY = windowHeight / 2.0;
float ballVelocityX = -0.25f; // Initial velocity
float ballVelocityY = 0.009f; // Initial velocity
float gravity = 0.00035f; // Gravity effect on the ball
float ballRadius = 15.0f;

void idle_func()
{
	//uncomment below to repeatedly draw new frames
	glutPostRedisplay();
}

void reshape_func(int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, -1.0, 1.0); // Set orthographic projection
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard_func(unsigned char key, int x, int y)
{
	switch (key)
	{
		// Exit on escape key press
	case '\x1B':
	{
		exit(EXIT_SUCCESS);
		break;
	}
	}
}

void passive_motion_func(int x, int y)
{
	mouseX = x;
}



//=================================================================================================
// RENDERING
//=================================================================================================

void display_func(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//RECTANGLE
	float width = 85.0f; // Adjust the width in pixels
	float height = 20.0f; // Adjust the height in pixels
	float fixedY = (9.0f * windowHeight) / 10.0f;

	glColor3f(1.0f, 1.0f, 1.0f); // Set color to white
	glBegin(GL_QUADS);
	glVertex2f(mouseX - (width / 2.0f), fixedY - (height / 2.0f));
	glVertex2f(mouseX + (width / 2.0f), fixedY - (height / 2.0f));
	glVertex2f(mouseX + (width / 2.0f), fixedY + (height / 2.0f));
	glVertex2f(mouseX - (width / 2.0f), fixedY + (height / 2.0f));
	glEnd();

	//BALL
	int numSegments = 50; // Number of segments to approximate the circle
	// Update ball position
	ballCenterX += ballVelocityX;
	ballCenterY += ballVelocityY;
	ballVelocityY += gravity;


	if (ballCenterX + ballRadius > mouseX - (width / 2.0f) && ballCenterX - ballRadius < mouseX + (width / 2.0f) &&
		ballCenterY + ballRadius > fixedY - (height / 2.0f) && ballCenterY - ballRadius < fixedY + (height / 2.0f)) {
		ballVelocityY = -fabs(ballVelocityY); // Bounce
	}

	// Collision with window borders
	if (ballCenterX - ballRadius < 0 || ballCenterX + ballRadius > windowWidth) {
		ballVelocityX = -ballVelocityX; // Bounce horizontally
	}

	if (ballCenterY + ballRadius > windowHeight) {
		ballVelocityY = -fabs(ballVelocityY); // Bounce at top
	}

	if (ballCenterY >= fixedY - (height / 2.0f) + 60.0f) {  // Check if the ball is below the rectangle
		life--;
		std::cout << "Lives left: " << life << std::endl;
		if (life <= 0) {
			std::cout << "Game Over!" << std::endl;
			exit(0); // Exit or handle game over differently
		}
	}

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(ballCenterX, ballCenterY);
	for (int i = 0; i <= 100; i++) {
		float theta = 2.0f * M_PI * float(i) / float(100);
		float x = ballRadius * cosf(theta);
		float y = ballRadius * sinf(theta);
		glVertex2f(x + ballCenterX, y + ballCenterY);
	}
	glEnd();

	std::string livesText = "Lives: " + std::to_string(life);
	glColor3f(1.0f, 1.0f, 1.0f); // White color for text
	glRasterPos2f(10, 20); // Position for lives text, adjust as needed
	for (char c : livesText) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
	}

	glutSwapBuffers();
}

//=================================================================================================
// INIT
//=================================================================================================

void init(void)
{
	// Print some info
	std::cout << "Vendor:         " << glGetString(GL_VENDOR) << "\n";
	std::cout << "Renderer:       " << glGetString(GL_RENDERER) << "\n";
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << "\n\n";

	// Set the background color
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	std::cout << "Finished initializing...\n\n";
}

//=================================================================================================
// MAIN
//=================================================================================================

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutCreateWindow("Solo Pong");

	glutDisplayFunc(display_func);
	glutIdleFunc(idle_func);
	glutReshapeFunc(reshape_func);
	glutKeyboardFunc(keyboard_func);
	glutPassiveMotionFunc(passive_motion_func);

	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
