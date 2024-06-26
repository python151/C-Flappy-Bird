#include "../constants_and_includes.h"

void do_nothing(void* pt1, void* pt2, GameState* state) {}

void do_reward(GameObject* self, GameObject* other, GameState* state) {
    puts("do_reward called");
    if (other->type_signature == NULL) {
        puts("exiting");
        return;
    }
        

    puts("do_reward calledsdfsdfs");
    if (other == NULL 
        || other->type_signature == NULL 
        || strcmp(other->type_signature, "bird") != 0) {
            puts("exiting");
        return;
    }
    puts("check passed");
    RewardPipeColliderData* data = (RewardPipeColliderData*) self->special_ptr;
    if (data->is_active) {
        state->score++;
        data->is_active = false;
    }
}

void pipe_update(GameObject* object, SDL_Event* event, GameState* state) {
    puts("pipe_update called");
    if (event == NULL) {
        puts("Updating pipe location");
        object->rect.x -= 1;
    }
}

// Returns an array of 2 game objects
void create_pipe_at_x(GameObject** objects, int start, int x, SDL_Renderer* renderer) {
    SDL_Texture *texture_top = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Texture *texture_reward = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_SetTextureBlendMode(texture_reward, SDL_BLENDMODE_BLEND);

    // Set the texture as the render target
    SDL_SetRenderTarget(renderer, texture_reward);

    // Clear the texture with transparent color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // RGBA = (0, 0, 0, 0) for transparent
    SDL_RenderClear(renderer);

    // Reset the render target back to the default (the window)
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Texture *texture_bottom = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);


    if (texture_top == NULL || texture_bottom == NULL)
        exit(1);

    int h_min = MINIMUM_PIPE_LENGTH;
    int h_max = HEIGHT - MINIMUM_PIPE_LENGTH - PIPE_GAP;

    // Ensure h_max is at least h_min to avoid invalid range for rand()
    if (h_max < h_min) {
        exit(1);
        h_max = h_min;
    }

    int top_height = h_min + rand() % (h_max - h_min + 1);

    SDL_Rect r_top;
    r_top.x = x;
    r_top.h = top_height;
    r_top.y = 0;
    r_top.w = PIPE_WIDTH;

    SDL_Rect r_reward = {.h=HEIGHT, .x=x+10, .y=0, .w=10};

    SDL_Rect r_bottom;
    r_bottom.x = x;
    r_bottom.h = HEIGHT - r_top.h - PIPE_GAP;
    r_bottom.y = HEIGHT - r_bottom.h;
    r_bottom.w = PIPE_WIDTH;

    puts("starting game object creation");
    objects[start] = create_game_object(r_top, texture_top, &pipe_update, &do_nothing, NULL, malloc(sizeof(PipeData))); 
    PipeData* object0_pipedata = (PipeData*) objects[start]->special_ptr;
    object0_pipedata->is_top = true;
    puts("starting second game object creation");
    objects[start+1] = create_game_object(r_bottom, texture_bottom, &pipe_update, &do_nothing, NULL, malloc(sizeof(PipeData))); 
    PipeData* object1_pipedata = (PipeData*) objects[start+1]->special_ptr;
    object1_pipedata->is_top = false;
    puts("starting third game object creation");
    objects[start+2] = create_game_object(r_reward, texture_reward, &pipe_update, &do_reward, strdup("reward_pipe"), malloc(sizeof(RewardPipeColliderData))); 
    RewardPipeColliderData* reward_data = (RewardPipeColliderData*) objects[start+2]->special_ptr;
    reward_data->is_active = true;
    puts("creating relationships");
    object0_pipedata->reward_collider = objects[start+2];
    object1_pipedata->reward_collider = objects[start+2];

    object0_pipedata->other_pipe = objects[start+1];
    object1_pipedata->other_pipe = objects[start];
    puts("returning");
}

void pipe_breaker_collide(GameObject* self, GameObject* other, GameState* state) {
    puts("pipe_breaker_collide called");

    GameObject* top_pipe;
    GameObject* bottom_pipe;
    GameObject* reward_collider;

    PipeData* other_pipe_data = (PipeData*) other->special_ptr;
    if (other_pipe_data->is_top) {
        puts("is top!");
        top_pipe = other;
        bottom_pipe = other_pipe_data->other_pipe;
        reward_collider = other_pipe_data->reward_collider;
    } else {
        puts("is bottom!");
        bottom_pipe = other;
        top_pipe = other_pipe_data->other_pipe;
        reward_collider = other_pipe_data->reward_collider;
    }
    puts("we've decided!");

    if (top_pipe == NULL || bottom_pipe == NULL || reward_collider == NULL) {
        puts("Error: top_pipe, bottom_pipe, or reward_collider is NULL");
        return;
    }

    // Update pipe to create the "infinite map" effect
    int new_x = state->pipe_locations[NUMBER_OF_PIPES-1];
    puts("touching top pipe");
    top_pipe->rect.x = new_x;
    puts("touching bottom pipe");
    bottom_pipe->rect.x = new_x;
    puts("touching reward data");
    reward_collider->rect.x = new_x;
    puts("getting special reward data");
    if (reward_collider->type_signature != NULL 
        && strcmp(reward_collider->type_signature, "reward_pipe") == 0) {
            puts("it's for sure a reward thingy");
    }
    RewardPipeColliderData* data = (RewardPipeColliderData*) reward_collider->special_ptr;
    puts("touching special reward data");
    data->is_active = true;
    puts("pipe_breaker_collide complete");
}

void create_map(GameObject** objects, int start, int end, SDL_Renderer* renderer, GameState* state) {
    // initializes all our pipes
    srand(time(NULL));
    int pipe_location = WIDTH/2;
    state->pipe_locations = malloc(NUMBER_OF_PIPES*sizeof(int));
    for (int p = 0; p < NUMBER_OF_PIPES; p++) {
        printf("Pipe %d is getting created...\n", p);
        create_pipe_at_x(objects, 3*p+start, pipe_location, renderer);
        printf("Pipe %d was created...\n", p);
        state->pipe_locations[p] = pipe_location;
        pipe_location += (MIN_PIPE_TO_PIPE_DISTANCE + rand() % (MAX_PIPE_TO_PIPE_DISTANCE - MIN_PIPE_TO_PIPE_DISTANCE + 1));
    }

    // initalizes pipe breaker
    SDL_Texture *breaker_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Rect breaker_rect = {.x = -10-PIPE_WIDTH, .y = 0, .w = 10, .h = HEIGHT};
    objects[3*NUMBER_OF_PIPES+start] = create_game_object(breaker_rect, breaker_texture, &do_nothing, &pipe_breaker_collide, NULL, NULL); 
    
    // initializes boundary boxes
    SDL_Texture *top_box_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Rect top_box_rect = {.x = 0, .y = 0, .w = WIDTH, .h = 10};
    objects[3*NUMBER_OF_PIPES+1+start] = create_game_object(top_box_rect, top_box_texture, &do_nothing, &do_nothing, NULL, NULL); 

    SDL_Texture *bottom_box_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);
    SDL_Rect bottom_box_rect = {.x = 0, .y = HEIGHT-10, .w = WIDTH, .h = 10};
    objects[3*NUMBER_OF_PIPES+2+start] = create_game_object(bottom_box_rect, bottom_box_texture, &do_nothing, &do_nothing, NULL, NULL); 
}