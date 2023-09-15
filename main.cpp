#include "SDL.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>

int largura_tela;
int altura_tela;
int aresta_cubo = 30;
double velocidade_cubo = 2.0;
int quantidade_cubos = 500;
bool rodando = true;

SDL_Window* janela = NULL;
SDL_Surface* superficie = NULL;
SDL_Renderer* render = NULL;

SDL_Event e;
bool quit = false;


int iniciar_SDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("falhou ao iniciar SDL, erro : %s \n", SDL_GetError());
		return EXIT_FAILURE;
	}
	else
	{
		janela = SDL_CreateWindow("ola mundo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, largura_tela, altura_tela, SDL_WINDOW_SHOWN);
		if (janela == NULL)
		{
			printf("falhou ao criar a janela, erro : %s \n", SDL_GetError());
			return EXIT_FAILURE;
		}
		else
		{
			/*
			psuperficie = SDL_GetWindowSurface(pjanela);
			SDL_FillRect(psuperficie, NULL, 0x000000);
			SDL_UpdateWindowSurface(pjanela);
			*/
			render = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

			if (render == NULL)
			{
				printf("falou ao criar o render, erro %s", SDL_GetError());
				return EXIT_FAILURE;
			}
		}
	}
}

void imput()
{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
				rodando = false;
			}
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_r)
				{
					rodando = false;
				}
			}

		}
}

void fechar_SDL(SDL_Window* pjanela)
{
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(janela);
	SDL_Quit();

}

double pi = 3.14;


//limitador de frames
Uint32 tick_intervalo = 8; //8 -> 125fps
Uint32 proximo_tick;
float frames = 0;
int segundo = 0;
void  limit_frames()
{
	Uint32 agora;
	frames++;

	agora = SDL_GetTicks();

	/*if (agora > segundo + 1000)
	{
		std::cout << frames << "\n\n";

		segundo = agora;

		frames = 0;
	}*/


	if (proximo_tick > agora)
	{
		SDL_Delay(proximo_tick - agora);
	}
	else if (proximo_tick < agora)
		proximo_tick = agora;


	proximo_tick += tick_intervalo;
}




struct simbolo
{
	SDL_FRect area;
	double direcao = 0;
	double velocidade = 0;
	int cor = 0;
};

bool mover(simbolo& a)
{
	double componente_x = cos(a.direcao) * a.velocidade;
	double componente_y = sin(a.direcao) * a.velocidade;

	a.area.x = a.area.x + componente_x;
	a.area.y = a.area.y + componente_y;

	
	bool colidiu_vertical = false;
	bool colidiu_horizontal = false;
	if (a.area.y + a.area.h > altura_tela || a.area.y < 0)
	{
		a.area.y = a.area.y - componente_y;
		a.direcao = 2 * pi - a.direcao;
		colidiu_vertical = true;
	}
	if (a.area.x + a.area.w > largura_tela || a.area.x < 0)
	{
		a.area.x = a.area.x - componente_x;
		a.direcao = pi - a.direcao;
		colidiu_horizontal = true;
	}

	if (colidiu_horizontal == true && colidiu_vertical == true)
	{
		a.cor = 1;
		std::cout << "objeto colidido " << "direcao pos ricochete: " << a.direcao/pi << "pi" << "\n";
	}

	return false;
}


std::vector<simbolo> pac;

int main(int argc, char *argv[])
{
	quit = false;
	while (quit == false) {
		std::cout << "digite o comprimento da tela : ";
		std::cin >> largura_tela;
		std::cout << "digite a altura da tela : ";
		std::cin >> altura_tela;
		std::cout << "digite a arestas do(s) cubo(s) : ";
		std::cin >> aresta_cubo;
		std::cout << "digite a velocidade do(s) cubo(s) : ";
		std::cin >> velocidade_cubo;
		std::cout << "digite a quatidade de cubos : ";
		std::cin >> quantidade_cubos;

		system("cls");

		std::cout << "aperte 'r' na janela da simulação para redefinir o tamanho da tela e os cubos\n";

		iniciar_SDL();


		srand((unsigned)time(NULL));








		for (int i = 0; i < quantidade_cubos; i++)
		{
			simbolo a = { rand() % (largura_tela - aresta_cubo), rand() % (altura_tela - aresta_cubo), aresta_cubo,aresta_cubo , (double)(rand() % 629) / 100.00 , velocidade_cubo };
			a.cor = 0;
			pac.push_back(a);
		}


		//GAME

		proximo_tick = SDL_GetTicks() + tick_intervalo;
		rodando = true;
		while (rodando)
		{
			SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(render);





			for (int i = 0; i < pac.size(); i++)
			{
				mover(pac[i]);
				if (pac[i].cor == 0)
					SDL_SetRenderDrawColor(render, 0xFF, 0x00, 0x00, 0xFF);
				else if (pac[i].cor == 1)
					SDL_SetRenderDrawColor(render, 0x00, 0x00, 0xFF, 0xFF);

				SDL_RenderFillRectF(render, &pac[i].area);
			}




			limit_frames();
			SDL_RenderPresent(render);
			imput();
		}

		pac.clear();
	}






		SDL_RenderPresent(render);

		//pausar();

		fechar_SDL(janela);
		return EXIT_SUCCESS;
	
}