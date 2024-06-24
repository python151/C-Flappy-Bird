typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    void (*update)(void*, void*, void*);
    void (*handle_collision)(void*, void*, void*);
    void *special_ptr;
} GameObject;

typedef struct {
    double velocity_x;
    double velocity_y;
} BirdData;

typedef struct {
    GameObject* other_pipe;
    bool is_top;
} PipeData;

typedef struct {
    int displayed_score;
} ScoreBoardData;