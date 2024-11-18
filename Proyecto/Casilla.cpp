#include <string.h>
#include "Casilla.h"

Casilla::Casilla() {}

float escalaTablero = 50.0f;
float EscalaCentro = escalaTablero * 0.73f;
float escalaOrilla = EscalaCentro / 9;
float escalaEsquina = escalaOrilla + (escalaOrilla * 0.66f);

void Casilla::iniCasillas(int i) {
	/*posicion*/ {
		int coeficiente = i;
		if (i == 10 || i == 20 || i == 30) coeficiente = 0;//reiniciar cada que este en una esquina
		if (i > 10) coeficiente -= 10;//asigna a valor unitario de i
		if (i > 20) coeficiente -= 10;
		if (i > 30) coeficiente -= 10;
		if (i > 1) coeficiente += (coeficiente - 1);//salto de numero

		casillaPosicion[1] = 0.01f;//posicion en y
		//fila abajo
		if ((i > 0 && i < 10) || i == 0 || i == 40)	casillaPosicion[2] = (EscalaCentro + escalaEsquina);//Z fija en positivo
		if (i == 0 || i == 40)						casillaPosicion[0] = -(EscalaCentro + escalaEsquina);//X en negativa
		if (i > 0 && i < 10)						casillaPosicion[0] = -EscalaCentro + (coeficiente * escalaOrilla);//aumenta
		//columna derecha
		if ((i > 10 && i < 20) || i == 10)			casillaPosicion[0] = (EscalaCentro + escalaEsquina);//X fija en positivo
		if (i == 10)								casillaPosicion[2] = (EscalaCentro + escalaEsquina);//Z en positivo
		if (i > 10 && i < 20)						casillaPosicion[2] = EscalaCentro - (coeficiente * escalaOrilla);//disminuye
		//fila arriba
		if ((i > 20 && i < 30) || i == 20)			casillaPosicion[2] = -(EscalaCentro + escalaEsquina);//Z fija en negativo
		if (i == 20)								casillaPosicion[0] = (EscalaCentro + escalaEsquina);//X en positivo
		if (i > 20 && i < 30)						casillaPosicion[0] = EscalaCentro - (coeficiente * escalaOrilla);//disminuye
		//columna izquierda
		if ((i > 30 && i < 40) || i == 30)			casillaPosicion[0] = -(EscalaCentro + escalaEsquina);//X fija en negativo
		if (i == 30)								casillaPosicion[2] = -(EscalaCentro + escalaEsquina);//Z en negativo
		if (i > 30 && i < 40)						casillaPosicion[2] = -EscalaCentro + (coeficiente * escalaOrilla);//aumenta
	}
	
	/*escala casilla*/{
		casillaEscala[1] = escalaEsquina;//Escala en Z
		if (i == 0 || i == 10 || i == 20 || i == 30) casillaEscala[0] = escalaEsquina;//Escala en X
		else casillaEscala[0] = escalaOrilla;
	}
}

void Casilla::iniTexturas(int i) {
	if (i == 40) return;
	char archivoRuta[100] = "";//lugar donde se encuentra el archivo
	char archivoRutaDigito[2] = { '0', '0' };//num de personaje
	/*archivo*/ {
		int digito = i;
		if (i > 14) digito -= 14;//asigna al primer valor del universo
		if (i > 28) digito -= 14;
		if (i == 14 || i == 28) digito = 0;//reinicia el conteo

		if (digito < 10) archivoRutaDigito[1] = '0' + digito;//pone el ultimo digito como el valor de i
		else {//asigna el primer digito como 1 y los demas como i 
			archivoRutaDigito[0] = '0' + 1;
			archivoRutaDigito[1] = '0' + (digito - 10);
		}
	}
	
	/*textura*/ {
		char texturaTipo;//tipo de textura
		for (int j = 0; j < 3; j++) {
			if (i >= 0 && i < 14) strcpy_s(archivoRuta, "Textures/casillas/mario/mario");
			if (i >= 14 && i < 28) strcpy_s(archivoRuta, "Textures/casillas/crash/crash");
			if (i >= 28 && i < 40) strcpy_s(archivoRuta, "Textures/casillas/simi/simi");
			switch (j) {
			case 0: texturaTipo = 'D'; break;
			case 1: texturaTipo = 'N'; break;
			case 2: texturaTipo = 'E'; break;
			}
			strncat_s(archivoRuta, &texturaTipo, 1);//bandera de Dia Noche o Especial
			strncat_s(archivoRuta, &archivoRutaDigito[0], 1);//numero de casilla
			strncat_s(archivoRuta, &archivoRutaDigito[1], 1);
			strcat_s(archivoRuta, ".png");//extension png
			textura[j] = Texture(archivoRuta);
			textura[j].LoadTextureA();
		}
	}

	/*banderas*/
	especial = false;
}

