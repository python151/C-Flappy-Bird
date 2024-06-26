typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
    void (*update)(void*, void*, void*);
    void (*handle_collision)(void*, void*, void*);
    char* type_signature;
    void *special_ptr;
} GameObject;

typedef struct {
    double velocity_x;
    double velocity_y;
} BirdData;

typedef struct {
    GameObject* other_pipe;
    GameObject* reward_collider;
    bool is_top;
} PipeData;

typedef struct {
    bool is_active;
} RewardPipeColliderData;

typedef struct {
    int displayed_score;
} ScoreBoardData;

GameObject* create_game_object(SDL_Rect rect, 
    SDL_Texture* texture, 
    void (*update)(void*, void*, void*), 
    void (*handle_collision)(void*, void*, void*),
    char* type_signature,
    void* special_ptr) {

    GameObject* object1 = malloc(sizeof(GameObject));

    object1->rect = rect;
    object1->texture = texture;
    object1->update = update;
    object1->handle_collision = handle_collision;
    object1->type_signature = type_signature;
    object1->special_ptr = special_ptr;

    return object1;
}

void destroy_game_object(GameObject* ptr) {
    free(ptr->type_signature);
    free(ptr->special_ptr);
    free(ptr);
}