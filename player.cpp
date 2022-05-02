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
    pimpl = new Impl;
    pimpl->history = nullptr;
    if(player_nr == 1 || player_nr == 2){
        pimpl->player_nr = player_nr;
    }else{
        throw player_exception{player_exception::index_out_of_bounds,string{"Impossibile selezionare il player!"}};
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

void Player::load_board(const string& filename){

}

void Player::init_board(const string& filename)const{
    pimpl->history = new Player::Impl::Cell;
    std::ofstream  output{filename};
    if(!output.good())
        throw player_exception{player_exception::missing_file, std::string{"File di output " + filename + " non è scrivibile"}};
    for(int i=0; i<2; i++){
        for(int j=0; j<8; j++){
            pimpl->history->table[i][j] = piece::o;
            output << pimpl->history->table[i][j];
        }
    }

    if(!output.good())
        throw player_exception{player_exception::missing_file, std::string{"Impossibile scrivere sul file di output " + filename}};
    output.close();
    if(output.fail())
        throw player_exception{player_exception::missing_file, std::string{"Impossibile chiudere il file " + filename}};
}

/**
 * Funzioni aggiuntive
 */
int Player::getPlayer_nr(){
    return pimpl->player_nr;
}


int main(){

Player p;
p.init_board("scacchiera.txt");
  
  std::cout << "Tutto apposto bro" << std::endl;
    return 0;
}



