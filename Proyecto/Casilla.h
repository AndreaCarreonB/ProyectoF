#pragma once
#include<stdio.h>
#include "Texture.h"
#include"Model.h"

class Casilla {
public:
	Casilla();
	float casillaPosicion[3];
	float casillaEscala[2];
	Texture textura[3];
	bool especial;
	float modeloPosicion[3];
	Model modelo;
	float modeloEscala;
	float modeloAngulo;
	float modeloGira;
	bool modeloSube;
	void iniCasillas(int i);
	void iniTexturas(int i);
	void iniModelos(int i);
};
