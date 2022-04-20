#include "player.hpp"

struct Impl{
    int player;
    struct Cell{
        int info;
        Cell* next;
    };
};

Player::Player(int player_nr = 1){
    pimpl = new Impl;
    pimpl->player = player_nr;
}