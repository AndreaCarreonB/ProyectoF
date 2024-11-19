#include "Casilla.h"
#include "Personaje.h"

Personaje::Personaje() {}

void Personaje::iniModelos(int i) {
	for (int m = 0; m < 5; m++) {
		modelo[m] = Model();
		switch (i) {
			case 0: {
				escala = 0.80f;
				switch (m) {
				case 0: modelo[m].LoadModel("Models/cuerpo.obj"); break;
				case 1: modelo[m].LoadModel("Models/mano.obj"); break;
				case 2: modelo[m].LoadModel("Models/pieder.obj"); break;
				case 3: modelo[m].LoadModel("Models/pieizq.obj"); break;
				}break;
			}
			case 1: {
				escala = 0.75f;
				switch (m) {
					case 0: modelo[m].LoadModel("Models/akuBase.obj"); break;
					case 1: modelo[m].LoadModel("Models/akuPluma1.obj"); break;
					case 2: modelo[m].LoadModel("Models/akuPluma2.obj"); break;
					case 3: modelo[m].LoadModel("Models/akuPluma3.obj"); break;
					case 4: modelo[m].LoadModel("Models/akuPluma4.obj"); break;
				}break;
			}
			case 2: {
				escala = 1.0f;
				switch (m) {
				case 0: modelo[m].LoadModel("Models/simi_camina_cuerpo.obj"); break;
				case 1: modelo[m].LoadModel("Models/simi_camina_piernader.obj"); break;
				case 2: modelo[m].LoadModel("Models/simi_camina_piernaizq.obj"); break;
				}break;
			}
		}
	}
}

int Personaje::validarCasilla(int i) {
	if (i >= 40) i -= 40;
	return i;
}	
