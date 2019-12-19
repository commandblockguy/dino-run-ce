cmake_minimum_required(VERSION 3.15)
project(dino C)

set(CMAKE_C_STANDARD 90)

include_directories($ENV{CEDEV}/include)

add_custom_target(Make ALL $(MAKE) debug WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# A dummy target to make CLion do stuff
add_executable(dummy
        src/main.c src/sprites.c src/sprites.h src/graphics.c src/graphics.h src/clouds.c src/clouds.h src/config.h src/dino.c src/dino.h src/gamestate.h src/obstacle.c src/obstacle.h src/physics.c src/physics.h src/util.h src/font/font.c src/font/font.h src/lib/steam_controller.c src/lib/steam_controller.h src/usb.c src/usb.h src/score.c src/score.h src/gfx/night.c src/gfx/night.h)
