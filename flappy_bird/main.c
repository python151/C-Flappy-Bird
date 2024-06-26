#include "constants_and_includes.h"

#include "./renderer.c"
#include "./GameObject.c"
#include "./GameObjects/objects.c"

int main (int argc, char **argv)
{
    /* Initialises data */
    SDL_Window *window = create_window();
    SDL_Renderer *renderer = create_renderer(window);

    puts("window opened, setting draw color");
    

    // Set render color to red ( background will be rendered in this color )

    puts("initializing gamestate");

    // NOTE: When dealing with more dynamic games, further optimization is required
    GameState* state = malloc(sizeof(GameState));
    state->score = 0;
    state->bird_dead = false;
    state->renderer = renderer;

    puts("gamestate initalized");

    GameObject* objects[GAME_OBJECTS];
    initialize_game_objects(&objects, renderer, state);
    puts("Game objects initialized...");

    bool running = true;
    SDL_Event event;
    Uint32 frameStart;
    int frameTime;

    puts("Starting game loop...");
    while (running) {
        puts("Getting ticks...");
        frameStart = SDL_GetTicks();
        puts("Getting ticks... done");

        puts("Handling events...");
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (!state->bird_dead) {
                // Update game state
                for (int i = 0; i < GAME_OBJECTS; i++) {
                    printf("Updating game state on event for object #%d...\n", i);
                    objects[i]->update(objects[i], &event, state);
                    printf("Updating game state on event for object #%d... done\n", i);
                }
            }  
        }
        puts("Handling events... done");

        if (!state->bird_dead) {
            // Update game state
            for (int i = 0; i < GAME_OBJECTS; i++) {
                printf("Updating game state for object #%d...\n", i);
                objects[i]->update(objects[i], NULL, state);
                printf("Updating game state for object #%d... done\n", i);
            }
        }

        // Check for collisions
        // NOTE: When working with larger amounts of objects, this must be optimized.
        if (!state->bird_dead) {
            for (int i = 0; i < GAME_OBJECTS; i++) {
                for (int j = i + 1; j < GAME_OBJECTS; j++) {
                    if (SDL_HasIntersection(&(objects[i]->rect), &(objects[j]->rect))) {
                        printf("Handling collision #%d and #%d...\n", i, j);
                        objects[i]->handle_collision(objects[i], objects[j], state);
                        printf("Done and now Handling collision #%d and #%d...\n", j, i);
                        objects[j]->handle_collision(objects[j], objects[i], state);
                        printf("Done and now Handling collision #%d and #%d... done\n", j, i);
                    }
                }
            }
        }


        // Render
        SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
        SDL_RenderClear(renderer);
        for (int i = 0; i < GAME_OBJECTS; i++) {
            printf("Rendering object #%d...\n", i);
            if (renderer == NULL || objects[i]->texture == NULL)
                exit(1);
            SDL_RenderCopy(renderer, objects[i]->texture, NULL, &objects[i]->rect);
            printf("Rendering object #%d... done\n", i);
        }

        puts("presenting render calculation");
        SDL_RenderPresent(renderer);

        puts("doing frametime calculation");

        frameTime = SDL_GetTicks() - frameStart;

        puts("doing delay if needed");

        // If frame finished early, delay to maintain the target frame rate
        if (FRAME_DELAY > frameTime) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
        puts("game loop iteration finished");
    }

    /* Frees memory */
    SDL_DestroyWindow(window);
    free(state);
    for (int i = 0; i < GAME_OBJECTS; i++) {
        destroy_game_object(objects[i]);
    }
    
    /* Shuts down all SDL subsystems */
    SDL_Quit(); 
    
    return 0;
}