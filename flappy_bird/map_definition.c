#include "./constants_and_includes.h"

void do_nothing_on_impact(GameObject* self, GameObject* other) {}

void pipe_update(GameObject* object, SDL_Event* event) {
    if (event == NULL) {
        object->rect.x -= 1;
    }
}

GameObject* create_map(SDL_Renderer* renderer) {
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);

    SDL_Rect r;
    r.x = 300;
    r.y = HEIGHT - 400;
    r.w = 50;
    r.h = 400;

    GameObject* object1 = malloc(sizeof(GameObject));
    object1->rect = r;
    object1->texture = texture;
    object1->update = &pipe_update;
    object1->handle_collision = &do_nothing_on_impact;
}