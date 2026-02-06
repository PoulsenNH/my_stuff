/* This file is made for everything that is going to be on the webpage no matter what page that the
user is on, any attempts to modify this file will also modify the entire webpage */

// To make the compiler shut up
#define SDL_MAIN_HANDLED

// For SDL2
#include <SDL2/SDL.h> // How does this code even compile..... (This line has the red line of death)
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

// For the C standard library
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// For Lua functionality
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

// Holds everything that's related to the state of the application
typedef struct {
    SDL_Renderer* renderer;
    lua_State* L;
    int window_w;
    int window_h;
    int top_bar_h;
    int footer_h;
} AppContext;

// Stupid geometry stuff that nobody cares about
typedef struct {
    int x, y, w, h;
    int radius;
    SDL_Color color;
} UIElement;

// The text style stuff
typedef struct {
    const char* font_path;
    int size;
    SDL_Color color;
} TextStyle;

// Functions in scope
int generate_side_bar(AppContext *app);
void new_button(AppContext *app, UIElement ui, const char* text);
void new_text(AppContext *app, TextStyle style, const char* text, int x, int y);
int top_bar(AppContext *app);
int footer(AppContext *app);

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

    // For the structures
    AppContext app;
    app.renderer = renderer;
    app.L = l;
    SDL_GetWindowSize(window, &app.window_w, &app.window_h);
	
// Main loop
int running = 1;

while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = 0;
    }

    // 1. ADD THIS LINE: Set the color back to the background color
    SDL_SetRenderDrawColor(app.renderer, 44, 44, 46, 255); 

        SDL_RenderClear(app.renderer);

        
        app.top_bar_h = top_bar(&app);
        app.footer_h = footer(&app);
        generate_side_bar(&app);

        SDL_RenderPresent(app.renderer);
    }

        // Cleanup after loop
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

int _i = 0; // To be used as an incrimentet

int generate_side_bar(AppContext *app) {
    int button_count = 10; // Initialize with a default size
    const char *button_names[button_count];

    lua_getglobal(app->L, "user_interface");     
    if (!lua_istable(app->L, -1)) {
        printf("user_interface is not a table\n");
        lua_pop(app->L, 1);
        return 1;
    }

    lua_getfield(app->L, -1, "side_menu");       
    if (!lua_istable(app->L, -1)) {
        printf("side_menu is not a table\n");
        lua_pop(app->L, 2);
        return 1;
    }

    lua_getfield(app->L, -1, "button_amount");    
    if (!lua_isinteger(app->L, -1)) {
        printf("button_amount is not an integer\n");
        lua_pop(app->L, 3);
        return 1;
    }

    button_count = (int)lua_tointeger(app->L, -1);
    lua_pop(app->L, 1); // Pop button_amount

    lua_getfield(app->L, -1, "button_names");    
    if (!lua_istable(app->L, -1)) {
        printf("button_names is not a table\n");
        lua_pop(app->L, 2);
        return 1;
    }

    // Gets all of the button names from the table, and assigns it to the button_names array
    for (int i = 1; i <= button_count; i++) {
        lua_rawgeti(app->L, -1, i); // Gets the button names [i]
        if (lua_isstring(app->L, -1)) {
            const char *name = lua_tostring(app->L, -1);
            button_names[i - 1] = name; // Stores it inside of the array
        } else {
            printf("button_names[%d] is not a string\n", i); // Dumb
        }
        lua_pop(app->L, 1); // Pops the string value
    }

    lua_pop(app->L, 3); // pop button_amount, side_menu, user_interface

    // Accessing the height and other properties from AppContext
    int top_padding    = app->top_bar_h + app->window_h * 0.10; // Use app->top_bar_h
    int bottom_padding = app->footer_h + app->window_h * 0.10; // Use app->footer_h

    int usable_height = app->window_h - top_padding - bottom_padding;

    // Total slots = button, gap, repeating + final gap
    int total_slots = button_count * 2 + 1;

    int slot_height = usable_height / total_slots;
    int button_height = slot_height;
    int gap_height = slot_height;

    int y = top_padding + gap_height; // Start after the first gap

    for (int i = 0; i < button_count; i++) {
        UIElement ui = {0, y, app->window_w / 4, button_height, 20, {0, 240, 255, 255}};
        new_button(app, ui, button_names[i]); // Correctly pass the UIElement and button name
        y += button_height + gap_height; // Move down by button + gap
    }
    return 0;
}

