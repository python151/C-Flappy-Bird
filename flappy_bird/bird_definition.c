#include "./constants_and_includes.h"

void bird_collide(GameObject* self, GameObject* other) {
    self->rect.x = 0;
    self->rect.y = (HEIGHT/2) - 25;
}

void bird_update(GameObject* self, SDL_Event* event) {
    if (event == NULL) {
        self->velocity_y += GRAVITY;
        self->rect.y += self->velocity_y;
    } else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE) {
        self->velocity_y -= 10;
    }
}

GameObject* create_bird(SDL_Renderer* renderer) {
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);

    SDL_Rect r;
    r.x = 0;
    r.y = (HEIGHT/2) - 25;
    r.w = 35;
    r.h = 35;

    GameObject* object1 = malloc(sizeof(GameObject));
    object1->rect = r;
    object1->texture = texture;
    object1->update = &bird_update;
    object1->handle_collision = &bird_collide;
}