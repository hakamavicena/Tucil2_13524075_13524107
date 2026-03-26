#pragma once
#include <iostream>
#include <fstream>
#include <limits>
#include "types.hpp"


std::pair<std::string, int> inputObj()
{
    bool pass = false;
    std::string path;
    int maxDepth;
    while (!pass)
    {
        pass = true;

        std::cout << "Masukkan path obj file Anda: " << std::endl;
        std::cin >> path;
        std::ifstream f(path);
        if (f.good())
        {
            if (path.substr(path.size() - 4, 4).compare(".obj") != 0)
            {
                pass = false;
            }
        }
        else
        {
            pass = false;
        }

        if (!pass)
            std::cout << "Masukkan path yang valid\n";
    }
    pass = false;
    while (!pass)
    {
        std::cout << "Masukkan tingkat kedalaman pohon maksimum : " << std::endl;
        std::cin >> maxDepth;
        if (std::cin.fail())
        {
            std::cout << "Masukkan tingkat kedalaman yang valid\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            pass = false;
        }
        else if (maxDepth < 1)
        {
            std::cout << "Kedalaman minimal adalah 1!\n";
            pass = false;
        }
        else 
        {
            pass = true;
        }
    }

    return std::make_pair(path, maxDepth);
}
