#include "../constants_and_includes.h"
#include "./bird_definition.c"
#include "./map_definition.c"
#include "./score_board.c"


GameObject* initialize_game_objects(SDL_Renderer* renderer, GameState* state) {
    GameObject* bird = create_bird(renderer);
    
    GameObject* objects = malloc(GAME_OBJECTS*sizeof(GameObject));
    memcpy(&objects[0], bird, sizeof(GameObject));
    free(bird);
    
    puts("creating map...");
    GameObject* map_objects = create_map(renderer, state);
    puts("map created...");
    for (int i = 1; i < GAME_OBJECTS; i++) {
        printf("assigning map object #%d...\n", i);
        memcpy(&objects[i], &map_objects[i-1], sizeof(GameObject));
        printf("assigning map object #%d... done\n", i);
    }
    free(map_objects);
    puts("map assigned...");

    GameObject* scoreboard = create_scoreboard(renderer);
    memcpy(&objects[GAME_OBJECTS-1], scoreboard, sizeof(GameObject));
    
    return objects;
}