#include "./constants_and_includes.h"
#include "./bird_definition.c"
#include "./map_definition.c"


GameObject* initialize_game_objects(SDL_Renderer* renderer) {

    GameObject* bird = create_bird(renderer);
    GameObject* objects = malloc(GAME_OBJECTS*sizeof(GameObject));
    objects[0] = *bird;
    
    GameObject* map_objects = create_map(renderer);
    for (int i = 1; i < GAME_OBJECTS; i++) {
        objects[i] = map_objects[i-1];
    }
    
    return objects;
}