void Casilla::iniModelos(int i) {
	if (i == 40) return;
	char archivoRuta[100] = "";//lugar donde se encuentra el archivo
	char archivoRutaDigito[2] = { '0', '0' };//num de personaje
	/*archivo*/ {
		int digito = i;
		if (i > 14) digito -= 14;//asigna al primer valor del universo
		if (i > 28) digito -= 14;
		if (i == 14 || i == 28) digito = 0;//reinicia el conteo

		if (digito < 10) archivoRutaDigito[1] = '0' + digito;//pone el ultimo digito como el valor de i
		else {//asigna el primer digito como 1 y los demas como i 
			archivoRutaDigito[0] = '0' + 1;
			archivoRutaDigito[1] = '0' + (digito - 10);
		}
	}

	/*modelo*/ {
		if (i >= 0 && i < 14) strcpy_s(archivoRuta, "Models/mario");
		if (i >= 14 && i < 28) strcpy_s(archivoRuta, "Models/crash");
		if (i >= 28 && i < 40) strcpy_s(archivoRuta, "Models/simi");
		strncat_s(archivoRuta, &archivoRutaDigito[0], 1);//numero de casilla
		strncat_s(archivoRuta, &archivoRutaDigito[1], 1);
		strcat_s(archivoRuta, ".obj");//extension png
		modelo = Model();
		//modelo.LoadModel(archivoRuta);
		modeloSube = false;//bandera de si sube o no
	}

	/*escala y angulo modelo*/ switch (i) {
		/*mario tuberia*/	case 0: modeloEscala = 1.2f; modeloAngulo = 180.0f; break;
		/*mario florhielo*/	case 1: modeloEscala = 0.8f; modeloAngulo = 180.0f; break;
		/*mario DK*/		case 2: modeloEscala = 0.9f; modeloAngulo = 180.0f; break;
		/*mario bowser*/	case 3: modeloEscala = 0.7f; modeloAngulo = 180.0f; break;
		/*mario peach*/		case 4: modeloEscala = 13.0f; modeloAngulo = 180.0f; break;
		/*mario goomba*/	case 5: modeloEscala = 0.8f; modeloAngulo = 180.0f; break;
		/*mario bomba*/		case 6: modeloEscala = 0.8f; modeloAngulo = 180.0f; break;
		/*mario planta*/	case 7: modeloEscala = 0.8f; modeloAngulo = 180.0f; break;
		/*mario flor*/		case 8: modeloEscala = 0.8f; modeloAngulo = 180.0f; break;
		/*mario estrella*/	case 9: modeloEscala = 0.8f; modeloAngulo = 180.0f; break;
		/*mario caja*/		case 10: modeloEscala = 2.0f; modeloAngulo = 0.0f; break;
		/*mario hongo*/		case 11: modeloEscala = 0.8f; modeloAngulo = 180.0f; break;
		/*mario koopa*/		case 12: modeloEscala = 3.0f; modeloAngulo = 0.0f; break;
		/*mario cheep*/		case 13: modeloEscala = 0.8f; modeloAngulo = 180.0f; break;
		/*crash coco*/		case 14: modeloEscala = 0.7f; modeloAngulo = 90.0f; break;
		/*crash spyro*/		case 15: modeloEscala = 0.5f; modeloAngulo = 180.0f; break;
		/*crash cortex*/	case 16: modeloEscala = 8.0f; modeloAngulo = 180.0f; break;
		/*crash dingo*/		case 17: modeloEscala = 4.5f; modeloAngulo = 180.0f; break;
		/*crash crate*/		case 18: modeloEscala = 1.0f; modeloAngulo = 0.0f; break;
		/*crash fake*/		case 19: modeloEscala = 0.3f; modeloAngulo = 180.0f; break;
		/*crash warp*/		case 20: modeloEscala = 0.3f; modeloAngulo = 90.0f; break;
		/*crash faster*/	case 21: modeloEscala = 0.2f; modeloAngulo = 0.0f; break;
		/*crash wumpa*/		case 22: modeloEscala = 0.1f; modeloAngulo = 0.0f; break;
		/*crash stormy*/	case 23: modeloEscala = 0.15f; modeloAngulo = 180.0f; break;
		/*crash blimp*/		case 24: modeloEscala = 0.15f; modeloAngulo = 180.0f; break;
		/*crash polar*/		case 25: modeloEscala = 0.1f; modeloAngulo = 180.0f; break;
		/*crash pura*/		case 26: modeloEscala = 0.5f; modeloAngulo = 180.0f; break;
		/*crash ripper*/	case 27: modeloEscala = 0.1f; modeloAngulo = 180.0f; break;
		/*simi ejecutivo*/	case 28: modeloEscala = 1.0f; modeloAngulo = 90.0f; break;
		/*simi moto*/		case 29: modeloEscala = 1.0f; modeloAngulo = 90.0f; break;
		/*simi estacion*/	case 30: modeloEscala = 3.0f; modeloAngulo = 180.0f; break;
		/*simi colonia*/	case 31: modeloEscala = 6.0f; modeloAngulo = 180.0f; break;
		/*simi jetona*/		case 32: modeloEscala = 6.0f; modeloAngulo = 180.0f; break;
		/*simi fino*/		case 33: modeloEscala = 1.0f; modeloAngulo = 90.0f; break;
		/*simi chapulin*/	case 34: modeloEscala = 1.0f; modeloAngulo = 90.0f; break;
		/*simi bombero*/	case 35: modeloEscala = 1.0f; modeloAngulo = 90.0f; break;
		/*simi alucin*/		case 36: modeloEscala = 1.0f; modeloAngulo = 90.0f; break;
		/*simi normal*/		case 37: modeloEscala = 1.0f; modeloAngulo = 90.0f; break;
		/*simi farmacia*/	case 38: modeloEscala = 0.06f; modeloAngulo = 180.0f; break;
		/*simi lunes*/		case 39: modeloEscala = 8.0f; modeloAngulo = 0.0f; break;
	}
	modeloGira = 0.0f;
}
