gcc -lSDL_image -o myprogram main.c `sdl2-config --cflags --libs`
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./myprogram > valgrind_logs.txt 2>&1
rm myprogram