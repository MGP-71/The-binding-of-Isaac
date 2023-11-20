#include "MenuLayer.h"
#include "Gamelayer.h"

MenuLayer::MenuLayer(Game* game)
	: Layer(game) {
	init();
	gamePad = SDL_GameControllerOpen(0);
}

void MenuLayer::init() {
	// Fondo normal, sin velocidad
	background = new Background("res/menu_background.png", WIDTH * 0.5, HEIGHT * 0.5, game);
	button = new Actor("res/boton_isaac.png", WIDTH * 0.48, HEIGHT * 0.2, 256, 66, game);
	button_eden = new Actor("res/boton_eden.png", WIDTH * 0.48, HEIGHT * 0.3, 208, 64, game);
	button_cain = new Actor("res/boton_cain.png", WIDTH * 0.48, HEIGHT * 0.4, 204, 63, game);
	button_eve = new Actor("res/boton_eve.png", WIDTH * 0.48, HEIGHT * 0.5, 158, 58, game);
	button_judas = new Actor("res/boton_judas.png", WIDTH * 0.5, HEIGHT * 0.6, 258, 68, game);
}

void MenuLayer::draw() {
	background->draw();
	button->draw();
	button_eden->draw();
	button_cain->draw();
	button_eve->draw();
	button_judas->draw();

	SDL_RenderPresent(game->renderer); // Renderiza NO PUEDE FALTAR
}


void MenuLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_CONTROLLERDEVICEADDED) {
			gamePad = SDL_GameControllerOpen(0);
			if (gamePad == NULL) {
				cout << "error en GamePad" << endl;
			}
			else {
				cout << "GamePad conectado" << endl;
			}
		}
		// Cambio automático de input
		// PONER el GamePad
		if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERAXISMOTION) {
			game->input = game->inputGamePad;
		}
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}
		// Procesar teclas
		if (game->input == game->inputGamePad) {  // gamePAD
			gamePadToControls(event);
		}
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}
	}

	//procesar controles, solo tiene uno
	if (controlContinue) {
		// Cambia la capa
		game->layer = new GameLayer(game, game->personaje);
		controlContinue = false;
	}
}

void MenuLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE: // derecha
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_SPACE: // dispara
			controlContinue = true;
			break;
		}
	}
}

void MenuLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;

	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (button->containsPoint(motionX, motionY)) {
			game->personaje = 0;
			controlContinue = true;
		}if (button_eden->containsPoint(motionX, motionY)) {
			game->personaje = 1;
			controlContinue = true;
		}if (button_cain->containsPoint(motionX, motionY)) {
			game->personaje = 2;
			controlContinue = true;
		}if (button_eve->containsPoint(motionX, motionY)) {
			game->personaje = 3;
			controlContinue = true;
		}if (button_judas->containsPoint(motionX, motionY)) {
			game->personaje = 4;
			controlContinue = true;
		}
	}
}

void MenuLayer::gamePadToControls(SDL_Event event) {
	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(gamePad, SDL_CONTROLLER_BUTTON_A);

	if (buttonA) {
		controlContinue = true;
	}
}



