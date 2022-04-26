#include "player.hpp"

using namespace std;

struct Player::Impl{
    int player_nr;
    struct Cell{
        int info;
        Cell* next;
    };
    Cell* history;
};


Player::Player(int player_nr = 1){
    Impl* pimpl;
    pimpl = new Impl;
    pimpl->history = nullptr;
    pimpl->player_nr = player_nr;   
}

Player::~Player(){
    delete pimpl->history;
    delete pimpl;
}

Player::Player(const Player& copy){
    pimpl = new Impl;
    pimpl->player_nr = copy.pimpl->player_nr;
    pimpl->history = copy.pimpl->history;
}

Player& Player::operator=(const Player& copy){
    pimpl->player_nr = copy.pimpl->player_nr;
    pimpl->history = copy.pimpl->history;
    return *this;
}

