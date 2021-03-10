# Connect-Four AI
A Connect 4 game with a one player and two player mode using the SDL library.

There are two versions of the code. The Emscripten (WebAssembly) version uses Emscripten as the compiler. The other version was the original code that compiled through Visual Studio. The difference between the two versions is that the Emscripten version has certain modifications to get it to work such as using emscripten_set_main_loop and a make.sh file for making the compiled output.

To play the game, download the `connect_four.data`, `connect_four.html`, `connect_four.js`, and `connect_four.wasm` files. Open `connect_four.html` in the browser.

> Note:
> 
> Unfortunately several browsers (including Chrome, Safari, and Internet Explorer) do not support file:// XHR requests, and can’t load extra files needed by the HTML (like a .wasm file, or packaged file data as mentioned lower down). For these browsers you’ll need to serve the files using a local webserver and then open http://localhost:8000/connect_four.html) See the [hello-world tutorial](https://emscripten.org/docs/getting_started/Tutorial.html#tutorial) for more details.




Keyboard Controls

------------

o - Switch to 1 player mode. Default mode is 2 players.

q - Quits the game.

c - Clears the board (restarts the game)

------------

1-7 - Drop a chip into the corresponding column

z - Undos previous move (can only be used once per turn)

-----------

8 - Changes the board color to the default blue

9 - Changes the board color to black

0 - Board color modulation option

r - If board color modulation option turned on, modulate the red color

g - If board color modulation option turned on, modulate the green color

b - If board color modulation option turned on, modulate the blue color




