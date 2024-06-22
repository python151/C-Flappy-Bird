typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    void (*update)(void*);
} GameObject;