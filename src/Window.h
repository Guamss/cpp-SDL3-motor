#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#include <vector>

struct Point {
  float x;
  float y;
  float z;
} typedef Point;

class App {
public:
  App(int w, int h, const char *title);
  ~App();

  void run();

private:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Event event;
  float cam_z = 0.0f;
  bool is_running = true;
  int width = 0, height = 0;
  SDL_Texture *texture = nullptr;
  std::vector<Uint32> pixels;

  void handleEvents();
  void update();
  void appDraw();
  void drawTriangle(Point p1, Point p2, Point p3);
  void handleKeyInput();
  void project(Point &p);
};