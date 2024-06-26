#include "../constants_and_includes.h"


SDL_Texture* concatenateTextures(SDL_Renderer* renderer, SDL_Texture* texture1, SDL_Texture* texture2) {
    // Query texture sizes
    int width1, height1, width2, height2;
    SDL_QueryTexture(texture1, NULL, NULL, &width1, &height1);
    SDL_QueryTexture(texture2, NULL, NULL, &width2, &height2);

    // Create a new texture to hold both textures side by side
    SDL_Texture* concatenatedTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width1 + width2, SDL_max(height1, height2));
    if (!concatenatedTexture) {
        printf("Failed to create concatenated texture! SDL_Error: %s\n", SDL_GetError());
        return NULL;
    }

    // Render both textures to the new texture
    SDL_SetRenderTarget(renderer, concatenatedTexture);
    SDL_RenderClear(renderer);
    SDL_Rect destRect1 = {0, 0, width1, height1};
    SDL_RenderCopy(renderer, texture1, NULL, &destRect1);
    SDL_Rect destRect2 = {width1, 0, width2, height2};
    SDL_RenderCopy(renderer, texture2, NULL, &destRect2);
    SDL_SetRenderTarget(renderer, NULL);

    return concatenatedTexture;
}

SDL_Texture* load_asset_from_number(int num, SDL_Renderer* renderer) {
    if (-1 > num || num > 9)
        return NULL;

    // Construct filepath
    // Note: path is relative to main.c, not individual C file
    char file_path[100];
    sprintf(&file_path, "./assets/%d.bmp", num); 

    puts("loading surface");
    SDL_Surface* surface = SDL_LoadBMP(file_path);
    puts("loading texture");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    //SDL_Color col = {.r=0, .g=0, .b=0, .a=1};
    //texture = createMaskedTexture(renderer, texture, col);
    
    return texture;
}

void score_do_nothing(void* pt1, void* pt2, GameState* state) {}

void update_scoreboard(GameObject* self, SDL_Event* event, GameState* state) {
    puts("update_scoreboard called");
    ScoreBoardData* data = (ScoreBoardData*)(self->special_ptr);
    if (data->displayed_score != state->score) {
        puts("running inner code");
        // TODO: UPDATE SCOREBOARD
        int score = state->score;
        puts("loading small texture from number");
        SDL_Texture* prev_texture = load_asset_from_number(score%10, state->renderer);
        score = score / 10;
        puts("beginning to build texture iteratively");
        while (score != 0) {
            SDL_Texture* new_texture = load_asset_from_number(score%10, state->renderer);
            score = score / 10;
            prev_texture = concatenateTextures(state->renderer, new_texture, prev_texture);
        }
        
        int width1, height1;
        SDL_QueryTexture(prev_texture, NULL, NULL, &width1, &height1);
        self->texture = prev_texture;
        self->rect.w = width1;
        self->rect.h = height1;

        data->displayed_score = state->score;
        puts("inner code finished!");
    }
}


GameObject* create_scoreboard(SDL_Renderer* renderer) {
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);

    SDL_Rect r;
    r.x = WIDTH-100;
    r.y = 15;
    r.w = 35;
    r.h = 35;

    GameObject* object1 = create_game_object(r, texture, &update_scoreboard, &score_do_nothing, NULL, malloc(sizeof(ScoreBoardData))); 
    ScoreBoardData* scoreboarddata = (ScoreBoardData*) object1->special_ptr;
    scoreboarddata->displayed_score = -1;

    return object1;
}