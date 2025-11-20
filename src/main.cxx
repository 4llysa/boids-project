#ifdef _WIN32
#include <SDL.h>

#define not !

#else

#include <SDL2/SDL.h>

#endif

#include "it_s_work.h"

#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include "model/vec2.h"
#include "model/boid.h"
#include "model/flock.h"
#include "utility/renderer.h"

int const NUM_BOIDS = 100;
int const WIDTH = 800;
int const HEIGHT = 600;
float const PI = M_PI; // 3.1415927; // TODO: better PI

Flock myFlock(NUM_BOIDS, WIDTH, HEIGHT);

struct global_t {
	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;

	// random
	std::random_device rd;
	std::default_random_engine eng;
	std::uniform_real_distribution<float> rand;

};

global_t g;

void paint_it_s_work(int ox, int oy, int scale = 20) {
	SDL_SetRenderDrawColor(g.renderer, 0u, 0u, 0u, SDL_ALPHA_OPAQUE);
	for (int j = 0; j < px::height; ++j) {
		for (int i = 0; i < px::width; ++i) {
			if (px::header_data[j*px::width+i] == 0) {
				SDL_Rect r = { i*scale+ox, j*scale+oy, 20, 20 };
				SDL_RenderFillRect(g.renderer, &r);
			}
		}
	}
}


/**
 * @brief Renders a filled circle using the SDL Renderer API.
 * Uses a basic radial approach or a simple line-based approximation for filling.
 * For a truly filled circle, drawing vertical lines is most efficient.
 */
void draw_filled_circle(int center_x, int center_y, int radius) {
    // Set the drawing color (e.g., Blue for boids)
    SDL_SetRenderDrawColor(g.renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);

    // Iterate through X coordinates from center - radius to center + radius
    for (int x = center_x - radius; x <= center_x + radius; x++) {
        // Calculate the corresponding vertical distance (dy) based on the circle equation:
        // (x - center_x)^2 + (y - center_y)^2 = radius^2
        // y = center_y +/- sqrt(radius^2 - (x - center_x)^2)
        int dx = x - center_x;
        int dy_sq = radius * radius - dx * dx;
        
        if (dy_sq >= 0) {
            int dy = (int)std::sqrt(dy_sq);
            
            // Draw a vertical line segment from the top edge to the bottom edge
            // to fill the circle quickly.
            int y1 = center_y - dy;
            int y2 = center_y + dy;
            
            // Draw a line (or simply iterate and draw points, but SDL_RenderDrawLine is cleaner)
            // Note: SDL_RenderDrawLine might not be strictly necessary, 
            // but drawing many vertical points is usually faster for filling.
            // Since we need to draw many points, a loop is best for a true fill:
            for (int y = y1; y <= y2; y++) {
                SDL_RenderDrawPoint(g.renderer, x, y);
            }
        }
    }
}


void do_render() {
    // 1. Clear the screen (White background)
    SDL_SetRenderDrawColor(g.renderer, 255u, 255u, 255u, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(g.renderer);

    // 2. Draw all Boids
    float const BOID_SIZE = 10.0f;
    
    // Get the boids collection from the Flock object
    for (const Boid& b : myFlock.get_boids()) {
        Renderer::draw_oriented_boid(g.renderer, b, BOID_SIZE);
    }

    // 3. Present the rendered scene
    SDL_RenderPresent(g.renderer);
}

void do_update() {
    float const DT = 0.1f;
    // Delegate the update logic to the Flock object
    myFlock.update(DT, WIDTH, HEIGHT);
}

// void do_render() {
// 	SDL_SetRenderDrawColor(g.renderer, 255u, 255u, 255u, SDL_ALPHA_OPAQUE);
// 	SDL_RenderClear(g.renderer);

// 	// paint_it_s_work(0, 0, 20);
// 	int const BOID_RADIUS = 5; // Adjust size as needed
// 	Boid* boids;

// 	SDL_RenderPresent(g.renderer);
// }

// void do_update() {

// }

int main(int argc, char ** argv)
{


	int status;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER) != 0) {
		return 1;
	}

	g.window = SDL_CreateWindow("Ant War",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (not g.window) {
		return 1;
	}

	// get the default renderer
	g.renderer = SDL_CreateRenderer(g.window, -1, 0);
	if (not g.renderer) {
		return 1;
	}

	bool end = false;
	while (not end) {
		SDL_Event event;
		if (SDL_WaitEventTimeout(&event, 20)) {
			switch (event.type) {
			case SDL_WINDOWEVENT:
				switch (event.window.event) {
					case SDL_WINDOWEVENT_CLOSE:
						end = true;
						break;
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						// Should never happen
						break;
					default:
						break;
				}
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					end = true;
				}
				break;
			case SDL_KEYUP:
				break;
			}
		} else {
			// Got time out or error
			char const * e = SDL_GetError();
			if (e != NULL) {
				if (strlen(e) != 0) {
					// Got error
					return 1;
				}
			}

			do_update();
			do_render();
		}
	}

	SDL_DestroyRenderer(g.renderer);
	SDL_DestroyWindow(g.window);
	SDL_CloseAudio();
	SDL_Quit();
	return 0;
}

