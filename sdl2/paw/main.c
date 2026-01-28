/* This file is made for everything that is going to be on the webpage no matter what page that the
user is on, any attempts to modify this file will also modify the entire webpage */

// For SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL_ttf.h>

// For the C standard library
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// For Lua functionality
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// Functions in scope
int generate_side_bar(lua_State *l);
void new_button(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color, int radius, char text[]);
void new_text(SDL_Renderer *renderer, const char *text, const char *font_path, int font_size, int x, int y, SDL_Color color);

int main() {
	printf("PROGRAM STARTED\n");

	// Initializing Lua
	lua_State *l = luaL_newstate(); // *l is a pointer to the newState structure
	luaL_openlibs(l); // Loads the Lua standard library into the virtual mach+++ine (The line above it)

	if (luaL_dofile(l, "user_interface.lua") != LUA_OK) { 
    	printf("Lua initialization error: %s\n", lua_tostring(l, -1));
    	printf("Lua failed — exiting before render loop.\n");
    	
    	return 1;
	} // Gets the user_interface.lua file, so that we can use it inside of this function
	
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    
    SDL_Window* window = SDL_CreateWindow("Puggy Adventures Website", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        dm.w, dm.h, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI );

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return 1;
    }

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!renderer) {
        printf("Accelerated renderer failed to load: %s\n", SDL_GetError());
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    }

	int rw, rh;
	
	SDL_GetRendererOutputSize(renderer, &rw, &rh);
	printf("Renderer output size: %d x %d\n", rw, rh);

	SDL_Rect vp;
	SDL_RenderGetViewport(renderer, &vp);
	printf("Viewport Display: x=%d y=%d w=%d h=%d\n", vp.x, vp.y, vp.w, vp.h);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // Helps the computer figure out how to do colors well

	lua_getglobal(l, "user_interface"); // Since we cannot directly access the variables from the Lua file, we have to push them into the stack, which is what this line of code does here

	lua_getfield(l, -1, "version"); 
	
	const char *text = lua_tostring(l, -1);

	lua_pop(l, 2); // pop version and user_interface
	
// Main loop
int running = 1;
while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = 0;
    }

    SDL_SetRenderDrawColor(renderer, 44, 44, 46, 255);
    SDL_RenderClear(renderer);

    new_button(renderer, 0, 0,
               width / 8, height / 12, (SDL_Color){0, 240, 255, 255}, 20, "Home");

	new_text(renderer, "Hello World!", "font.ttf", 24, 200, 200, (SDL_Color){255,255,255,255});

    SDL_RenderPresent(renderer);
}

// Cleanup after loop
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();
return 0;

}

int _i = 0; // To be used as an incrimentet

int generate_side_bar(lua_State *l) {
    int button_count;

    lua_getglobal(l, "user_interface");     
    if (!lua_istable(l, -1)) {
        printf("user_interface is not a table\n");
        lua_pop(l, 1);
        return 1;
    }

    lua_getfield(l, -1, "side_menu");       
    if (!lua_istable(l, -1)) {
        printf("side_menu is not a table\n");
        lua_pop(l, 2);
        return 1;
    }

    lua_getfield(l, -1, "button_amount");    
    if (!lua_isinteger(l, -1)) {
        printf("button_amount is not an integer\n");
        lua_pop(l, 3); // Haha anti-memory leak stuff =)
        return 1;
    }

    button_count = (int)lua_tointeger(l, -1);

    lua_pop(l, 3); // pop button_amount, side_menu, user_interface

    for (int i = 0; i <= button_count; i++) {
    	// new_button()
        printf("Button generated %d.\n", i);
    }

    return 0;
}

// This is the code to ensure that we is able to draw buttons to the screen
void new_button(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color, int radius, char text[]) {

	// To draw the rectangle
    roundedBoxRGBA(renderer,
                   x, y,
                   x + w, y + h,
                   radius,
                   color.r, color.g, color.b, color.a);

    // Render text inside button, scaled to 80% of the button size
    SDL_Color textColor = {255, 255, 255, 255};
    int paddingX = w / 10; // 10% horizontal padding
    int paddingY = h / 10; // 10% vertical padding
    int textX = x + paddingX;
    int textY = y + paddingY;
    int textW = w - 2 * paddingX; // 80% width
    int textH = h - 2 * paddingY; // 80% height

    // Crude font size calculation based on the width and height
    int fontSize = (textW * 1.2 + textH) / 5; // weight width more
    new_text(renderer, text, "font.ttf", fontSize, textX, textY, textColor);
}

/* int int(int int) {
	printf("%d\n", int);
	if (int == (int - int)) {
		return int;
	} else return int - (int - (int - int + 1));
} */ 

void new_text(SDL_Renderer *renderer, const char *text, const char *font_path, int font_size, int x, int y, SDL_Color color) {
    if (TTF_Init() < 0) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        return;
    }

    TTF_Font* font = TTF_OpenFont(font_path, font_size);
    if (!font) {
        printf("Failed to load the font: %s\n", font_path, TTF_GetError());
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, color);
    if (!textSurface) {
        printf("TTF_RenderText_Blended error: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
    } else {
        SDL_Rect renderQuad;
        SDL_QueryTexture(textTexture, NULL, NULL, &renderQuad.w, &renderQuad.h);
        renderQuad.x = x;
        renderQuad.y = y;

        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
        SDL_DestroyTexture(textTexture);
    }

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}
