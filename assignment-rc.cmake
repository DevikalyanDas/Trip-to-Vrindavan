cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
)

set(ASSIGNMENT_SOURCES
    ${MY_ADDITIONAL_SOURCES}
    main/a_scene.cpp
)

file(COPY
    models/tex_mountain_bump.png
    models/tex_stone_bump.png
    models/tex_boat_inside_bump.png
    models/tex_boat_out_bump.png
    models/tex_boat_up_bump.png
    models/tex_lotus_bump.png
    models/tex_grass.png
    models/blue_sky.png
    models/tex_bird.png
    models/tex_boat_inside.png
    models/tex_boat_out.png
    models/tex_boat_up.png
    models/tex_stones.png
    models/tex_riverbed.png
    models/tex_flower.png
    models/scene_new_cp.obj
    models/scene_new_cp.mtl
    DESTINATION ${CMAKE_BINARY_DIR}/models
)
