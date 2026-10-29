#include "Window.h"
#include "Misc.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <algorithm>

App::App(int w, int h, const char *title) {
  width = w;
  height = h;

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Initialization failed! : %s\n", SDL_GetError());
  }

  window = SDL_CreateWindow(title, width, height, SDL_WINDOW_HIDDEN);

  if (window == nullptr) {
    SDL_Log("Window creation failed! : %s\n", SDL_GetError());
  }

  renderer = SDL_CreateRenderer(window, nullptr);

  if (renderer == nullptr) {
    SDL_Log("Renderer creation failed : %s\n", SDL_GetError());
  }

  SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                              SDL_TEXTUREACCESS_STREAMING, width, height);
  if (texture == nullptr) {
    SDL_Log("Texture creation failed : %s\n", SDL_GetError());
  }
  pixels.resize(width * height);

  SDL_ShowWindow(window);
}

App::~App() {
  if (window) {
    SDL_DestroyWindow(window);
    window = nullptr;
  }

  if (renderer) {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
  }

  if (texture) {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }

  SDL_Quit();
  printf("App destroyed !\n");
}

void App::run() {
  SDL_zero(event);
  int fps = 0;
  Uint64 lastTick = 0;
  while (is_running) {
    Uint64 currentTick = SDL_GetTicks();
    handleEvents();
    appDraw();
    fps++;
    if (currentTick > lastTick + 1000) {
      lastTick = currentTick;
      printf("FPS: %d\n", fps);
      fps = 0;
    }
  }
}

void App::handleKeyInput() {
  if (event.key.key == SDLK_S) {
    cam_z -= 0.5;
  } else if (event.key.key == SDLK_Z) {
    cam_z += 0.5;
  }
}

void App::handleEvents() {
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      is_running = false;
    } else if (event.type == SDL_EVENT_KEY_DOWN) {
      handleKeyInput();
    }
  }
}

void App::drawTriangle(Point p1, Point p2, Point p3) {
  int min_x = std::min({p1.x, p2.x, p3.x});
  int min_y = std::min({p1.y, p2.y, p3.y});
  int max_x = std::max({p1.x, p2.x, p3.x});
  int max_y = std::max({p1.y, p2.y, p3.y});

  // limiter l'affichage à la taille de la fenêtre
  min_x = std::max(min_x, 0);
  min_y = std::max(min_y, 0);
  max_x = std::min(max_x, width - 1);
  max_y = std::min(max_y, height - 1);

  float total_area =
      signed_triangle_area((float)p1.x, (float)p1.y, (float)p2.x, (float)p2.y,
                           (float)p3.x, (float)p3.y);

  for (int x = min_x; x <= max_x; x++) {
    for (int y = min_y; y <= max_y; y++) {
      float alpha =
          signed_triangle_area(x, y, p2.x, p2.y, p3.x, p3.y) / total_area;
      float beta =
          signed_triangle_area(p1.x, p1.y, x, y, p3.x, p3.y) / total_area;
      float gamma =
          signed_triangle_area(p1.x, p1.y, p2.x, p2.y, x, y) / total_area;

      if (alpha >= 0 && beta >= 0 && gamma >= 0) {
        Uint8 r = (Uint8)(alpha * 255);
        Uint8 g = (Uint8)(beta * 255);
        Uint8 b = (Uint8)(gamma * 255);
        pixels[y * width + x] = (255u << 24) | (r << 16) | (g << 8) | b;
      }
    }
  }
}

void App::project(Point &p) {
  float d = 1.0f;
  float z = p.z - cam_z;
  p.x = (int)(width / 2.0f + (p.x * d / z) * (height / 2.0f));
  p.y = (int)(height / 2.0f - (p.y * d / z) * (height / 2.0f));
}

void App::appDraw() {
  std::fill(pixels.begin(), pixels.end(),
            (255u << 24) | (20 << 16) | (20 << 8) | 30);

  Point a = {-1.0f, -1.0f, 3.0f};
  Point b = {1.0f, -1.0f, 3.0f};
  Point c = {0.0f, 1.0f, 3.0f};

  project(a);
  project(b);
  project(c);

  drawTriangle(a, b, c);

  SDL_UpdateTexture(texture, nullptr, pixels.data(), width * sizeof(Uint32));
  SDL_RenderTexture(renderer, texture, nullptr, nullptr);
  SDL_RenderPresent(renderer);
}