#include "APairOfSquares.hpp"

// Globals

// Main game window
SDL_Window* window = NULL;

// Renderer for window
SDL_Renderer* renderer = NULL;

// Spritesheet
SDL_Texture* spritesheet_texture = NULL;
Spritesheet spritesheet;

// Game state
GameState game_state = GameState::MENU_TITLE;

// Input handler
InputHandler input_handler;

// Particle handler
ParticleHandler particle_handler;

// Player
Player player;

// Methods

bool init() {
	// Initialise SDL
	int sdl_init_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_init_result < 0)
	{
		printf("SDL could not initialize!\nSDL Error: %s\n", SDL_GetError());
		return false;
	}

	// Initialise SDL_image
	int img_init_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int img_init_result = IMG_Init(img_init_flags);
	if ((img_init_result & img_init_flags) != img_init_flags) {
		printf("SDL_IMG could not initialize!\nSDL_IMG Error: %s\n", IMG_GetError());
		return false;
	}

	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	// Create window
	window = SDL_CreateWindow(
		APP_TITLE,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	// Create renderer for window
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// Set renderer color
	SDL_SetRenderDrawColor(renderer, 0x03, 0x07, 0x10, 0xFF);

	return true;
}

void quit() {
	// Destroy assets
	clear_data();

	// Destroy renderer
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	// Destroy window
	SDL_DestroyWindow(window);
	window = NULL;

	// Quit SDL
	IMG_Quit();
	SDL_Quit();
}

void load_data() {
	// Load data such as images
	spritesheet_texture = load_texture("F:/Repositories/APairOfSquares/assets/spritesheet.png");

	spritesheet = Spritesheet(renderer, spritesheet_texture, 16, 4);
}

void clear_data() {
	// Free loaded data such as images
	SDL_DestroyTexture(spritesheet_texture);
	spritesheet_texture = NULL;
}

int main(int argc, char* argv[])
{
	// Initialise SDL and globals - if it fails, don't run program
	bool running = init();

	load_data();

	// Main game loop variables
	float dt = 0.0f;
	uint32_t last_time = SDL_GetTicks();
	uint32_t time = 0;

	SDL_Event sdl_event;

	// Update input handler (updates all key states etc)
	input_handler.update();

	// Main game loop
	while (running) {
		// Handle events
		while (SDL_PollEvent(&sdl_event) != 0) {
			if (sdl_event.type == SDL_QUIT) {
				// X (close) is pressed
				running = false;
			}
			else {
				// Delegate to InputHandler
				input_handler.handle_sdl_event(sdl_event);
			}
		}

		// Calculate dt
		time = SDL_GetTicks();
		dt = (time - last_time) / 1000.0f;
		last_time = time;

		update(dt);

		// Clear the screen
		SDL_RenderClear(renderer);

		// Render game
		render();

		// Update screen
		SDL_RenderPresent(renderer);

		// Limit framerate - to fix
		//if (dt < MIN_DT) {
		//	// Wait remaining time
		//	SDL_Delay(1000 * (MIN_DT - dt));
		//}


		//printf("FPS: %f\n", 1.0f / dt);
	}

	// Quit everything
	quit();

	return 0;
}

void update(float dt) {
	switch (game_state) {
	case GameState::MENU_TITLE:
		update_menu_title(dt);
		break;
	default:
		break;
	}
}

void render() {
	switch (game_state) {
	case GameState::MENU_TITLE:
		render_menu_title();
		break;
	default:
		break;
	}

	/*spritesheet.sprite_scaled(0, 16, 16);

	spritesheet.sprite_scaled(4, 48, 16);


	spritesheet.sprite_scaled(80, 16, 32);
	spritesheet.sprite_scaled(81, 32, 32);
	spritesheet.sprite_scaled(82, 48, 32);*/
}

void update_menu_title(float dt) {
	// to remove
	player.update(input_handler, dt);
}

void render_menu_title() {
	// to remove
	player.render(spritesheet);
}

SDL_Texture* load_texture(std::string path) {
	// Load image at specified path
	SDL_Surface* image = IMG_Load(path.c_str());

	if (image == NULL)
	{
		printf("Unable to create texture from %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	// Create texture from image
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	if (texture == NULL)
	{
		printf("Unable to create texture from %s!\nSDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	// Get rid of old loaded surface
	SDL_FreeSurface(image);

	return texture;
}