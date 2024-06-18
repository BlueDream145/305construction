/*
** EPITECH PROJECT, 2019
** entry.hpp
** File description:
** entry
*/

#ifndef ENTRY_HPP_
    #define ENTRY_HPP_

    #include <string>
    #include <vector>

    class entry {
        public:
            entry();
            ~entry();

            std::string name;
            std::string description;
            int duration;
            std::vector<std::string> dependencies;
            int mustStart;
            int mustEnd;
            int mustInterval;
    };


#endif /* !ENTRY_HPP_ */