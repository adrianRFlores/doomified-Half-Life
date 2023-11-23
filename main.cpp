#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <sstream>
#include "color.h"
#include "imageloader.h"
#include "raycaster.h"

SDL_Window* window;
SDL_Renderer* renderer;
bool menu = true;

void clear() {
  SDL_SetRenderDrawColor(renderer, 63, 73, 84, 255);
  SDL_RenderClear(renderer);
}

void draw_floor() {
  // floor color
  SDL_SetRenderDrawColor(renderer, 78, 77, 72, 255);
  SDL_Rect rect = {
    0, 
    SCREEN_HEIGHT / 2,
    SCREEN_WIDTH,
    SCREEN_HEIGHT / 2
  };
  SDL_RenderFillRect(renderer, &rect);
}

void draw_ui(const bool currentWeapon, const int direction) {
  if(menu){
    ImageLoader::render(renderer, "menu", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  } else {
    ImageLoader::render(renderer, "bg", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    if(currentWeapon) {
      switch(direction) {
        case 0:
          ImageLoader::render(renderer, "cbarDefault", SCREEN_WIDTH/2.0f + 100.0f, SCREEN_HEIGHT - 252, 123, 252);
          break;
        case 1:
          ImageLoader::render(renderer, "cbarDefaultRight", SCREEN_WIDTH/2.0f + 100.0f, SCREEN_HEIGHT - 252, 123, 252);
          break;
        case 2:
          ImageLoader::render(renderer, "cbarDefaultLeft", SCREEN_WIDTH/2.0f + 100.0f, SCREEN_HEIGHT - 252, 123, 252);
          break;
      }
      ImageLoader::render(renderer, "cbarUI", 10, 10, 230, 94);
    } else {
      switch(direction) {
        case 0:
          ImageLoader::render(renderer, "gunDefault", SCREEN_WIDTH/2.0f + 100.0f, SCREEN_HEIGHT - 193, 200, 193);
          break;
        case 1:
          ImageLoader::render(renderer, "gunDefaultLeft", SCREEN_WIDTH/2.0f + 100.0f, SCREEN_HEIGHT - 193, 200, 193);
          break;
        case 2:
          ImageLoader::render(renderer, "gunDefaultRight", SCREEN_WIDTH/2.0f + 100.0f, SCREEN_HEIGHT - 193, 200, 193);
          break;
      }
      ImageLoader::render(renderer, "gunUI", 10, 10, 230, 94);
    }
  }
  
}

void playFootstep(const std::vector<Mix_Chunk*>& footsteps, int& counter) {
  counter++;
  counter = counter > 3 ? 0 : counter;
  Mix_PlayChannel(-1, footsteps[counter], 0);
}

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);
  Mix_Init(0);
  ImageLoader::init();
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  int num_joysticks = 0;
  auto joysticks = std::vector<SDL_Joystick*>();

  if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
      printf("SDL initialization failed.  Error: %s\n", SDL_GetError());
  }

  num_joysticks = SDL_NumJoysticks();
  for (int i = 0; i < num_joysticks; i++) {
      auto joystick = SDL_JoystickOpen(i);
      if (joystick == nullptr) {
          printf("Warning: unable to open game controller %d!  SDL Error: %s\n", i, SDL_GetError());
      }
      else {
          joysticks.push_back(joystick);
      }
  }

  window = SDL_CreateWindow("DOOM", 50, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_INPUT_GRABBED);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  ImageLoader::loadImage("+", "assets/bm22.png"); //3
  ImageLoader::loadImage("-", "assets/bm11.png");
  ImageLoader::loadImage("|", "assets/bm22.png"); //2
  ImageLoader::loadImage("*", "assets/bm22.png"); //4
  ImageLoader::loadImage("g", "assets/bm44.png"); //5
  ImageLoader::loadImage("bg", "assets/generalHUD.png");
  ImageLoader::loadImage("menu", "assets/menu.png");
  ImageLoader::loadImage("cbarDefault", "assets/defaultCrowbar.png");
  ImageLoader::loadImage("cbarDefaultRight", "assets/defaultCrowbarRight.png");
  ImageLoader::loadImage("cbarDefaultLeft", "assets/defaultCrowbarLeft.png");
  ImageLoader::loadImage("cbarUI", "assets/crowbarUI.png");
  ImageLoader::loadImage("gunDefault", "assets/defaultGun.png");
  ImageLoader::loadImage("gunDefaultRight", "assets/defaultGunRight.png");
  ImageLoader::loadImage("gunDefaultLeft", "assets/defaultGunLeft.png");
  ImageLoader::loadImage("gunUI", "assets/gunUI.png");

  Raycaster r = { renderer };
  r.load_map("assets/map.txt");

  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

  Mix_Music* music = Mix_LoadMUS("assets/hl2_song1.wav");
  std::vector<Mix_Chunk*> footsteps;

  for ( int i = 1; i <= 4; i++) {
    std::string wavName = "assets/concrete" + std::to_string(i) + ".wav";
    footsteps.push_back(Mix_LoadWAV(wavName.c_str()));
  }

  Mix_Chunk* wpnSwitch = Mix_LoadWAV("assets/wpn_hudoff.wav");

  int footstepsCounter = 0;

  Mix_VolumeMusic(20);
  Mix_PlayMusic(music, -1);

  SDL_SetRelativeMouseMode(SDL_TRUE);

  bool currentWeapon = true;
  int stepCounter = 0;

  bool running = true;
  int speed = 10;

  Uint32 frameStart, frameTime;
  float oldX = r.player.x;
  float oldY = r.player.y;

  while(running) {
    frameStart = SDL_GetTicks();
    SDL_Event event;
    int direction = 0;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
        break;
      }
      if (event.type == SDL_KEYDOWN) {
        switch(event.key.keysym.sym ){
          case SDLK_w:
            if(!menu) {
              r.player.x += speed * cos(r.player.a);
              r.player.y += speed * sin(r.player.a);
              if(++stepCounter == 4){
                playFootstep(footsteps, footstepsCounter);
                stepCounter = 0;
              }
            }
            break;
          case SDLK_s:
            if(!menu) {
              r.player.x -= speed * cos(r.player.a);
              r.player.y -= speed * sin(r.player.a);
              if(++stepCounter == 4){
                playFootstep(footsteps, footstepsCounter);
                stepCounter = 0;
              }
            }
            break;
          case SDLK_1:
            if(!currentWeapon && !menu) {
              currentWeapon = true;
              Mix_PlayChannel(-1, wpnSwitch, 0);
            }
            break;
          case SDLK_2:
            if(currentWeapon && !menu) {
              currentWeapon = false;
              Mix_PlayChannel(-1, wpnSwitch, 0);
            }
            break;
          case SDLK_ESCAPE:
            running = false;
            break;
          case SDLK_RETURN:
            menu = false;
            break;
          default:
            break;
        }
      }
      if ( event.type == SDL_MOUSEMOTION && !menu) {
        if(event.motion.xrel < 0) {
          direction = 1;
        } else if ( event.motion.xrel > 0 ) {
          direction = 2;
        }
        r.player.a += event.motion.xrel * 3.141592f/180.0f * -0.5f;
      }
      if (event.type == SDL_JOYAXISMOTION && !menu) {
        switch(event.jaxis.axis) {
            case 2: // joystick
                if (event.jaxis.value > 0) {
                    r.player.a -= M_PI / 5000000 * event.jaxis.value;
                }
                else if (event.jaxis.value < 0) {
                    r.player.a -= M_PI / 5000000 * event.jaxis.value;
                }
                break;
            case 1: // dpad
                if (event.jaxis.value < 0) { // dpad up
                    r.player.x += speed * cos(r.player.a);
                    r.player.y += speed * sin(r.player.a);
                } else if (event.jaxis.value > 0) { // dpad down
                    r.player.x -= speed * cos(r.player.a);
                    r.player.y -= speed * sin(r.player.a);
                }
                break;
        }
      }
    }

    clear();
    draw_floor();

    if(!r.render()) {
      r.player.x = oldX;
      r.player.y = oldY;
      r.render();
    }

    draw_ui(currentWeapon, direction);

    SDL_RenderPresent(renderer);

    frameTime = SDL_GetTicks() - frameStart;

    if (frameTime > 0) {
        std::ostringstream titleStream;
        titleStream << "FPS: " << 1000.0 / frameTime;
        SDL_SetWindowTitle(window, titleStream.str().c_str());
    }

    oldX = r.player.x;
    oldY = r.player.y;
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
}
