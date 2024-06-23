#include "../constants_and_includes.h"

void bird_collide(GameObject* self, GameObject* other) {
    self->rect.x = 0;
    self->rect.y = (HEIGHT/2) - 25;
}

void bird_update(GameObject* self, SDL_Event* event) {
    GameObject* bird = (GameObject*)self;
    BirdData* birdData = (BirdData*)bird->special_ptr;

    if (event == NULL) {
        birdData->velocity_y += GRAVITY;
        if (birdData->velocity_y > MAX_BIRD_VELOCITY_Y)
            birdData->velocity_y = MAX_BIRD_VELOCITY_Y;
        if (birdData->velocity_y < -MAX_BIRD_VELOCITY_Y)
            birdData->velocity_y = -MAX_BIRD_VELOCITY_Y;
        self->rect.y += birdData->velocity_y;
    } else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE) {
        birdData->velocity_y -= 6;
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
    object1->special_ptr = malloc(sizeof(BirdData));

    return object1;
}