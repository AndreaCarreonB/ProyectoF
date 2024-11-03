#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	GLint getmuevex() { return muevex; }
	GLint getmuevey() { return muevey; }
	GLint getmuevez() { return muevez; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }

	//dados
	GLboolean getAnimacionDado() { return tirar; }
	GLboolean tirar = false;

	//camaras
	GLfloat getPosicionZ() { return PosicionZ; }
	GLfloat getPosicionX() { return PosicionX; }

	GLfloat getPosCamaraZ() {
		switch (static_cast<int>(angulo)) {
		case 0:
			return PosicionZ - 12.0f;
		case 90:
			return PosicionZ;
		case 180:
			return PosicionZ + 12.0f;
		default:
			return PosicionZ;
		}
	}
	GLfloat getPosCamaraX() {
		switch (static_cast<int>(angulo)) {
		case 0:
			return PosicionX;
		case 90:
			return PosicionX - 12.0f;
		case 180:
			return PosicionX;
		default:
			return PosicionX + 12.0f;
		}
	}

	GLfloat getangulo() { return angulo; }
	GLint getCamaraVista() { return CamaraVista; }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	GLint muevex;
	GLint muevey;
	GLint muevez;
	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

	//////CAMARA//////
	GLint CamaraVista = 1;
	GLfloat PosicionZ = -290.0f;
	GLfloat PosicionX = 0.0f;
	GLfloat angulo = 0.0f;
	//////CAMARA fin//////
};

