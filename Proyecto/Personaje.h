#pragma once
#include<stdio.h>
#include"Model.h"

class Personaje {
public:
	Personaje();
	float posicion[3];
	int casillaActual;
	int casillaSiguiente;
	Model modelo[5];
	float escala;
	bool avanza;
	
	float angulos[5];
	
	void iniModelos(int i);
	int validarCasilla(int i);
};
