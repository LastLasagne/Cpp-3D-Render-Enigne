#pragma once

#ifndef objManager_H
#define objManager_H


#include <vector>
#include "Vertex.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ofFileUtils.h>
#include "C_MeshRenderer.h"

#endif

class OBJManager        //manager used to load "obj" files from txt files
{
    //buffer to write
    std::vector<int> indexBuffer;
	std::vector<Vertex*> vertexBuffer;
    std::vector<Vector3*> normalBuffer;

public:
    C_MeshRenderer* loadOBJ(const std::string& filePath)
    {
        //load file by readin each line and writing the values in the corresponding buffer
        processFileLineByLine(filePath);
        return (new C_MeshRenderer(vertexBuffer, indexBuffer, normalBuffer));
    }

private:
    void processFileLineByLine(const std::string& filePath) {
        //prepare buffers and open file
        indexBuffer.clear();
        vertexBuffer.clear();
        ofFile file;

        file.open(filePath, ofFile::ReadWrite, true);

        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filePath << std::endl;
            return;
        }

        // Read the file line by line
        std::string line;
        while (std::getline(file, line)) {
            if (line[0] == 'v')                                 //vertex positions
            {
                Vector3 pos;
                int posCount = 0;
                int space = 0;
                for (int i = 0; i< line.size(); i++)
                {
                    if (line[i] == ' ' || i == line.size()-1)   //split by  empty spaces between
                    {
                        if (space==0)
                        {
                            space = i;
                        }
                        else
                        {
                            float value = std::stof(line.substr(space, i));
                            switch (posCount)                   //write the x,y and z values
                            {
                            case 0:
                                pos.x = value;
                                break;
                            case 1: 
                                pos.y = value;
                                break;
                            case 2: pos.z = value;
                                break;
                            }
                            space = i;
                            posCount++;
                        }
                    }
                }
                vertexBuffer.push_back(new Vertex(pos));
            }
            if (line[0] == 'f')                                    //indizes
            {
                int space = 0;
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i] == ' ' || i == line.size() - 1)    //split by ' '
                    {
                        if (space == 0)
                        {
                            space = i;
                        }
                        else
                        {
                            int value = std::stof(line.substr(space, i)) -1;
                            indexBuffer.push_back(value);           //write value to buffer
                            space = i;
                        }
                    }
                }
            }
            if (line[0] == 'n')                                     //normals
            {
                Vector3 normal;
                int posCount = 0;
                int space = 0;
                for (int i = 0; i < line.size(); i++)
                {
                    if (line[i] == ' ' || i == line.size() - 1)     //split
                    {
                        if (space == 0)
                        {
                            space = i;
                        }
                        else
                        {
                            float value = std::stof(line.substr(space, i));
                            switch (posCount)                       //write x,y and z value
                            {
                            case 0:
                                normal.x = value;
                                break;
                            case 1:
                                normal.y = value;
                                break;
                            case 2: normal.z = value;
                                break;
                            }
                            space = i;
                            posCount++;
                        }
                    }
                }
                normalBuffer.push_back(new Vector3(normal));
            }
        }
        //reverse the matrizes because they were filled first to last, but are read first to first
        std::reverse(vertexBuffer.begin(), vertexBuffer.end());
        std::reverse(indexBuffer.begin(), indexBuffer.end());
        std::reverse(normalBuffer.begin(), normalBuffer.end());

        // Close the file
        file.close();
    }
};

