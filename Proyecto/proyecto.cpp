#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

#include "Casilla.h"
#include "Personaje.h"

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera, cameraAerea, cameraPersonaje;

Texture pisoTexture;
Texture centroTexture;

Skybox skybox;

Model luminaria;
Model farola;

//MODELOS ANIMADOS EXTRAS MARIO
Model pista;
Model banzai;
Model casa;
Model coin;
Model ExtraSimiFino_Cuerpo;
Model ExtraSimiFino_Piernas;
Model ExtraSimiColorado_Cuerpo;
Model ExtraSimiColorado_Brazo;
Model ExtraSimiAlucin_Cuerpo;
Model ExtraSimiAlucin_Cabeza;

float movSimiPrincipal;
float movSimiPrincipalOffset;
bool avanzaSimiPrincipal;
bool estadoSimiPrincipal;

//dados
Texture dadoTexture8;
Model dado4;
bool tirar;
float altura_8, altura_4, alturaOffset;
float rotX2_8, rotX2_4, rotX2Offset;
float rotY2_8, rotY2_4, rotY2Offset;
float rotX_8, rotX_4, rotY_8, rotY_4;
int cara_random_8, cara_random_4;
int numDado = 0;

//materiales
Material Material_brillante;
Material Material_opaco;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
PointLight pointLights2[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
SpotLight spotLights2[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

void CreateObjects()
{

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj1);

	unsigned int tableroIndices[] = {
		8, 9, 10,
		11, 10, 9,

		4, 5, 6,
		7, 6, 5,

		0, 2, 4,
		2, 4, 6,

		0, 1, 4,
		1, 4, 5,

		1, 3, 5,
		3, 5, 7,

		2, 3, 6,
		3, 6, 7
	};

	GLfloat tableroVertices[] = {
		//sin textura
		-1.0f,	1.0f,	-1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	1.0f,	-1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-1.0f,	1.0f,	1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	1.0f,	1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,

		-1.0f,	0.0f,	-1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	0.0f,	-1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		-1.0f,	0.0f,	1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	0.0f,	1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,

		//con textura
		-1.0f,	1.0f,	-1.0f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	1.0f,	-1.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-1.0f,	1.0f,	1.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		1.0f,	1.0f,	1.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
	};

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(tableroVertices, tableroIndices, 96, 36);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//dados
int caraRandom_8() {
	srand(time(NULL));
	int cara = rand();
	return (cara % 8) + 1;
}

int caraRandom_4() {
	srand(time(NULL));
	int cara = rand();
	return (cara % 4) + 1;
}

void crearDado_8()
{
	unsigned int dado8_indices[] =
	{
		// front up
		0, 1, 2,

		// right up
		3, 4, 5,

		// back up
		6, 7, 8,

		// left up
		9, 10, 11,

		// front down
		12, 13, 14,

		// right down
		15, 16, 17,

		// back down
		18, 19, 20,

		// left down
		21, 22, 23
	};

	GLfloat dado8_vertices[] =
	{
		// front up 6
		//x		y		z		S		T			NX		NY		NZ
		0.0f,-2.0f, 0.0f,	0.66f,	0.4f,		0.0f,	1.0f,	-1.0f,	//0 A
		2.0f, 0.0f, 0.0f,	0.66f,	0.8f,		0.0f,	1.0f,	-1.0f,	//1 C
		0.0f, 0.0f, 2.0f,	0.33f,	0.6f,		0.0f,	1.0f,	-1.0f,	//2 F

		// right up 7
		//x		y		z		S		T
		0.0f, 2.0f, 0.0f,	0.66f,   0.4f,		-1.0f,	0.0f,	0.0f, //3 F
		0.0f, 0.0f, 2.0f,	0.66f,	0.8f,		-1.0f,	0.0f,	0.0f, //4 C
		-2.0f, 0.0f, 0.0f,	1.0f,	0.6f,		-1.0f,	0.0f,	0.0f, //5 E

		// back up 8
		-2.0f, 0.0f, 0.0f,	1.0f,  1.0f,		0.0f,	-1.0f,	-1.0f, //6 E
		0.0f,-2.0f, 0.0f,	0.66f,	0.8f,		0.0f,	-1.0f,	-1.0f, //7 C
		0.0f, 0.0f,-2.0f,	1.0f,	0.6f,		0.0f,	-1.0f,	-1.0f, //8 B

		// left up 5
		//x		y		z		S		T
		2.0f, 0.0f, 0.0f,	0.34f,  0.61f,		1.0f,	0.0f,	0.0f, //9 B
		0.0f, 2.0f, 0.0f,	0.66f,	0.8f,		1.0f,	0.0f,	0.0f, //10 C
		0.0f, 0.0f, 2.0f,	0.34f,	0.99f,		1.0f,	0.0f,	0.0f, //11 A

		// front down 3
		//x		y		z		S		T			NX		NY		NZ
		0.0f, 2.0f, 0.0f,	0.33f,	0.6f,		0.0f,	1.0f,	-1.0f,	//12 A
		-2.0f, 0.0f, 0.0f,	0.33f,	0.2f,		0.0f,	1.0f,	-1.0f,	//13 D
		0.0f, 0.0f,-2.0f,	0.66f,	0.4,		0.0f,	1.0f,	-1.0f,	//14 F

		// right down 4
		//x		y		z		S		T
		0.0f,-2.0f, 0.0f,	0.66f,  0.4f,		-1.0f,	0.0f,	0.0f, //15 F
		2.0f, 0.0f, 0.0f,	0.33f,	0.2f,		-1.0f,	0.0f,	0.0f, //16 D
		0.0f, 0.0f,-2.0f,	0.66f,	0.0f,		-1.0f,	0.0f,	0.0f, //17 E

		// back down 1
		//x		y		z		S		T
		-2.0f, 0.0f, 0.0f,	0.0f,  0.0f,		0.0f,	-1.0f,	1.0f, //18 E
		0.0f,-2.0f, 0.0f,	0.33f,	0.2f,		0.0f,	-1.0f,	1.0f, //19 D
		0.0f, 0.0f, 2.0f,	0.0f,	0.4f,		0.0f,	-1.0f,	1.0f, //20 B

		// left down 2
		//x		y		z		S		T
		2.0f, 0.0f, 0.0f,	0.31f,  0.58f,		1.0f,	0.0f,	0.0f, //21 B
		0.0f, 2.0f, 0.0f,	0.31f,  0.22f,		1.0f,	0.0f,	0.0f, //22 D
		0.0f, 0.0f,-2.0f,	0.04f,  0.38f,		1.0f,	0.0f,	0.0f, //23 A
	};


	Mesh* dado = new Mesh();
	dado->CreateMesh(dado8_vertices, dado8_indices, 192, 24);
	meshList.push_back(dado);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	//camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);
	Camera camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 45.0f, 5.0f, 0.1f);
	cameraAerea = Camera(glm::vec3(0.0f, 185.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, -90.0f, 0.6f, 0.5f);
	//base
	{
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	centroTexture = Texture("Textures/casillas/centro.png");
	centroTexture.LoadTextureA();
	}

	//skybox
	bool cicloDia = true;
	float ultimocambio = 0.0f;
	const float tiempocambio = 10.0f;

	std::vector<std::string> Dia_Skybox = {
	"Textures/Skybox/skybox_rt.tga",
	"Textures/Skybox/skybox_lf.tga",
	"Textures/Skybox/skybox_dn.tga",
	"Textures/Skybox/skybox_up.tga",
	"Textures/Skybox/skybox_bk.tga",
	"Textures/Skybox/skybox_ft.tga"
	};

	std::vector<std::string> Noche_Skybox = {
		"Textures/Skybox/skyboxn_rt.tga",
		"Textures/Skybox/skyboxn_lf.tga",
		"Textures/Skybox/skyboxn_dn.tga",
		"Textures/Skybox/skyboxn_up.tga",
		"Textures/Skybox/skyboxn_bk.tga",
		"Textures/Skybox/skyboxn_ft.tga"
	};

	skybox = Skybox(Dia_Skybox);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.7f, 1.0f,
		0.0f, 0.0f, -1.0f);

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int pointLightCount2 = 0;
	//contador de luces spot
	unsigned int spotLightCount = 0;
	unsigned int spotLightCount2 = 0;

	//Luz ligada al personaje
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f, // luz blanca
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, // posicion
		1.0f, 0.09f, 0.032f);
	pointLightCount++;

	pointLights2[0] = PointLight(1.0f, 1.0f, 0.0f, // luz blanca
		1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, // posicion
		1.0f, 0.09f, 0.032f);
	pointLightCount2++;

	//Luces de las luminarias
	spotLights[0] = SpotLight(0.0f, 1.0f, 1.0f, //luz cian
		1.0f, 2.0f,
		45.0f, 10.0f, 45.0f, // posicion
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		40.0f); // angulo de apertura
	spotLightCount++;

	spotLights2[0] = SpotLight(0.0f, 1.0f, 1.0f, //luz cian
		1.0f, 2.0f,
		-45.0f, 10.0f, -45.0f, // posicion
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		40.0f); // angulo de apertura
	spotLightCount2++;

	pointLights[1] = PointLight(1.0f, 1.0f, 0.0f, // luz amarilla
		7.0f, 1.0f,
		45.0f, 20.0f, -45.0f, // posicion
		1.0f, 0.09f, 0.032f);
	pointLightCount++;

	pointLights2[1] = PointLight(1.0f, 1.0f, 0.0f, // luz amarilla
		7.0f, 1.0f,
		-45.0f, 20.0f, 45.0f, // posicion
		1.0f, 0.09f, 0.032f);
	pointLightCount2++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana

	//Casillas
	Casilla casilla[41];
	float modeloOffsetY = 16.5f;
	float modeloOffsetZ = 12.5f;
	for (int i = 0; i <= 40; i++) {
		casilla[i].iniCasillas(i);
		casilla[i].iniTexturas(i);
		casilla[i].iniModelos(i);
		//posicion de modelos iniciales
		for (int p = 0; p < 3; p++) switch (p) {
			case 0: casilla[i].modeloPosicion[0] = casilla[i].casillaPosicion[0]; break;
			case 1: casilla[i].modeloPosicion[1] = casilla[i].casillaPosicion[1] - modeloOffsetY; break;
			case 2: casilla[i].modeloPosicion[2] = casilla[i].casillaPosicion[2]; break;
		}
	}

	/*dados ini*/ {
		crearDado_8();

		dadoTexture8 = Texture("Textures/dado_8_caras_optimizado.jpeg");
		dadoTexture8.LoadTextureA();

		dado4 = Model();
		dado4.LoadModel("Models/dado4.obj");

		cara_random_8 = 0;
		cara_random_4 = 0;

		altura_8 = 20.0f;
		altura_4 = 20.0f;
		alturaOffset = 0.6;

		rotX2_8 = 0.0f;
		rotX2_4 = 0.0f;
		rotX2Offset = 3.5f;

		rotY2_8 = 0.0f;
		rotY2_4 = 0.0f;
		rotY2Offset = 3.5f;

		tirar = false;
	}

	//personajes
	Personaje personaje[3];
	int personajeActual = 0;
	bool personajeSig = true;
	float vector[3] = { 35.0f, 0.1f, 70.0f };
	for (int i = 0; i < 3; i++) {
		personaje[i].iniModelos(i);
		//posicion inicial
		personaje[i].casillaActual = 0;
		personaje[i].casillaSiguiente = 0;
		//posicion XYZ
		for (int p = 0; p < 3; p++) switch (i) {
			case 0: {
				switch (p) {
				case 0: personaje[0].posicion[p] = -vector[p]; break;
				case 1: personaje[0].posicion[p] = vector[p]; break;
				case 2: personaje[0].posicion[p] = vector[p]; break;
				}
				break;
			}
			case 1: {
				switch (p) {
				case 0: personaje[1].posicion[p] = 0.0f; break;
				case 1: personaje[1].posicion[p] = vector[p]; break;
				case 2: personaje[1].posicion[p] = vector[p]; break;
				}
				break;
			}
			case 2: {
				switch (p) {
				case 0: personaje[2].posicion[p] = vector[p]; break;
				case 1: personaje[2].posicion[p] = vector[p]; break;
				case 2: personaje[2].posicion[p] = vector[p]; break;
				}
				break;
			}
		}
		//angulos iniciales
		for (int a = 0; a < 5; a++) personaje[i].angulos[a] = 0.0f;
	}

	//animaciones
	float modeloSubeVelocidad = 0.8f;
	float modeloGiraVelocidad = 4.0f;
	float personajeMovVelocidad = 0.4f;

	float movSimiFino;
	float movSimiFinoOffset;
	float movSimiColorado;
	float movSimiColoradoOffset;
	float movSimiAlucin;
	float movSimiAlucinOffset;

	bool estadoSimiFino;
	bool estadoSimiColorado;
	bool estadoSimiAlucin;

	//Animaci�n del Simi Fino
	movSimiFino = 0.0f;
	movSimiFinoOffset = 1.0;
	estadoSimiFino = true;

	//Animaci�n del Simi Colorado
	movSimiColorado = 0.0f;
	movSimiColoradoOffset = 5.0;
	estadoSimiColorado = true;

	//Animaci�n del Simi Aluc�n
	movSimiAlucin = 0.0f;
	movSimiAlucinOffset = 0.5;
	estadoSimiAlucin = true;

	//variables para animaci�n del simi principal
	movSimiPrincipal = 0.0f;
	movSimiPrincipalOffset = 5.0;
	avanzaSimiPrincipal = true;

	//variables para animaci�n de mario
	float movCoinOffset = 1.0f;
	float giraCoin = 0.0f;
	float movBanzai = 0.0f;
	float movBanzaiOf = 0.1f;
	float angulovaria = 0.0f;

	//modelos extra
	luminaria = Model();
	luminaria.LoadModel("Models/street_light.obj");
	farola = Model();
	farola.LoadModel("Models/farola.obj");

	pista = Model();
	pista.LoadModel("Models/Circuit.obj");
	banzai = Model();
	banzai.LoadModel("Models/banzai.obj");
	casa = Model();
	casa.LoadModel("Models/Toad.obj");
	coin = Model();
	coin.LoadModel("Models/coin.obj");
	
	ExtraSimiFino_Cuerpo = Model();
	ExtraSimiFino_Cuerpo.LoadModel("Models/simi_fino_cuerpo.obj");
	ExtraSimiFino_Piernas = Model();
	ExtraSimiFino_Piernas.LoadModel("Models/simi_fino_piernas.obj");

	ExtraSimiColorado_Cuerpo = Model();
	ExtraSimiColorado_Cuerpo.LoadModel("Models/simi_chapulin_cuerpo.obj");
	ExtraSimiColorado_Brazo = Model();
	ExtraSimiColorado_Brazo.LoadModel("Models/simi_chapulin_brazo.obj");

	ExtraSimiAlucin_Cuerpo = Model();
	ExtraSimiAlucin_Cuerpo.LoadModel("Models/simi_alucin_cuerpo.obj");
	ExtraSimiAlucin_Cabeza = Model();
	ExtraSimiAlucin_Cabeza.LoadModel("Models/simi_alucin_cabeza.obj");


	bool asignar = false;

	while (!mainWindow.getShouldClose())
	{
		//ciclo de dia
		float currentTime = glfwGetTime();
		if (currentTime - ultimocambio >= tiempocambio) {
			cicloDia = !cicloDia;
			if (cicloDia) skybox = Skybox(Dia_Skybox);
			else skybox = Skybox(Noche_Skybox);
			ultimocambio = currentTime;
		}

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		cameraPersonaje = Camera(glm::vec3(mainWindow.getPosCamaraX(), 18.0f, mainWindow.getPosCamaraZ() - 6.0f), glm::vec3(0.0f, 1.0f, 0.0f), -mainWindow.getangulo() + 90.0f, 0.0f, 0.3f, 0.5f);

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		cameraAerea.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		cameraAerea.keyControl(mainWindow.getsKeys(), deltaTime);

		if (mainWindow.getCamaraVista() > 0) camera = cameraPersonaje;
		else camera = cameraAerea;

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		/*luces*/
		{
			//informaci�n al shader de fuentes de iluminaci�n
			shaderList[0].SetDirectionalLight(&mainLight);

			if (!cicloDia)
			{
				//Para que la luz ligada al personaje siga a este cuando el tiempo es de noche
				pointLights[0].SetPos(glm::vec3(personaje[personajeActual].posicion[0], personaje[personajeActual].posicion[1], personaje[personajeActual].posicion[2]));
				pointLights2[0].SetPos(glm::vec3(personaje[personajeActual].posicion[0], personaje[personajeActual].posicion[1], personaje[personajeActual].posicion[2]));

				//Para validar la posici�n del personaje y encender las luces correspondientes
				if (personaje[personajeActual].posicion[0] > 35 && personaje[personajeActual].posicion[2] > 35)
				{
					shaderList[0].SetSpotLights(spotLights, spotLightCount);
					shaderList[0].SetPointLights(pointLights, 1);
				}
				else if (personaje[personajeActual].posicion[0] > 35 && personaje[personajeActual].posicion[2] < -35)
				{
					shaderList[0].SetSpotLights(spotLights, 0);
					shaderList[0].SetPointLights(pointLights, pointLightCount);
				}
				else if (personaje[personajeActual].posicion[0] < -35 && personaje[personajeActual].posicion[2] < -35)
				{
					shaderList[0].SetSpotLights(spotLights2, spotLightCount2);
					shaderList[0].SetPointLights(pointLights, 1);
				}
				else if (personaje[personajeActual].posicion[0] < -35 && personaje[personajeActual].posicion[2] > 35)
				{
					shaderList[0].SetSpotLights(spotLights, 0);
					shaderList[0].SetPointLights(pointLights2, pointLightCount2);
				}
				else
				{
					shaderList[0].SetSpotLights(spotLights, 0);
					shaderList[0].SetPointLights(pointLights, 1);
				}
			}
			else
			{
				shaderList[0].SetSpotLights(spotLights, 0);
				shaderList[0].SetPointLights(pointLights, 0);
			}
		}
		
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);

		/*modelos extra*/ {
			angulovaria += 5.0f * deltaTime;
			if (movBanzai < 30.0f) movBanzai -= movBanzaiOf * deltaTime;
			//RENDERIZADO DE LOS MODELOS 
			//CASA
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(80.0f, 1.0f, 20.0f));
			model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			casa.RenderModel();

			//PISTA
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(15.0f, 1.0f, 80.0f));
			model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			pista.RenderModel();

			//COIN
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(movCoinOffset, 5.0f + (1 * sin(glm::radians(angulovaria))), 6.0));
			model = glm::translate(model, glm::vec3(0.0, 0.5f, 6.0));
			model = glm::rotate(model, glm::radians(giraCoin), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			coin.RenderModel();

			//BANZAI
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-50.0f, 11.0f, -2.0f));
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			banzai.RenderModel();

			//Animaci�n del Simi Colorado
			if (movSimiColorado >= -25 && movSimiColorado <= 95)
			{
				if (estadoSimiColorado)
				{
					if (movSimiColorado <= 90)
					{
						movSimiColorado += movSimiColoradoOffset * deltaTime;
					}
					else
					{
						estadoSimiColorado = false;
					}
				}
				else
				{
					if (movSimiColorado >= -20)
					{
						movSimiColorado -= movSimiColoradoOffset * deltaTime;
					}
					else
					{
						estadoSimiColorado = true;
					}
				}
			}
			else
			{
				movSimiColorado = 0;
			}

			//Animaci�n del Simi Aluc�n
			if (movSimiAlucin >= -50 && movSimiAlucin <= 50)
			{
				if (estadoSimiAlucin)
				{
					if (movSimiAlucin <= 45)
					{
						movSimiAlucin += movSimiAlucinOffset * deltaTime;
					}
					else
					{
						estadoSimiAlucin = false;
					}
				}
				else
				{
					if (movSimiAlucin >= -45)
					{
						movSimiAlucin -= movSimiAlucinOffset * deltaTime;
					}
					else
					{
						estadoSimiAlucin = true;
					}
				}
			}
			else
			{
				movSimiAlucin = 0;
			}

			//Simi Fino
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-70.0f, 0.0f, -15.0f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ExtraSimiFino_Piernas.RenderModel();

			model = modelaux;
			model = glm::translate(model, glm::vec3(0.0f, 3.6f, 0.0f));
			model = glm::rotate(model, -movSimiFino * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ExtraSimiFino_Cuerpo.RenderModel();

			//Simi Colorado
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-70.0f, 1.0f, 15.0f));
			modelaux = model;
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ExtraSimiColorado_Cuerpo.RenderModel();

			model = modelaux;
			model = glm::translate(model, glm::vec3(-0.5f, 7.0f, -2.0f));
			model = glm::rotate(model, -movSimiColorado * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ExtraSimiColorado_Brazo.RenderModel();

			//Simi Alucin
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-35.0f, -1.0f, -70.0f));
			modelaux = model;
			model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ExtraSimiAlucin_Cuerpo.RenderModel();

			model = modelaux;
			model = glm::rotate(model, -movSimiAlucin * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			ExtraSimiAlucin_Cabeza.RenderModel();


			//Luces de las esquinas
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(52.0f, 0.0f, 52.0f));
			model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			luminaria.RenderModel();

			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-52.0f, 0.0f, -52.0f));
			model = glm::rotate(model, 315 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			luminaria.RenderModel();

			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(52.0f, 0.0f, -52.0f));
			model = glm::rotate(model, 225 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			farola.RenderModel();

			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(-52.0f, 0.0f, 52.0f));
			model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			farola.RenderModel();
		}

		/*piso*/{
			model = glm::mat4(1.0);
			model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			pisoTexture.UseTexture();
			meshList[0]->RenderMesh();
		}

		/*tablero*/{
			/*centro*/ {
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(0.0f, 0.01f, 0.0f));
				model = glm::scale(model, glm::vec3(50.0f * 0.73f, 1.8f, 50.0f * 0.73f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				centroTexture.UseTexture();
				meshList[1]->RenderMesh();
			}
		
			/*exterior*/ {
				float angulo = 0.0f;
				for (int i = 0; i < 40; i++) {
					if (i == 10 || i == 20 || i == 30) angulo += 90.0f;//que fila o columna del tablero
					/*casillas*/ {
						/*bandera dia, noche, o especial*/ {
							if (casilla[i].especial) casilla[i].textura[2].UseTexture();
							else if (cicloDia) casilla[i].textura[0].UseTexture();
							else casilla[i].textura[1].UseTexture();
						}

						/*dibujado*/ {
							glm::mat4 model(1.0);
							model = glm::translate(model, glm::vec3(
								casilla[i].casillaPosicion[0], casilla[i].casillaPosicion[1], casilla[i].casillaPosicion[2]));
							model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
							model = glm::scale(model, glm::vec3(casilla[i].casillaEscala[0], 1.0f, casilla[i].casillaEscala[1]));
							glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
							meshList[1]->RenderMesh();
						}
					}

					/*modelos*/ {
						/*animacion modelos exteriores*/ {
							if (casilla[i].modeloPosicion[1] > 1.5f) //si se pasan de esos valores, ponlo en su posicion final
								casilla[i].modeloPosicion[1] = casilla[i].casillaPosicion[1]; 
							
							if (casilla[i].modeloSube) {
							if (casilla[i].modeloPosicion[1] < casilla[i].casillaPosicion[1])
									casilla[i].modeloPosicion[1] += modeloSubeVelocidad * deltaTime;
								else if (casilla[i].modeloGira < 2 * 360.0f)
									casilla[i].modeloGira += modeloGiraVelocidad * deltaTime;
							}
							else {
								if (casilla[i].modeloPosicion[1] < casilla[i].casillaPosicion[1] - modeloOffsetY) //si se pasan de esos valores, ponlo en su posicion inicial
									casilla[i].modeloPosicion[1] = casilla[i].casillaPosicion[1] - modeloOffsetY;

								casilla[i].modeloGira = 0.0f;
								if (casilla[i].modeloPosicion[1] > casilla[i].casillaPosicion[1] - modeloOffsetY)
									casilla[i].modeloPosicion[1] -= modeloSubeVelocidad * deltaTime;
							}
						}

						/*renderizado*/ {
							model = glm::mat4(1.0);
							model = glm::translate(model, glm::vec3(
								casilla[i].modeloPosicion[0], casilla[i].modeloPosicion[1], casilla[i].modeloPosicion[2]));
							model = glm::rotate(model, angulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//que fila/columna del tablero
							model = glm::translate(model, glm::vec3(0.0f, 0.0f, modeloOffsetZ));//offset para que quede afuera
							model = glm::rotate(model, casilla[i].modeloAngulo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//rotacion para que vea el tablero
							model = glm::rotate(model, casilla[i].modeloGira * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));//giro de animacion
							model = glm::scale(model, glm::vec3(casilla[i].modeloEscala, casilla[i].modeloEscala, casilla[i].modeloEscala));
							glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
							casilla[i].modelo.RenderModel();
						}
					}
				}
			}
		}

		/*dados*/ {
			/*posiciones 8*/ switch (cara_random_8) {
			case 1: rotX_8 = -307; rotY_8 = 137; break;
			case 2: rotX_8 = -234; rotY_8 = 127; break;
			case 3: rotX_8 = -129; rotY_8 = 135; break;
			case 4: rotX_8 = -309; rotY_8 = 224.5; break;
			case 5: rotX_8 = -129; rotY_8 = 52; break;
			case 6: rotX_8 = -306; rotY_8 = 42; break;
			case 7: rotX_8 = 239; rotY_8 = 44; break;
			case 8: rotX_8 = -308; rotY_8 = -47; break;
			}
			/*posiciones 4*/ switch (cara_random_4) {
			case 1: rotX_4 = 360; rotY_4 = 360; break;
			case 2: rotX_4 = 110; rotY_4 = 330; break;
			case 3: rotX_4 = 110; rotY_4 = -270; break;
			case 4: rotX_4 = 110; rotY_4 = 210; break;
			}

			if (mainWindow.getAnimacionDado()) tirar = true;
			/*animaciones*/ if (tirar) {
				if (asignar) {
					cara_random_8 = caraRandom_8();
					cara_random_4 = caraRandom_4();
					numDado = cara_random_8 + cara_random_4;
					personaje[personajeActual].casillaSiguiente = personaje[personajeActual].casillaActual + numDado;
					personaje[personajeActual].avanza = true;
					asignar = false;
				}
				else tirar = false;

				if (altura_8 > 5.8) altura_8 -= alturaOffset * deltaTime;

				if (rotX2_8 != rotX_8) {
					if (rotX2_8 <= rotX_8) rotX2_8 += rotX2Offset * deltaTime;
					else rotX2_8 -= rotX2Offset * deltaTime;
				}
				if (rotY2_8 != rotY_8) {
					if (rotY2_8 < rotY_8) rotY2_8 += rotY2Offset * deltaTime;
					else rotY2_8 -= rotY2Offset * deltaTime;
				}
				if (altura_4 > 5.2) altura_4 -= alturaOffset * deltaTime;

				if (rotX2_4 != rotX_4) {
					if (rotX2_4 <= rotX_4) rotX2_4 += rotX2Offset * deltaTime;
						
					else rotX2_4 -= rotX2Offset * deltaTime;
				}
				if (rotY2_4 != rotY_4) {
					if (rotY2_4 < rotY_4) rotY2_4 += rotY2Offset * deltaTime;
					else rotY2_4 -= rotY2Offset * deltaTime;
				}
			}
			else {
				rotX2_8 = 0.0f;
				rotY2_8 = 0.0f,
				altura_8 = 20.0f;
				rotX2_4 = 0.0f;
				rotY2_4 = 0.0f,
				altura_4 = 20.0f;
				asignar = true;
			}
			/*renderizado*/ {
				// Dado de 8 caras
				float dadoEscala;
				dadoEscala = 3;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-10.0f, altura_8, 0.0f));
				model = glm::rotate(model, glm::radians(rotX2_8), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotY2_8), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(dadoEscala, dadoEscala, dadoEscala));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				dadoTexture8.UseTexture();
				meshList[2]->RenderMesh();

				// Dado de 4 caras
				dadoEscala = 0.4;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(10.0f, altura_4, 0.0f));
				model = glm::rotate(model, glm::radians(rotX2_4), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotY2_4), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(dadoEscala, dadoEscala, dadoEscala));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				dado4.RenderModel();
			}
		}

		/*personaje*/ {
			if (personajeSig){
				for (int p = 0; p < 3; p++) personaje[personajeActual].posicion[p] = casilla[0].casillaPosicion[p];
				personajeSig = false;
			}
			for (int i = 0; i < 3; i++) {
				/*recorrer el tablero*/ if (personaje[i].avanza) {
					/*movimiento*/ if (personaje[i].casillaActual != personaje[i].casillaSiguiente) {
						//fila abajo 
						if ((personaje[i].casillaActual + 1) > 0 && (personaje[i].casillaActual + 1) <= 10) {
							//aumentar X
							if (personaje[i].posicion[0] < casilla[personaje[i].casillaActual + 1].casillaPosicion[0])
								personaje[i].posicion[0] += personajeMovVelocidad * deltaTime;
							else personaje[i].casillaActual++;
						}
						//columna derecha
						if ((personaje[i].casillaActual + 1) > 10 && (personaje[i].casillaActual + 1) <= 20) {
							//disminuir Z
							if (personaje[i].posicion[2] > casilla[personaje[i].casillaActual + 1].casillaPosicion[2])
								personaje[i].posicion[2] -= personajeMovVelocidad * deltaTime;
							else personaje[i].casillaActual++;
						}
						//fila arriba 
						if ((personaje[i].casillaActual + 1) > 20 && (personaje[i].casillaActual + 1) <= 30) {
							//disminuir X
							if (personaje[i].posicion[0] > casilla[personaje[i].casillaActual + 1].casillaPosicion[0])
								personaje[i].posicion[0] -= personajeMovVelocidad * deltaTime;
							else personaje[i].casillaActual++;
						}
						//columna izquierda
						if ((personaje[i].casillaActual + 1) > 30 && (personaje[i].casillaActual + 1) <= 40) {
							//aumentar Z
							if (personaje[i].posicion[2] < casilla[personaje[i].casillaActual + 1].casillaPosicion[2])
								personaje[i].posicion[2] += personajeMovVelocidad * deltaTime;
							else personaje[i].casillaActual++;
						}
						
						/*animacion dinamica*/ switch (i) {
							case 0: {

								break;
							}
							case 1: {
								if (personaje[1].angulos[1] > 360.0f) personaje[1].angulos[1] = 0.0f;
								personaje[1].angulos[1] += 6.5f * deltaTime;
								break;
							}
							case 2: {
								if (movSimiPrincipal >= -50 && movSimiPrincipal <= 50)
								{
									if (estadoSimiPrincipal)
									{
										if (movSimiPrincipal <= 45)
										{
											movSimiPrincipal += movSimiPrincipalOffset * deltaTime;
										}
										else
										{
											estadoSimiPrincipal = false;
										}
									}
									else
									{
										if (movSimiPrincipal >= -45)
										{
											movSimiPrincipal -= movSimiPrincipalOffset * deltaTime;
										}
										else
										{
											estadoSimiPrincipal = true;
										}
									}
								}
								else
								{
									movSimiPrincipal = 0;
								}
								
								break;
							}
						}
					}
					else personaje[i].avanza = false;
					
					/*cambio de personaje*/ {
						if (personaje[personajeActual].casillaActual == 40) {
							/*validar casillas*/ {
								personaje[i].casillaActual = 0;
								if (personaje[i].casillaSiguiente >= 40) personaje[i].casillaSiguiente = 0;
							}

							//mover al inicio del tablero
							for (int p = 0; p < 3; p++) switch (personajeActual) {
							case 0: {
								switch (p) {
								case 0: personaje[0].posicion[p] = -vector[p]; break;
								case 1: personaje[0].posicion[p] = vector[p]; break;
								case 2: personaje[0].posicion[p] = vector[p]; break;
								}
								break;
							}
							case 1: {
								switch (p) {
								case 0: personaje[1].posicion[p] = 0.0f; break;
								case 1: personaje[1].posicion[p] = vector[p]; break;
								case 2: personaje[1].posicion[p] = vector[p]; break;
								}
								break;
							}
							case 2: {
								switch (p) {
								case 0: personaje[2].posicion[p] = vector[p]; break;
								case 1: personaje[2].posicion[p] = vector[p]; break;
								case 2: personaje[2].posicion[p] = vector[p]; break;
								}
								break;
							}
							}

							//cambiar de personaje
							personajeActual++;
							personajeSig = true;
						}
						if (personajeActual > 2) personajeActual = 0;
					}
				}

				/*animacion estatica*/ else {
					switch (i) {
						case 0: {

							break;
						}
						case 1: {
							if (personaje[1].angulos[0] > 360.0f) personaje[1].angulos[0] = 0.0f;
							personaje[1].angulos[0] += 6.5f * deltaTime;
							personaje[1].posicion[1] = 1.5 * sin(glm::radians(personaje[1].angulos[0]));
							break;
						}
						case 2: {

							break;
						}
					}
				}

				/*cambio de banderas de casilla*/ {
					casilla[personaje[i].casillaActual].especial = true;
					if (personaje[i].casillaActual - 1 < 0) casilla[39].especial = false;
					else casilla[personaje[i].casillaActual - 1].especial = false;

					if (!personaje[i].avanza) casilla[personaje[i].casillaActual].modeloSube = true;
					else casilla[personaje[i].casillaActual].modeloSube = false;
				}

				float giroFinal = 0.0f;
				/*renderizado*/ switch (i) {
					case 0: {
						if (personaje[0].casillaActual >= 0 && personaje[0].casillaActual < 10) giroFinal = 90.0f;
						if (personaje[0].casillaActual >= 10 && personaje[0].casillaActual < 20) giroFinal = 180.0f;
						if (personaje[0].casillaActual >= 20 && personaje[0].casillaActual < 30) giroFinal = 270.0f;
						if (personaje[0].casillaActual >= 30 && personaje[0].casillaActual < 40) giroFinal = 0.0f;
					
						//CUERPO 
						model = glm::mat4(1.0);
						model = glm::translate(model, glm::vec3(
							personaje[0].posicion[0], personaje[0].posicion[1] + 1.4f, personaje[0].posicion[2]));
						model = glm::rotate(model, giroFinal * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

						modelaux = model;

						model = glm::scale(model, glm::vec3(personaje[0].escala, personaje[0].escala, personaje[0].escala));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
						personaje[0].modelo[0].RenderModel();

						//PIE DERECHO
						model = modelaux;
						model = glm::translate(model, glm::vec3(0.2f, 0.2f, 0.5f));
						model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
						model = glm::scale(model, glm::vec3(personaje[0].escala, personaje[0].escala, personaje[0].escala));
						glUniform3fv(uniformColor, 1, glm::value_ptr(color));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
						personaje[0].modelo[2].RenderModel();

						//PIE IZQUIERDO 
						model = modelaux;
						model = glm::translate(model, glm::vec3(-0.4f, 0.1f, -0.9f));
						model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
						model = glm::scale(model, glm::vec3(personaje[0].escala, personaje[0].escala, personaje[0].escala));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
						personaje[0].modelo[3].RenderModel();

						//MANO
						model = modelaux;
						model = glm::rotate(model, 90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
						model = glm::translate(model, glm::vec3(-0.4f, 2.5f, 1.0f));
						model = glm::rotate(model, -90.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
						model = glm::scale(model, glm::vec3(personaje[0].escala, personaje[0].escala, personaje[0].escala));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
						personaje[0].modelo[1].RenderModel();
						break;
					}
					case 1: {
						if (personaje[1].casillaActual >= 0 && personaje[1].casillaActual < 10) giroFinal = 90.0f;
						if (personaje[1].casillaActual >= 10 && personaje[1].casillaActual < 20) giroFinal = 180.0f;
						if (personaje[1].casillaActual >= 20 && personaje[1].casillaActual < 30) giroFinal = 270.0f;
						if (personaje[1].casillaActual >= 30 && personaje[1].casillaActual < 40) giroFinal = 0.0f;
					
						model = glm::mat4(1.0);
						/*base*/ {
							model = glm::translate(model, glm::vec3(
								personaje[1].posicion[0], personaje[1].posicion[1] + 8.0f, personaje[1].posicion[2]));
							model = glm::rotate(model, giroFinal * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
							float aux1 = 12.0f * sin(glm::radians(personaje[1].angulos[1]));
							model = glm::rotate(model, aux1 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
							modelaux = model;
							model = glm::scale(model, glm::vec3(personaje[1].escala, personaje[1].escala, personaje[1].escala));
							glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
							personaje[1].modelo[0].RenderModel();
						}
						float aux2;
						/*animacion de plumas*/ {
							if (personaje[1].angulos[2] < 360.0f) personaje[1].angulos[2] += 7.0f * deltaTime;
							else personaje[1].angulos[2] = 0.0f;
							aux2 = 14.5f * sin(glm::radians(personaje[1].angulos[2]));
						}
						/*pluma 1*/ {
							model = modelaux;
							model = glm::translate(model, glm::vec3(-0.9f, 2.2f, -0.6f));
							model = glm::rotate(model, aux2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
							model = glm::scale(model, glm::vec3(personaje[1].escala, personaje[1].escala, personaje[1].escala));
							glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
							personaje[1].modelo[1].RenderModel();
						}
						/*pluma 2*/ {
							model = modelaux;
							model = glm::translate(model, glm::vec3(-0.4f, 2.2f, -0.4f));
							model = glm::rotate(model, aux2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
							model = glm::scale(model, glm::vec3(personaje[1].escala, personaje[1].escala, personaje[1].escala));
							glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
							personaje[1].modelo[2].RenderModel();
						}
						/*pluma 3*/ {
							model = modelaux;
							model = glm::translate(model, glm::vec3(0.2f, 2.2f, -0.4f));
							model = glm::rotate(model, aux2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
							model = glm::scale(model, glm::vec3(personaje[1].escala, personaje[1].escala, personaje[1].escala));
							glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
							personaje[1].modelo[3].RenderModel();
						}
						/*pluma 4*/ {
							model = modelaux;
							model = glm::translate(model, glm::vec3(1.2f, 2.2f, -0.6f));
							model = glm::rotate(model, aux2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
							model = glm::scale(model, glm::vec3(personaje[1].escala, personaje[1].escala, personaje[1].escala));
							glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
							personaje[1].modelo[4].RenderModel();
						}
						break;
					}
					case 2: {
						if (personaje[2].casillaActual >= 0 && personaje[2].casillaActual < 10) giroFinal = 0.0f;
						if (personaje[2].casillaActual >= 10 && personaje[2].casillaActual < 20) giroFinal = 90.0f;
						if (personaje[2].casillaActual >= 20 && personaje[2].casillaActual < 30) giroFinal = 180.0f;
						if (personaje[2].casillaActual >= 30 && personaje[2].casillaActual < 40) giroFinal = 270.0f;
						
						//Simi Principal
						model = glm::mat4(1.0);
						model = glm::translate(model, glm::vec3(
							personaje[2].posicion[0], personaje[2].posicion[1], personaje[2].posicion[2]));
						model = glm::rotate(model, giroFinal * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
						modelaux = model;
						model = glm::scale(model, glm::vec3(personaje[2].escala, personaje[2].escala, personaje[2].escala));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
						personaje[2].modelo[0].RenderModel();

						model = modelaux;
						model = glm::translate(model, glm::vec3(0.2f, 2.5f, -1.0f));
						model = glm::rotate(model, -movSimiPrincipal * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
						personaje[2].modelo[1].RenderModel();

						model = modelaux;
						model = glm::translate(model, glm::vec3(0.2f, 2.5f, 1.0f));
						model = glm::rotate(model, movSimiPrincipal * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
						personaje[2].modelo[2].RenderModel();

						break;
					}
				}
			}
		}

		/*debug*/ {

			//printf("X: %d Y: %d Z: %d\n", mainWindow.getmuevex(), mainWindow.getmuevey(), mainWindow.getmuevez());
		}

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
