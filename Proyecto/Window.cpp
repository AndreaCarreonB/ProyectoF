#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 0;
	muevey = 12;
	muevez = 0;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	//Inicializaci�n de GLFW
	if (!glfwInit())
	{
		printf("Fall� inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final: Monopoly Go", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tama�o de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Fall� inicializaci�n de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se est� usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	//camaras
	//LETRA PARA CAMBIAR DE CAMARA
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		theWindow->CamaraVista = (theWindow->CamaraVista + 1) % 3;
	}


	//FLECHAS
	if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
		GLfloat delta = (key == GLFW_KEY_UP) ? 1.0f : -1.0f;
		switch (static_cast<int>(theWindow->angulo)) {
		case 0:
			theWindow->PosicionZ += delta;
			break;
		case 90:
			theWindow->PosicionX += delta;
			break;
		case 180:
			theWindow->PosicionZ -= delta;
			break;
		case 270:
			theWindow->PosicionX -= delta;
			break;
		}
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		theWindow->angulo = (theWindow->angulo < 270.0f) ? theWindow->angulo + 90.0f : 0.0f;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		theWindow->angulo = (theWindow->angulo > 0.0f) ? theWindow->angulo - 90.0f : 270.0f;
	}

	//dados
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		theWindow->tirar = true;
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		theWindow->tirar = false;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		theWindow-> muevex -= 1;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		theWindow-> muevex += 1;
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		theWindow->muevey -= 1;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		theWindow->muevey += 1;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		theWindow->muevez -= 1;
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		theWindow->muevez += 1;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
