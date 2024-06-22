#include "constants_and_includes.h"

#include "./renderer.c"
#include "./GameObject.c"
#include "./objects.c"

int main (int argc, char **argv)
{
    /* Initialises data */
    SDL_Window *window = create_window();
    SDL_Renderer *renderer = create_renderer(window);
    

    // Set render color to red ( background will be rendered in this color )
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );

    GameObject* objects = initialize_game_objects(renderer);


    bool running = true;
    SDL_Event event;
    
    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            // Update game state
            for (int i = 0; i < GAME_OBJECTS; i++) {
                objects[i].update(&(objects[i]), &event);
            }
        }

        // Update game state
        for (int i = 0; i < GAME_OBJECTS; i++) {
            objects[i].update(&(objects[i]), NULL);
        }

        // Check for collisions
        for (int i = 0; i < GAME_OBJECTS; i++) {
            for (int j = i + 1; j < GAME_OBJECTS; j++) {
                if (SDL_HasIntersection(&(objects[i].rect), &(objects[j].rect))) {
                    objects[i].handle_collision(&(objects[i]),&(objects[j]));
                    objects[j].handle_collision(&(objects[j]), &(objects[i]));
                }
            }
        }


        // Render
        SDL_RenderClear(renderer);
        for (int i = 0; i < GAME_OBJECTS; i++) {
            SDL_RenderCopy(renderer, objects[i].texture, NULL, &objects[i].rect);
        }
        SDL_RenderPresent(renderer);

        SDL_Delay(16); // Delay to cap frame rate (approx 60 FPS)
    }

    /* Frees memory */
    SDL_DestroyWindow(window);
    
    /* Shuts down all SDL subsystems */
    SDL_Quit(); 
    
    return 0;
}