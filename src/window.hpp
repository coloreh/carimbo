#pragma once

#include "common.hpp"

namespace graphics {
class window {
public:
  window(std::string_view title, int32_t width, int32_t height, bool fullscreen);
  ~window() = default;

  operator SDL_Window *();

  const std::shared_ptr<renderer> create_renderer() const;

  int32_t width() const;

  int32_t height() const;

private:
  int32_t _width;
  int32_t _height;

  std::unique_ptr<SDL_Window, SDL_Deleter> _window;
};
}
