typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    void (*update)(void*, void*);
    void (*handle_collision)(void*, void*);
} GameObject;