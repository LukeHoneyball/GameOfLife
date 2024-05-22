#include <iostream>
#include <SDL.h>
#include <SDL_events.h>
#include <thread>
#include <Windows.h>
using namespace std;

int grid[100][100];
int gridbuf[100][100];
int cellsrnd(int x, int y) {
	int tot = 0;
	if (grid[x - 1][y - 1] == 1) { tot++; }//top left
	if (grid[x + 1][y - 1] == 1) { tot++; }//top right
	if (grid[x + 1][y + 1] == 1) { tot++; }//bottom right
	if (grid[x - 1][y + 1] == 1) { tot++; }//bottom left
	if (grid[x][y + 1] == 1) { tot++; }//bottom
	if (grid[x - 1][y] == 1) { tot++; }//left
	if (grid[x + 1][y] == 1) { tot++; }//right
	if (grid[x][y - 1] == 1) { tot++; }//top
	return tot;
}

int rnext = 255;
int gnext = 0;
int bnext = 255;

int main(int argc, char* argv[])
{	/*GAME OF LIFE RULES
	Any live cell with fewer than two live neighbors dies, as if by underpopulation.
	Any live cell with two or three live neighbors lives on to the next generation.
	Any live cell with more than three live neighbors dies, as if by overpopulation.
	Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
	
	*/
	FreeConsole();
	bool running = false;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	
	SDL_Window* w = NULL;
	
	SDL_Renderer* r;
	w = SDL_CreateWindow("GAME OF LIFE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500,0);
	

	r =SDL_CreateRenderer(w,-1,0);
	SDL_RenderClear(r);
	
	SDL_RenderPresent(r);
	SDL_Event e;
	bool loop = true;
	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			grid[x][y] = 0;
		}
	}
	for (int x = 0; x < 100; x++) {
		for (int y = 0; y < 100; y++) {
			gridbuf[x][y] = 0;
		}
	}
	int x = 0;
	int y = 0;
	bool down = false;
	bool cntrl = false;
	while (loop) {
		
		while (SDL_PollEvent(&e)) {

			SDL_GetMouseState(&x, &y);

			if (e.type == SDL_QUIT) { SDL_Quit(); loop = false; }
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (cntrl) {
					
					if (x / 5 < 100 && y / 5 < 100 && x / 5 > 0 && y / 5 > 0) {
						grid[x / 5][y / 5] = 0;
						SDL_SetRenderDrawColor(r, 0,0,0, 0);


									SDL_Rect rect;
									rect.w = 5;
									rect.h = 5;
									rect.x = x * 5;
									rect.y = y * 5;

						SDL_RenderFillRect(r, &rect);
						SDL_RenderClear(r);
						SDL_RenderPresent(r);
					
					}

				}
				else if (x / 5 < 100 && y / 5 < 100 && x / 5 > 0 && y / 5 > 0) {
					grid[x / 5][y / 5] = 1;
					down = true;
				}
				
				
			}
			else if (e.type == SDL_MOUSEBUTTONUP) {
				
				down = false;
			
			}
			if (down) {
				if (x / 5 < 100 && y / 5 < 100 && x / 5 > 0 && y / 5 > 0) {
					grid[x / 5][y / 5] = 1;
				}
			}
			if (e.type == SDL_KEYDOWN && e.key.keysym.scancode == 224) {
				cntrl = true;
			}
			if (e.type == SDL_KEYUP && e.key.keysym.scancode == 224) {
				cntrl = false;
			}
			if (e.type == SDL_KEYDOWN) {
				
				
				if (e.key.keysym.scancode == 6) {
					gnext = rand() % 255;
					rnext = rand() % 255;
					bnext = rand() % 255;
				}
				else if (e.key.keysym.scancode == 44) {
					if (running) {
						running = false;
					}
					else {
						running = true;
					}

				}
				else if (e.key.keysym.scancode == 21) {
					
					for (int x = 0; x < 100; x++) {
						for (int y = 0; y < 100; y++) {
							grid[x][y] = 0;
						}
					}
					for (int x = 0; x < 100; x++) {
						for (int y = 0; y < 100; y++) {
							gridbuf[x][y] = 0;
						}
					}
					SDL_SetRenderDrawColor(r, 0, 0, 0, 0);
					SDL_RenderClear(r);
				}
			}

		}
		
		for (int x = 0; x < 100; x++) {
			for (int y = 0; y < 100; y++) {
				if (grid[x][y] == 1) {

					SDL_SetRenderDrawColor(r, rnext, gnext,bnext, 0);


					SDL_Rect rect;
					rect.w = 5;
					rect.h = 5;
					rect.x = x * 5;
					rect.y = y * 5;

					SDL_RenderFillRect(r, &rect);
					SDL_SetRenderDrawColor(r, 0, gnext, bnext, 0);
					SDL_RenderDrawRect(r, &rect);
					
				}
				
			}
		}
		SDL_RenderPresent(r);
		if (running) {
			SDL_SetRenderDrawColor(r,0,0,0,0);
			SDL_RenderClear(r);
			SDL_Delay(25);
			

			for (int x = 0; x < 100; x++) {
				for (int y = 0; y < 100; y++) {
					int num = cellsrnd(x, y);
					if (grid[x][y] == 1) {
						if (num == 2 || num == 3) {
							gridbuf[x][y] = 1;
						}
						else {
							gridbuf[x][y] = 0;
						}
					}
					else {
						if (num == 3) {
							gridbuf[x][y] = 1;
						}
					}
				}
			}
			for (int x = 0; x < 100; x++) {
				for (int y = 0; y < 100; y++) {
					grid[x][y] = gridbuf[x][y];
				}
			}
		}
	}
	return 0;
}
