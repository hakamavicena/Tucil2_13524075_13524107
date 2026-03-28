#pragma once
#include <SDL2/SDL.h>
#include "types.hpp"
#include <vector>
#include <iostream>
#include <cmath>

    struct Viewer{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    int height= 600;
    int width = 800;

    float fov = 300.0f;
    float anglex = 0.0f;
    float angley = 0.0f;
    float zoom = 1.0f;

    bool init(){
        if(SDL_Init(SDL_INIT_VIDEO) != 0){
            std::cerr << "SDL gagal: " << SDL_GetError() << "\n";
            return false;
            
        }
        window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
        if(window == nullptr) {
            std::cerr << "SDL gagal: " << SDL_GetError() << "\n";
            return false;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer == nullptr){
            std::cerr << "SDL gagal: " << SDL_GetError() << "\n";
            return false;
        }
        return true;
    }

    Vec3 rotatedVertex(Vec3& v){
        Vec3 tmp;
        tmp.x = v.x * cos(angley) + v.z * sin(angley);
        tmp.z = -v.x * sin(angley) + v.z * cos(angley);
        tmp.y = v.y;

        Vec3 rotated;
        rotated.x = tmp.x;
        rotated.y = tmp.y * cos(anglex) - tmp.z * sin(anglex);
        rotated.z = tmp.y * sin(anglex) + tmp.z * cos(anglex);
        return rotated;
    }   

    SDL_Point project(Vec3& v){
        SDL_Point projected;
        float projected_x = v.x * zoom * fov/(fov + v.z);
        float projected_y = v.y * zoom * fov/(fov + v.z);
        projected.x = (int) (projected_x + width/2);
        projected.y = (int) (projected_y + height/2);
        return projected;
    }

    void render(const VoxelMesh &mesh)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (const Triangle &tri : mesh.faces)
        {
            Vec3 vertex1 = mesh.vertices[tri.v[0]];
            Vec3 vertex2 = mesh.vertices[tri.v[1]];
            Vec3 vertex3 = mesh.vertices[tri.v[2]];

            Vec3 rotatedVertex1 = rotatedVertex(vertex1);
            Vec3 rotatedVertex2 = rotatedVertex(vertex2);
            Vec3 rotatedVertex3 = rotatedVertex(vertex3);

            SDL_Point project1 = project(rotatedVertex1);
            SDL_Point project2 = project(rotatedVertex2);
            SDL_Point project3 = project(rotatedVertex3);

            SDL_RenderDrawLine(renderer, project1.x, project1.y, project2.x, project2.y);
            SDL_RenderDrawLine(renderer, project2.x, project2.y, project3.x, project3.y);
            SDL_RenderDrawLine(renderer, project3.x, project3.y, project1.x, project1.y);
        }

        SDL_RenderPresent(renderer);
    }

    void run(const VoxelMesh &mesh)
    {
        bool running = true;
        bool mouseDown = false;
        int lastMouseX = 0, lastMouseY = 0;

        while(running){
            SDL_Event event;

            while(SDL_PollEvent(&event)){
                switch (event.type)
                {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT){
                        mouseDown = true;
                        lastMouseX = event.button.x;
                        lastMouseY = event.button.y;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button ==SDL_BUTTON_LEFT){
                        mouseDown = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if(mouseDown){
                        int dx = event.motion.x - lastMouseX;
                        int dy = event.motion.y - lastMouseY;

                        angley += dx * 0.01;
                        anglex += dy * 0.01;

                        lastMouseX = event.motion.x;
                        lastMouseY = event.motion.y;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if(event.wheel.y > 0) zoom += 0.5f;
                    else if(event.wheel.y < 0)   zoom = std::max(0.1f, zoom - 0.5f);
                    break;

                
                }
            }
            render(mesh);
        }
    }

    void close()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};