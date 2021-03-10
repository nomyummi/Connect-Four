# Not actually a makefile
../emcc -O2 ./code/AI.cpp -c -o AI.o
../emcc -O2 ./code/Board.cpp -c -o Board.o
../emcc -O2 ./code/Chip.cpp -c -o Chip.o
../emcc -O2 ./code/Game.cpp -c -o Game.o
../emcc -O2 ./code/Texture.cpp -c -o Texture.o
../emcc -O2 ./code/Tile.cpp -c -o Tile.o
../emcc -O2 ./code/main.cpp -c -o main.o
../emcc -O2 AI.o Board.o Chip.o Game.o Texture.o Tile.o main.o -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -o connect_four.html