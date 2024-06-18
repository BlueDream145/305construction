/*
** EPITECH PROJECT, 2019
** Work
** File description:
** main
*/

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <list>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>

#include "entry.hpp"

std::ifstream makefileStream;
std::ifstream fileStream;
std::vector<entry *> data;
std::vector<entry*> depencies_enabled;
std::vector<std::string> graph;
int pos = 0;
int time_elapsed = 0;

bool isDependencyEnabled(std::string name)
{
    try {
        for(int i = 0; i < depencies_enabled.size(); i++)
            if (depencies_enabled[i]->name == name)
                return (true);
        return (false);
    } catch (...) { exit(0); return (false); }
}

bool isDependenciesEnabled(std::vector<std::string> names)
{
    try {
        for (int i = 0; i < names.size(); i++)
            if (isDependencyEnabled(names[i]) == false)
                return (false);
        return (true);
    } catch (...) { exit(0); return (false); }
}

bool parseLine(std::string line)
{
    try {
        size_t start;
        size_t end = 0;
        int index = 0;
        entry *ent = new entry();

        while ((start = line.find_first_not_of(";", end)) != std::string::npos)
        {
            end = line.find(";", start);
            switch(index) {
                case 0:
                    ent->name = line.substr(start, end - start);
                    break;
                case 1:
                    ent->description = line.substr(start, end - start);
                    break;
                case 2:
                    ent->duration = std::stoi(line.substr(start, end - start));
                    break;
            }
            if (index >= 3) {
                ent->dependencies.push_back(line.substr(start, end - start));
            }
            index++;
        }
        if (index < 3)
            return (false);
        data.push_back(ent);
        return (true);
    } catch (...) { exit(0); return (false); }
}

std::vector<entry *> check_dependency()
{
    std::vector<entry *> entries;
    try {
        if (depencies_enabled.size() == 0) {
            for (int i = 0; i < data.size(); i++)
                if (data[i]->dependencies.size() == 0) {
                    entries.push_back(data[i]);
                    return (entries);
                }
        } else {
            for (int i = 0; i < data.size(); i++) {
                if (isDependencyEnabled(data[i]->name) == true)
                    continue;
                if (isDependenciesEnabled(data[i]->dependencies) == true) {
                    entries.push_back(data[i]);
                }
            }
        }
    } catch (...) { exit(0); return (entries); }
    return (entries);
}

int total_duration()
{
    int total = 0;

    try {
        std::vector<entry *>entries = check_dependency();
        std::vector<entry *>must;

        if (entries.size() == 0)
            return (0);
        while (entries.size() != 0) {
            int t = 0;
            int t2 = 9999;
            for (int i = 0; i < entries.size(); i++) {
                if (entries[i]->dependencies.size() > 1)
                    continue;
                depencies_enabled.push_back(entries[i]);
                t = entries[i]->duration;
                if (must.size() != 0)
                    for (int e = 0; e < entries[i]->dependencies.size(); e++) {
                        if (entries[i]->dependencies[e].compare(must[0]->name)) {
                            must[0]->mustEnd = time_elapsed + t;
                            must[0]->mustInterval = must[0]->mustEnd - must[0]->mustStart - must[0]->duration;
                            must.erase(must.begin());
                                break;
                        }
                    }
            }
            for (int i = 0; i < entries.size(); i++) {
                if (entries[i]->dependencies.size() <= 1)
                    continue;
                depencies_enabled.push_back(entries[i]);
                if (entries[i]->duration < t2 && t == 0) {
                    t2 = entries[i]->duration;
                    if (must.size() != 0)
                        for (int e = 0; e < entries[i]->dependencies.size(); e++) {
                            if (entries[i]->dependencies[e].compare(must[0]->name)) {
                                must[0]->mustEnd = time_elapsed + t2;
                                must[0]->mustInterval = must[0]->mustEnd - must[0]->mustStart - must[0]->duration;
                                must.erase(must.begin());
                                break;
                            }
                        }
                } else {
                    entries[i]->mustStart = time_elapsed;
                    must.push_back(entries[i]);
                }
            }
            if (t != 0)
                time_elapsed += t;
            else {
                time_elapsed += t2;
            }
            entries = check_dependency();
        }
    } catch (...) { exit(0); return (total); }
    return (total);
}