// This is the code to ensure that we is able to draw buttons to the screen
void new_button(AppContext *app, UIElement ui, const char* text) {

	// To draw the rectangle
    roundedBoxRGBA(app->renderer,
                   ui.x, ui.y,
                   ui.x + ui.w, ui.y + ui.h,
                   ui.radius,
                   ui.color.r, ui.color.g, ui.color.b, ui.color.a);

    // Render text inside button, scaled to 80% of the button size
    SDL_Color textColor = {255, 255, 255, 255};
    int paddingX = ui.w / 10; // 10% horizontal padding
    int paddingY = ui.h / 10; // 10% vertical padding
    int textX = ui.x + paddingX;
    int textY = ui.y + paddingY;
    int textW = ui.w - 2 * paddingX; // 80% width
    int textH = ui.h - 2 * paddingY; // 80% height

    /* Crude font size calculation based on the width and height
    Auto-scale text to fit inside textW × textH (Scales down until it fits inside of the rectangle because I'm too lazy to program actually good logic for it)*/

    int fontSize = ui.h;  // start large
    TTF_Font* font = NULL;
    int textW_actual, textH_actual;

    while (fontSize > 5) {
        font = TTF_OpenFont("font.ttf", fontSize);
        if (!font) break;

        TTF_SizeText(font, text, &textW_actual, &textH_actual);

        if (textW_actual <= textW && textH_actual <= textH)
            break; // fits

        TTF_CloseFont(font);
        font = NULL;
        fontSize--;
    }

    if (!font) return;

    // Render text
    SDL_Surface* surf = TTF_RenderText_Blended(font, text, textColor);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(app->renderer, surf);

    SDL_Rect dst = {
        ui.x + (ui.w - textW_actual) / 2,
        ui.y + (ui.h - textH_actual) / 2,
        textW_actual,
        textH_actual
    };

    SDL_RenderCopy(app->renderer, tex, NULL, &dst);

    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
}

/* int int(int int) {
	printf("%d\n", int);
	if (int == (int - int)) {
		return int;
	} else return int - (int - (int - int + 1));
} */ 

void new_text(AppContext *app, TextStyle style, const char* text, int x, int y) {
    if (TTF_Init() < 0) {
        printf("TTF_Init error: %s\n", TTF_GetError());
        return;
    }

    TTF_Font* font = TTF_OpenFont(style.font_path, style.size);
    if (!font) {
        printf("Failed to load the font: %s\n", style.font_path);
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text, style.color);
    if (!textSurface) {
        printf("TTF_RenderText_Blended error: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(app->renderer, textSurface);
    if (!textTexture) {
        printf("SDL_CreateTextureFromSurface error: %s\n", SDL_GetError());
    } else {
        SDL_Rect renderQuad;
        SDL_QueryTexture(textTexture, NULL, NULL, &renderQuad.w, &renderQuad.h);
        renderQuad.x = x;
        renderQuad.y = y;

        SDL_RenderCopy(app->renderer, textTexture, NULL, &renderQuad);
        SDL_DestroyTexture(textTexture);
    }

    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
}

int top_bar(AppContext *app) {
    int top_bar_height = app->window_h * 0.1;

    SDL_Rect topBarRect = {0, 0, app->window_w, top_bar_height};
    SDL_SetRenderDrawColor(app->renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(app->renderer, &topBarRect);

    TextStyle style = {"font.ttf", 36, {255, 255, 255, 255}};
    new_text(app, style, "The Quest Academy Cyberpatriots Website",
        (int)(app->window_w / 6),
        (int)(top_bar_height / 2.5));

    return top_bar_height;
}

int footer(AppContext *app) {
    int footer_height = app->window_h * 0.1;

    SDL_Rect footerRect = {0, app->window_h - footer_height, app->window_w, footer_height};
    SDL_SetRenderDrawColor(app->renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(app->renderer, &footerRect);

    TextStyle style = {"font.ttf", 24, {255, 255, 255, 255}};
    new_text(app, style, "© 2026 Puggy Adventures. All rights reserved | Heathp3881@qackids.org",
        (int)(app->window_w / 2.5),
        (int)(app->window_h - footer_height / 1.5));

    return footer_height;
}

int broadcast(AppContext *app, const char message[]) {
    UIElement ui = {app->window_w / 4, app->window_h / 4, app->window_w / 2, app->window_h / 10, 30, {255, 0, 0, 255}};
    new_button(app, ui, "Submit");
    return 0;
}
