#pragma once
#include <iostream>
#include <fstream>
#include <limits>
#include "types.hpp"
#include <string>
#include <filesystem>
#include <cstdio>
namespace fs = std::filesystem;

void objWriter(VoxelMesh res, const std::string &input_path)
{
    fs::path p(input_path);
    std::string directory = p.parent_path().string();
    std::string stem = p.stem().string();
    std::string output_filename = stem + "_voxelized.obj";
    std::string output_path = directory.empty() ? output_filename : directory + "/" + output_filename;

    std::ofstream output_file(output_path);
    if (!output_file.is_open())
    {
        std::cerr << "Error: Could not open the file." << std::endl;
        return; 
    }

    for(int i = 0; i < res.vertices.size(); i++){
        output_file << "v " << res.vertices[i].x << ' ' << res.vertices[i].y << ' ' << res.vertices[i].z << std::endl;
    }

    for (int i = 0; i < res.faces.size(); i++)
    {
        output_file << "f " << res.faces[i].v[0] << ' ' << res.faces[i].v[1] << ' ' << res.faces[i].v[2] << std::endl;
    }

    output_file.close();
    std::cout << "Output disimpan di: " << output_path << std::endl;
}