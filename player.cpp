#include "player.hpp"

struct Player::Impl{
    int player_nr;
    struct Cell{
        piece table[8][8];
        Cell* next;
        Cell* prev;
    };
    typedef Cell* List;
    
    List history;
};


Player::Player(int player_nr){
    Impl* pimpl;
    pimpl = new Impl;
    pimpl->history = nullptr;
    if(player_nr == 1 || player_nr == 2){
        pimpl->player_nr = player_nr;
    }else{
        player_exception e;
        e.t = e.index_out_of_bounds;
        e.msg = "A questo gioco si gioca in 2!";
        std::cout << e.msg << std::endl;
        throw e;
    }
}

Player::~Player(){
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

Player::piece Player::operator()(int r,int c,int hystory_offset) const{
    piece p;

    if(hystory_offset == 0){
        p = pimpl->history->table[r][c];
    }else{
        int i = 0;
        while(pimpl->history != nullptr || i != hystory_offset){
            p = pimpl->history->table[r][c];
            pimpl->history = pimpl->history->prev;
            i++;
        }
    }
    return p;
}

/**
 * Funzioni aggiuntive
 */
int Player::getPlayer_nr(){
    return pimpl->player_nr;
}


int main(){

    Player p1(2);
    //Player p2(2);

    //p1 = p2;

    //std::cout << p2.getPlayer_nr() << std::endl;
    std::cout << "fine" << std::endl;
    return 0;
}



