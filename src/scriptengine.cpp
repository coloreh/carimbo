#include "scriptengine.hpp"

#include "delay.hpp"
#include "engine.hpp"
#include "enginefactory.hpp"
#include "entity.hpp"
#include "event.hpp"
#include "eventreceiver.hpp"
#include "io.hpp"
#include "noncopyable.hpp"
#include "pixmap.hpp"
#include "point.hpp"
#include "soundmanager.hpp"
#include "statemanager.hpp"
#include "ticks.hpp"

using namespace framework;

void scriptengine::run() {
  sol::state lua;

  lua.open_libraries();

  lua.new_enum(
      "KeyEvent",
      "w", input::keyevent::w,
      "a", input::keyevent::a,
      "s", input::keyevent::s,
      "d", input::keyevent::d,
      "space", input::keyevent::space);

  lua.new_usertype<geometry::point>(
      "Point",
      sol::constructors<geometry::point(int32_t, int32_t)>(),
      "x", sol::property(&geometry::point::x, &geometry::point::set_x),
      "y", sol::property(&geometry::point::y, &geometry::point::set_y));

  lua.new_usertype<enginefactory>(
      "EngineFactory",
      sol::constructors<enginefactory()>(),
      "set_title", &enginefactory::set_title,
      "set_width", &enginefactory::set_width,
      "set_height", &enginefactory::set_height,
      "set_fullscreen", &enginefactory::set_fullscreen,
      "create", &enginefactory::create);

  lua.new_usertype<engine>(
      "Engine",
      "run", &engine::run,
      "spawn", &engine::spawn,
      "destroy", &engine::destroy,
      "ticks", &ticks,
      "is_keydown", &engine::is_keydown,
      "width", sol::property(&engine::width),
      "height", sol::property(&engine::height),
      "prefetch", [](engine &engine, sol::table table) {
        std::vector<std::string> filenames{table.size()};
        for (auto &item : table) {
          filenames.push_back(item.second.as<std::string>());
        }
        engine.prefetch(filenames);
      });

  lua.new_usertype<entity>(
      "Entity",
      "id", sol::property(&entity::id),
      "x", sol::property(&entity::x, &entity::set_x),
      "y", sol::property(&entity::y, &entity::set_y),
      "width", sol::property(&entity::width),
      "height", sol::property(&entity::height),
      "move", &entity::move,
      "scale", &entity::scale,
      "angle", sol::property(&entity::angle, &entity::set_angle),
      "alpha", sol::property(&entity::alpha, &entity::set_alpha),
      "pixmap", sol::property(&entity::set_pixmap),
      "play", &entity::play_sound,
      "on_update", &entity::set_onupdate);

  lua.set_function("sleep", &sleep);

  const auto script = storage::io::read("scripts/main.lua");
  lua.script(std::string_view(reinterpret_cast<const char *>(script.data()), script.size()));
}