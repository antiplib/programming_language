//
// Created by Владислав Отвагин on 24.10.2024.
//
#include "solve.cpp"


int main(int argc, char* argv[]) {
    std::string s = *argv;
    for(int i = 0; i < 18+21; ++i) {
        s.erase(s.end() -1);
    }

    project = s;

    solve();
    return 0;
}

