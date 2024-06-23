#include "../constants_and_includes.h"

void do_nothing(void* pt1, void* pt2, GameState* state) {}

void pipe_update(GameObject* object, SDL_Event* event, GameState* state) {
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

    int h_min = MINIMUM_PIPE_LENGTH;
    int h_max = HEIGHT - MINIMUM_PIPE_LENGTH - PIPE_GAP; // Set the bottom_pipe height to MINIMUM_PIPE_LENGTH and solve to get this
    int top_height = h_min + rand() % (h_max - h_min + 1);

    SDL_Rect r_top;
    r_top.x = x;
    r_top.h = top_height;
    r_top.y = 0;
    r_top.w = PIPE_WIDTH;
    

    SDL_Rect r_bottom;
    r_bottom.x = x;
    r_bottom.h = HEIGHT - r_top.h - PIPE_GAP;
    r_bottom.y = HEIGHT - r_bottom.h;
    r_bottom.w = PIPE_WIDTH;
    

    GameObject* objects = malloc(2*sizeof(GameObject));

    objects[0].rect = r_top;
    objects[0].texture = texture_top;
    objects[0].update = &pipe_update;
    objects[0].handle_collision = &do_nothing;
    objects[0].special_ptr = malloc(sizeof(PipeData));
    PipeData* object0_pipedata = (PipeData*) objects[0].special_ptr;
    object0_pipedata->is_top = true;

    objects[1].rect = r_bottom;
    objects[1].texture = texture_bottom;
    objects[1].update = &pipe_update;
    objects[1].handle_collision = &do_nothing;
    objects[1].special_ptr = malloc(sizeof(PipeData));
    PipeData* object1_pipedata = (PipeData*) objects[1].special_ptr;
    object1_pipedata->is_top = false;

    object0_pipedata->other_pipe = &objects[1];
    object1_pipedata->other_pipe = &objects[0];

    return objects;
}

void pipe_breaker_collide(GameObject* self, GameObject* other, GameState* state) {
    puts("pipe_breaker_collide called");

    GameObject* top_pipe;
    GameObject* bottom_pipe;

    PipeData* other_pipe_data = (PipeData*) other->special_ptr;
    if (other_pipe_data->is_top) {
        top_pipe = other;
        bottom_pipe = other_pipe_data->other_pipe;
    } else {
        bottom_pipe = other;
        top_pipe = other_pipe_data->other_pipe;
    }

    // Update pipe parameters to create the "infinite map" effect
    int new_x = state->pipe_locations[NUMBER_OF_PIPES-1];

    int h_min = MINIMUM_PIPE_LENGTH;
    int h_max = HEIGHT - MINIMUM_PIPE_LENGTH - PIPE_GAP; // Set the bottom_pipe height to MINIMUM_PIPE_LENGTH and solve to get this
    int top_height = h_min + rand() % (h_max - h_min + 1);

    top_pipe->rect.x = new_x;
    top_pipe->rect.h = top_height;
    bottom_pipe->rect.x = new_x;
    bottom_pipe->rect.h = (HEIGHT - top_height) - PIPE_GAP;
    bottom_pipe->rect.y = HEIGHT - bottom_pipe->rect.h;
}

GameObject* create_map(SDL_Renderer* renderer, GameState* state) {
    GameObject* map_objects = malloc((GAME_OBJECTS-1)*sizeof(GameObject));

    // initializes all our pipes
    srand(time(NULL));
    int pipe_location = WIDTH/2;
    state->pipe_locations = malloc(NUMBER_OF_PIPES*sizeof(int));
    for (int p = 0; p < NUMBER_OF_PIPES; p++) {
        printf("Pipe %d is getting created...\n", p);
        GameObject* pipe_objects = create_pipe_at_x(pipe_location, renderer);
        map_objects[2*p] = pipe_objects[0];
        map_objects[2*p + 1] = pipe_objects[1];
        free(pipe_objects);
        printf("Pipe %d was created...\n", p);
        state->pipe_locations[p] = pipe_location;
        pipe_location += (MIN_PIPE_TO_PIPE_DISTANCE + rand() % (MAX_PIPE_TO_PIPE_DISTANCE - MIN_PIPE_TO_PIPE_DISTANCE + 1));
    }

    // initalizes pipe breaker
    SDL_Texture *breaker_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Rect breaker_rect = {.x = -10-PIPE_WIDTH, .y = 0, .w = 10, .h = HEIGHT};
    map_objects[2*NUMBER_OF_PIPES].rect = breaker_rect;
    map_objects[2*NUMBER_OF_PIPES].texture = breaker_texture;
    map_objects[2*NUMBER_OF_PIPES].update = &do_nothing;
    map_objects[2*NUMBER_OF_PIPES].handle_collision = &pipe_breaker_collide;

    
    // initializes boundary box
    SDL_Texture *top_box_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Rect top_box_rect = {.x = 0, .y = 0, .w = WIDTH, .h = 10};
    map_objects[2*NUMBER_OF_PIPES+1].rect = top_box_rect;
    map_objects[2*NUMBER_OF_PIPES+1].texture = top_box_texture;
    map_objects[2*NUMBER_OF_PIPES+1].update = &do_nothing;
    map_objects[2*NUMBER_OF_PIPES+1].handle_collision = &do_nothing;

    SDL_Texture *bottom_box_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Rect bottom_box_rect = {.x = 0, .y = HEIGHT-10, .w = WIDTH, .h = 10};
    map_objects[2*NUMBER_OF_PIPES+2].rect = bottom_box_rect;
    map_objects[2*NUMBER_OF_PIPES+2].texture = bottom_box_texture;
    map_objects[2*NUMBER_OF_PIPES+2].update = &do_nothing;
    map_objects[2*NUMBER_OF_PIPES+2].handle_collision = &do_nothing;

    return map_objects;
}