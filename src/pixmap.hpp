#pragma once

#include "common.hpp"

#include "point.hpp"
#include "size.hpp"

typedef std::unique_ptr<SDL_Texture, SDL_Deleter> texture_ptr;

namespace graphics {
enum class flip : int32_t {
  none = SDL_FLIP_NONE,
  horizontal = SDL_FLIP_HORIZONTAL,
  vertical = SDL_FLIP_VERTICAL,
  both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
};

class pixmap {
public:
  pixmap(const std::shared_ptr<renderer> renderer, std::string_view filename);
  ~pixmap() = default;

  void draw(const geometry::point &point, const double angle = 0.0, const uint8_t alpha = 255) const;

  const geometry::size size() const;

  void set_size(const geometry::size &size);

private:
  std::shared_ptr<renderer> _renderer;

  geometry::size _size;
  texture_ptr _texture;
};
}
