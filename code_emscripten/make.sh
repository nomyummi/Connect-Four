../emcc -O2 ./code/AI.cpp -c -o AI.o
../emcc -O2 ./code/Board.cpp -c -o Board.o
../emcc -O2 ./code/Chip.cpp -c -o Chip.o
../emcc -O2 ./code/Game.cpp -c -o Game.o
../emcc -O2 ./code/Texture.cpp -c -o Texture.o
../emcc -O2 ./code/Title.cpp -c -o Title.o
../emcc -O2 ./code/OnePlayerMode.cpp -c -o OnePlayerMode.o
../emcc -O2 ./code/main.cpp -c -o main.o
# ../emcc -O2 --emrun AI.o Board.o Chip.o Game.o Texture.o Title.o OnePlayerMode.o main.o -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file images -o connect_four.html
../emcc -O2  AI.o Board.o Chip.o Game.o Texture.o Title.o OnePlayerMode.o main.o -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s TOTAL_MEMORY=327680000 --preload-file images -o connect_four.html
# -fsanitize=address -s INITIAL_MEMORY=327680000
# Debug using printfs?