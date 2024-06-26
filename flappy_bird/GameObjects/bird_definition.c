#include "../constants_and_includes.h"

void bird_collide(GameObject* self, GameObject* other, GameState* state) {
    // No action if this is a reward pipe
    //RewardPipeColliderData* reward_data = (RewardPipeColliderData*) other->special_ptr;
    puts("bird collide called");
    if (other->type_signature != NULL 
        && strcmp(other->type_signature, "reward_pipe") == 0) {
            return;
        }
    state->bird_dead = true;
}

void bird_update(GameObject* self, SDL_Event* event, GameState* state) {
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
    r.x = 30;
    r.y = (HEIGHT/2) - 25;
    r.w = 35;
    r.h = 35;

    GameObject* object1 = create_game_object(r, texture, &bird_update, &bird_collide, strdup("bird"), malloc(sizeof(BirdData))); 
    BirdData* ptr = (BirdData*) object1->special_ptr;
    ptr->velocity_x = 0;
    ptr->velocity_y = 0;

    return object1;
}