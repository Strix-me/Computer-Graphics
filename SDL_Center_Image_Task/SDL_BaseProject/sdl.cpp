#include "sdl.h" // Including custom header file for SDL (assuming it defines some additional functionalities)
#include <SDL.h> // Including SDL header file
#include <stdio.h> // Including standard I/O header file for printf function
#include <SDL_image.h> // Including SDL_image header file for image loading
#include <string> // Including standard C++ string header file
#include <vector> // Including standard C++ vector header file

const int SCREEN_WIDTH = 800; // Defining screen width
const int SCREEN_HEIGHT = 1000; // Defining screen height
const int IMAGE_WIDTH = SCREEN_WIDTH - 300; // Defining image width (one-tenth of screen width)
const int IMAGE_HEIGHT = SCREEN_HEIGHT -500; // Defining image height

SDL_Window* gWindow = NULL; // Pointer to SDL window
SDL_Renderer* gRenderer = NULL; // Pointer to SDL renderer

// Function to load texture from file path
SDL_Texture* loadTexture(const std::string& path) {
    SDL_Texture* newTexture = NULL; // Pointer to the texture to be loaded
    SDL_Surface* loadedSurface = IMG_Load(path.c_str()); // Loading image surface from file path
    if (loadedSurface == NULL) {
        // Error handling if image loading fails
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        // Creating texture from loaded surface
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            // Error handling if texture creation fails
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        // Freeing loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture; // Returning loaded texture
}

// Function to initialize SDL
bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // Error handling if SDL initialization fails
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    // Creating SDL window
    gWindow = SDL_CreateWindow("2D Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        // Error handling if window creation fails
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    // Creating SDL renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == NULL) {
        // Error handling if renderer creation fails
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    // Setting renderer draw color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    // Initializing SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        // Error handling if SDL_image initialization fails
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

// Function to close SDL
void closeSDL() {
    // Destroying renderer and window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    // Quitting SDL_image and SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    // Initializing SDL
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return -1;
    }

    // Loading textures for background
    SDL_Texture* backgroundFloor = loadTexture("./Assets/Chunthesun.jpg");
    SDL_Texture* backgroundSky = loadTexture("./Assets/Chunthewulf.jpg");

    bool quit = false; // Flag for main loop
    SDL_Event e; // SDL event handler

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                // Handling quit event
                quit = true;
            }
        }

        // Clearing renderer
        SDL_RenderClear(gRenderer);

        // Rendering sky texture stacked on top
        SDL_Rect skyDest = { (SCREEN_WIDTH - IMAGE_WIDTH) / 2, 0, IMAGE_WIDTH, IMAGE_HEIGHT };
        SDL_RenderCopy(gRenderer, backgroundSky, NULL, &skyDest);

        // Rendering floor texture stacked below
        SDL_Rect floorDest = { (SCREEN_WIDTH - IMAGE_WIDTH) / 2, IMAGE_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT };
        SDL_RenderCopy(gRenderer, backgroundFloor, NULL, &floorDest);

        // Presenting renderer
        SDL_RenderPresent(gRenderer);
    }

    // Destroying textures and closing SDL
    SDL_DestroyTexture(backgroundFloor);
    SDL_DestroyTexture(backgroundSky);
    closeSDL();

    return 0;
}
