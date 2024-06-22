#include "./constants_and_includes.h"

void move_right(GameObject* object, SDL_Event* event) {
    if (event == NULL)
        object->rect.x += 1;
    else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE) 
        object->rect.x -= 20;
}

GameObject* initialize_game_objects(SDL_Renderer* renderer) {
    // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
    SDL_Rect r;
    r.x = 0;
    r.y = HEIGHT-50;
    r.w = 50;
    r.h = 50;

    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Texture *texture1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);

    GameObject* object1 = malloc(sizeof(GameObject));
    object1->rect = r;
    object1->texture = texture;
    object1->update = &move_right;

    // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
    SDL_Rect r1;
    r1.x = 50;
    r1.y = 50;
    r1.w = 50;
    r1.h = 50;

    GameObject* object2 = malloc(sizeof(GameObject));
    object2->rect = r1;
    object2->texture = texture1;
    object2->update = &move_right;

    GameObject* objects = malloc(GAME_OBJECTS*sizeof(GameObject));
    objects[0] = *object1;
    objects[1] = *object2;
    
    return objects;
}