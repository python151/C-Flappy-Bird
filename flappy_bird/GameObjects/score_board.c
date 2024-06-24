#include "../constants_and_includes.h"


// Function to create a texture with only the specific color
// Function to create a texture with only the specific color
SDL_Texture* createMaskedTexture(SDL_Renderer* renderer, SDL_Texture* original, SDL_Color targetColor) {
    int width, height;
    Uint32 format;
    SDL_QueryTexture(original, &format, NULL, &width, &height);

    // Create an empty RGBA texture for the mask
    SDL_Texture* maskTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!maskTexture) {
        fprintf(stderr, "SDL_CreateTexture Error: %s\n", SDL_GetError());
        return NULL;
    }
    
    SDL_SetRenderTarget(renderer, maskTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    void* pixels;
    int pitch;
    SDL_LockTexture(original, NULL, &pixels, &pitch);

    Uint8* pixelArray = (Uint8*)pixels;
    int pixelCount = pitch * height;
    Uint32 targetColorUint = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), targetColor.r, targetColor.g, targetColor.b, targetColor.a);

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(pixelArray, width, height, 8, pitch, SDL_PIXELFORMAT_INDEX8);
    if (!surface) {
        fprintf(stderr, "SDL_CreateRGBSurfaceWithFormatFrom Error: %s\n", SDL_GetError());
        SDL_UnlockTexture(original);
        SDL_DestroyTexture(maskTexture);
        return NULL;
    }

    SDL_Palette* palette = surface->format->palette;

    // Find the index of the target color in the palette
    int targetIndex = -1;
    for (int i = 0; i < palette->ncolors; ++i) {
        SDL_Color color = palette->colors[i];
        if (color.r == targetColor.r && color.g == targetColor.g && color.b == targetColor.b) {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1) {
        fprintf(stderr, "Target color not found in the palette.\n");
        SDL_FreeSurface(surface);
        SDL_UnlockTexture(original);
        SDL_DestroyTexture(maskTexture);
        return NULL;
    }

    // Create a new surface for the mask
    SDL_Surface* maskSurface = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!maskSurface) {
        fprintf(stderr, "SDL_CreateRGBSurface Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        SDL_UnlockTexture(original);
        SDL_DestroyTexture(maskTexture);
        return NULL;
    }

    Uint32* maskPixels = (Uint32*)maskSurface->pixels;

    for (int i = 0; i < pixelCount; ++i) {
        if (pixelArray[i] == targetIndex) {
            maskPixels[i] = targetColorUint;
        } else {
            maskPixels[i] = SDL_MapRGBA(maskSurface->format, 0, 0, 0, 0); // Transparent
        }
    }

    SDL_Texture* resultTexture = SDL_CreateTextureFromSurface(renderer, maskSurface);
    SDL_FreeSurface(maskSurface);
    SDL_FreeSurface(surface);

    SDL_UnlockTexture(original);
    SDL_SetRenderTarget(renderer, NULL);

    return resultTexture;
}


// TODO: THIS
// WARNING: COPIED DIRECTLY FROM CHATGPT AND UNTESTED, NEED TO VERIFY
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
    char file_path[16];
    sprintf(&file_path, "./assets/%d.bmp", num); 

    SDL_Surface* surface = SDL_LoadBMP(file_path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    //SDL_Color col = {.r=0, .g=0, .b=0, .a=1};
    //texture = createMaskedTexture(renderer, texture, col);
    
    return texture;
}

SDL_Texture* constructTextureFromNumber(SDL_Renderer* renderer, int num) {
    // TODO: THIS
}

void score_do_nothing(void* pt1, void* pt2, GameState* state) {}

void update_scoreboard(GameObject* self, SDL_Event* event, GameState* state) {
    ScoreBoardData* data = (ScoreBoardData*)(self->special_ptr);
    if (data->displayed_score != state->score) {
        // TODO: UPDATE SCOREBOARD
        int score = state->score;
        SDL_Texture* prev_texture = load_asset_from_number(score%10, state->renderer);
        score = score / 10;
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
    }
}


GameObject* create_scoreboard(SDL_Renderer* renderer) {
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 50, 50);

    SDL_Rect r;
    r.x = WIDTH-100;
    r.y = 15;
    r.w = 35;
    r.h = 35;

    GameObject* object1 = malloc(sizeof(GameObject));
    object1->rect = r;
    object1->texture = texture;
    object1->update = &update_scoreboard;
    object1->handle_collision = &score_do_nothing;
    object1->special_ptr = malloc(sizeof(ScoreBoardData));

    return object1;
}