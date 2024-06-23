#include "./constants_and_includes.h"

void do_nothing(void* pt1, void* pt2) {}

void pipe_update(GameObject* object, SDL_Event* event) {
    puts("pipe_update called");
    if (event == NULL) {
        puts("Updating pipe location");
        object->rect.x -= 1;
    }
}

// Returns an array of 2 game objects
GameObject* create_pipe_at_x(int x, SDL_Renderer* renderer) {
    SDL_Texture *texture_top = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Texture *texture_bottom = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);

    SDL_Rect r_top;
    r_top.x = x;
    r_top.h = 250;
    r_top.y = 0;
    r_top.w = 50;
    

    SDL_Rect r_bottom;
    r_bottom.x = x;
    r_bottom.h = r_top.h - PIPE_GAP;
    r_bottom.y = HEIGHT - r_bottom.h;
    r_bottom.w = 50;
    

    GameObject* objects = malloc(2*sizeof(GameObject));
    objects[0].rect = r_top;
    objects[0].texture = texture_top;
    objects[0].update = &pipe_update;
    objects[0].handle_collision = &do_nothing;

    objects[1].rect = r_bottom;
    objects[1].texture = texture_bottom;
    objects[1].update = &pipe_update;
    objects[1].handle_collision = &do_nothing;

    return objects;
}

void pipe_breaker_collide(GameObject* self, GameObject* other) {

}

GameObject* create_map(SDL_Renderer* renderer) {
    GameObject* map_objects = malloc((GAME_OBJECTS-1)*sizeof(GameObject));

    // initializes all our pipes
    int pipe_locations[] = {2*WIDTH/3, WIDTH};
    for (int p = 0; p < 2; p++) {
        printf("Pipe %d is getting created...\n", p);
        GameObject* pipe_objects = create_pipe_at_x(pipe_locations[p], renderer);
        map_objects[2*p] = pipe_objects[0];
        map_objects[2*p + 1] = pipe_objects[1];
        printf("Pipe %d was created...\n", p);
    }

    // initalizes pipe breaker

    // initializes boundary box

    return map_objects;
}