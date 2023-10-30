#include "application.hpp"

application::application(int argc, char **argv) {
  curl_global_init(CURL_GLOBAL_ALL);
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK);
  PHYSFS_init(argv[0]);
}

int application::run() {
  try {
    filesystem::mount("bundle.zip", "/");

    auto ss = scriptengine();
    ss.run();
    // const auto engine = engine::create();
    // engine->init("Carimbo", 640, 480, false);
    // engine->run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}

application::~application() {
  PHYSFS_deinit();
  SDL_Quit();
  curl_global_cleanup();
}