#include "ChessSaveLoadManager.h"

#include <fstream>
#include <iostream>

ChessSaveLoadManager::ChessSaveLoadManager()
{
}

ChessSaveLoadManager::~ChessSaveLoadManager()
{
}

bool ChessSaveLoadManager::WriteFile(const std::string& fileName, const ChessSaveData gameData)
{
    std::ofstream outFile(fileName);
    
    if (!outFile) {
        std::cerr << "Cannot open or no file." << fileName << std::endl;
        return false;
    }

    for (int i=0;i<32;++i)
    {
        outFile << gameData.units[i].Name << " " << gameData.units[i].Symbol << " " << gameData.units[i].team << " " <<
            gameData.units[i].x << " " << gameData.units[i].y << " " << gameData.units[i].bDead << "\n";
    }
    
    outFile.close();
    return true;
}

bool ChessSaveLoadManager::ReadFile(const std::string& fileName, ChessSaveData& gameData)
{
    std::ifstream inFile(fileName);
    if (!inFile) {
        std::cerr << "Cannot open or no file." << fileName << std::endl;
        return false;
    }
    for (int i=0;i<32;++i)
    {
        inFile >> gameData.units[i].Name;
        inFile >> gameData.units[i].Symbol;
        inFile >> gameData.units[i].team;
        inFile >> gameData.units[i].x;
        inFile >> gameData.units[i].y;
        inFile >> gameData.units[i].bDead;
    }
    
    inFile.close();
    return true;
    
}
