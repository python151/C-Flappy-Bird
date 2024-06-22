#include "./constants_and_includes.h"

void move_right(GameObject* object, SDL_Event* event) {
    if (event == NULL)
        object->rect.x += 1;
}

void move_left(GameObject* object, SDL_Event* event) {
    if (event == NULL)
        object->rect.x -= 1;
}

void move_to_left_on_impact(GameObject* self, GameObject* other) {
    self->rect.x = 0;
}

void move_to_right_on_impact(GameObject* self, GameObject* other) {
    self->rect.x = WIDTH-50;
}

void do_nothing_on_impact(GameObject* self, GameObject* other) {}

GameObject* initialize_game_objects(SDL_Renderer* renderer) {
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Texture *texture1 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);


    // Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
    SDL_Rect r;
    r.x = 0;
    r.y = (HEIGHT/2) - 25;
    r.w = 50;
    r.h = 50;

    SDL_Rect r1;
    r1.x = WIDTH-50;
    r1.y = (HEIGHT/2) - 25;
    r1.w = 50;
    r1.h = 50;


    GameObject* object1 = malloc(sizeof(GameObject));
    object1->rect = r;
    object1->texture = texture;
    object1->update = &move_right;
    object1->handle_collision = &move_to_left_on_impact;

    GameObject* object2 = malloc(sizeof(GameObject));
    object2->rect = r1;
    object2->texture = texture1;
    object2->update = &move_left;
    object2->handle_collision = &move_to_right_on_impact;


    GameObject* objects = malloc(GAME_OBJECTS*sizeof(GameObject));
    objects[0] = *object1;
    objects[1] = *object2;
    
    return objects;
}