std::vector<entry *> sort(std::vector<entry *> list)
{
    std::vector<entry *> sorted;
    try {
        int minValue;

        while (list.size() != 0) {
            minValue = 999;
            entry *shortest = NULL;
            int index = 0;

            for (int i = 0; i < list.size(); i++) {
                if (list[i]->duration < minValue) {
                    minValue = list[i]->duration;
                    shortest = list[i];
                    index = i;
                } else if (list[i]->duration == minValue) {
                    if (shortest != NULL)
                        if (list[i]->dependencies.size() < shortest->dependencies.size()) {
                            shortest = list[i];
                            index = i;
                        } else
                            continue;
                    else {
                        minValue = list[i]->duration;
                        shortest = list[i];
                        index = i;
                    }
                }
            }
            if (shortest != NULL) {
                sorted.push_back(shortest);
                list.erase(list.begin() + index);
            }
        }
    } catch (...) { exit(0); return (sorted); }

    return (sorted);
}

bool process()
{
    try {
        data = sort(data);
        std::vector<entry *>entries = check_dependency();
        int total = total_duration();

        if (entries.size() == 0)
            return (false);
        std::cout << "Total duration of construction: " << time_elapsed << " weeks" << std::endl << std::endl;
        depencies_enabled.clear();
        time_elapsed = 0;

        while (entries.size() != 0) {
            int t = 0;
            int t2 = 999;

            for (int i = 0; i < entries.size(); i++) {
                if (entries[i]->dependencies.size() > 1)
                    continue;
                depencies_enabled.push_back(entries[i]);
                t = entries[i]->duration;
                std::cout << entries[i]->name << " must begin at t=" << std::to_string(time_elapsed) << std::endl;
                std::string tmp = entries[i]->name + "\t(0)\t";
                for (int e = 0; e < time_elapsed; e++)
                    tmp += " ";
                for (int e = 0; e < entries[i]->duration; e++)
                    tmp += "=";
                graph.push_back(tmp);
            }
            for (int i = 0; i < entries.size(); i++) {
                if (entries[i]->dependencies.size() <= 1)
                    continue;
                std::string tmp;
                depencies_enabled.push_back(entries[i]);
                if (entries[i]->duration < t2 && t == 0) {
                    t2 = entries[i]->duration;
                    std::cout << entries[i]->name << " must begin at t=" << std::to_string(time_elapsed) << std::endl;
                    tmp = entries[i]->name + "\t(0)\t";
                } else {
                    std::cout << entries[i]->name << " must begin between t=" << std::to_string(time_elapsed) << " and t=" << std::to_string(time_elapsed + entries[i]->mustInterval) << std::endl;
                    tmp = entries[i]->name + "\t(" + std::to_string(entries[i]->mustInterval)+ ")\t";
                }
                for (int e = 0; e < time_elapsed; e++)
                    tmp += " ";
                for (int e = 0; e < entries[i]->duration; e++)
                    tmp += "=";
                graph.push_back(tmp);
            }
            if (t != 0)
                time_elapsed += t;
            else
                time_elapsed += t2;
            entries = check_dependency();
        }
        std::cout << std::endl;
        for (int i = 0; i < graph.size(); i++)
            std::cout << graph[i] << std::endl;
        return (true);
    } catch (...) { exit(0); return (84); }
}

int main(int ac, char **av)
{
    std::string line;
    try {
        // Help
        if (ac >= 2 && (!strcmp(av[1], "-h") || !strcmp(av[1], "-help"))) {
            std::cout << "USAGE" << std::endl;
            std::cout << "\t" << av[0] << " file" << std::endl;
            std::cout << "DESCRIPTION" << std::endl;
            std::cout << "\tfile\tfile describing the tasks" << std::endl;
            return (0);
        }
        // ErrorHandling
        if (ac != 2) {
            std::cerr << "Bad usage." << std::endl;
            return (84);
        }
    } catch (...) { return (84); }
    try {
        makefileStream.open(av[1]);
        if (makefileStream.fail()) {
            std::cerr << "Bad input." << std::endl;
            return (84);
        }
        // Parsing
        for (pos = 0; std::getline(makefileStream, line); pos++)
        {
            std::string d(line.c_str());
            if (!parseLine(line)) {
                std::cerr << "Invalid file. line " << pos << std::endl;
                return (84);
            }
        }
        if (data.size() == 0) {
            std::cerr << "Invalid input, no data." << std::endl;
            return (84);
        } 
        if (!process()) {
            std::cerr << "Error during the process of the construction." << std::endl;
            return (84);
        }
    } catch (...) { return (84); }
    return (0);
}