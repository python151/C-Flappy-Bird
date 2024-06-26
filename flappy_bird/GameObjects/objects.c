#include "../constants_and_includes.h"
#include "./bird_definition.c"
#include "./map_definition.c"
#include "./score_board.c"


void initialize_game_objects(GameObject** objects, SDL_Renderer* renderer, GameState* state) {
    puts("creating bird");
    GameObject* bird = create_bird(renderer);
    objects[0] = bird;
    
    puts("creating map...");
    create_map(objects, 1, GAME_OBJECTS-2, renderer, state);
    puts("map assigned...");

    puts("creating scoreboard");
    GameObject* scoreboard = create_scoreboard(renderer);
    objects[GAME_OBJECTS-1] = scoreboard;
    puts("scoreboard assigned");
}