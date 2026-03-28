#pragma once
#include <SDL.h>
#include "types.hpp"
#include <vector>
#include <iostream>
#include <cmath>

struct Viewer {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    int height = 600;
    int width = 800;

    float fov = 300.0f;
    float anglex = 0.0f;
    float angley = 0.0f;
    float zoom = 1.0f;
    Vec3 centerOffset = {0.0f, 0.0f, 0.0f};

    bool init() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL gagal: " << SDL_GetError() << "\n";
            return false;
        }
        window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
        if (window == nullptr) {
            std::cerr << "SDL gagal: " << SDL_GetError() << "\n";
            return false;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            std::cerr << "SDL gagal: " << SDL_GetError() << "\n";
            return false;
        }
        return true;
    }

    void autofit(const VoxelMesh& mesh) {
        if (mesh.vertices.empty()) return;
        Vec3 mn = mesh.vertices[0], mx = mesh.vertices[0];
        for (const Vec3& v : mesh.vertices) {
            if (v.x < mn.x) mn.x = v.x; if (v.x > mx.x) mx.x = v.x;
            if (v.y < mn.y) mn.y = v.y; if (v.y > mx.y) mx.y = v.y;
            if (v.z < mn.z) mn.z = v.z; if (v.z > mx.z) mx.z = v.z;
        }
        centerOffset.x = (mn.x + mx.x) * 0.5f;
        centerOffset.y = (mn.y + mx.y) * 0.5f;
        centerOffset.z = (mn.z + mx.z) * 0.5f;
        float maxSpan = std::max({mx.x - mn.x, mx.y - mn.y, mx.z - mn.z});
        if (maxSpan > 0.0f)
            zoom = (std::min(width, height) * 0.7f) / maxSpan;
    }

    Vec3 rotatedVertex(Vec3& v) {
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

    SDL_Point project(Vec3& v) {
        SDL_Point projected;
        float projected_x = v.x * zoom * fov / (fov + v.z);
        float projected_y = v.y * zoom * fov / (fov + v.z);
        projected.x = (int)(projected_x + width / 2);
        projected.y = (int)(projected_y + height / 2);
        return projected;
    }

    void render(const VoxelMesh& mesh) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (const Triangle& tri : mesh.faces) {
            Vec3 v1 = mesh.vertices[tri.v[0] - 1];
            Vec3 v2 = mesh.vertices[tri.v[1] - 1];
            Vec3 v3 = mesh.vertices[tri.v[2] - 1];

            Vec3 c1 = {v1.x - centerOffset.x, v1.y - centerOffset.y, v1.z - centerOffset.z};
            Vec3 c2 = {v2.x - centerOffset.x, v2.y - centerOffset.y, v2.z - centerOffset.z};
            Vec3 c3 = {v3.x - centerOffset.x, v3.y - centerOffset.y, v3.z - centerOffset.z};

            Vec3 r1 = rotatedVertex(c1);
            Vec3 r2 = rotatedVertex(c2);
            Vec3 r3 = rotatedVertex(c3);

            SDL_Point p1 = project(r1);
            SDL_Point p2 = project(r2);
            SDL_Point p3 = project(r3);

            SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
            SDL_RenderDrawLine(renderer, p2.x, p2.y, p3.x, p3.y);
            SDL_RenderDrawLine(renderer, p3.x, p3.y, p1.x, p1.y);
        }

        SDL_RenderPresent(renderer);
    }

    void run(const VoxelMesh& mesh) {
        bool running = true;
        bool mouseDown = false;
        int lastMouseX = 0, lastMouseY = 0;

        autofit(mesh);
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouseDown = true;
                        lastMouseX = event.button.x;
                        lastMouseY = event.button.y;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        mouseDown = false;
                    break;
                case SDL_MOUSEMOTION:
                    if (mouseDown) {
                        int dx = event.motion.x - lastMouseX;
                        int dy = event.motion.y - lastMouseY;
                        angley += dx * 0.01f;
                        anglex += dy * 0.01f;
                        lastMouseX = event.motion.x;
                        lastMouseY = event.motion.y;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0) zoom += zoom * 0.1f;
                    else if (event.wheel.y < 0) zoom = std::max(0.1f, zoom - zoom * 0.1f);
                    break;
                }
            }
            render(mesh);
            SDL_Delay(16);
        }
    }

    void close() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};