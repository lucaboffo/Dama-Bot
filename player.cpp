#include "player.hpp"

struct Player::Impl{
    int player_nr;
    struct Cell{
        piece table[8][8];
        Cell* next;
    };
    typedef Cell* List;
    
    List history;

    void prepend(List& t){
        List nuova;
        nuova = new Cell;
        nuova->next = t;
        t = nuova;    
    }
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

Player::piece Player::operator()(int r,int c,int history_offset) const{
    piece p;
    bool flag = false;
    Impl::List pCell = pimpl->history;
    if( r > 8 || r < 0 || c > 8 || c < 0 || history_offset < 0){
        throw player_exception{player_exception::index_out_of_bounds,string{"Parametri non presenti in memoria!"}};
    }else{
       int sum = 0;
       while(pCell != nullptr){
           if(sum == history_offset){
               p = pCell->table[r][c];
           }
           sum++;
           pCell = pCell->next;
       }
       if(history_offset >= sum){
           throw player_exception{player_exception::index_out_of_bounds,string{"Parametri non presenti in memoria!"}};
           
       }
    }
    
    return p;
}

void Player::load_board(const string& filename){
    pimpl->prepend(pimpl->history);
    std::ifstream input{filename};
    if(!input.good()){
        throw player_exception{player_exception::missing_file,string{"Errore nel file di input " + filename}};
    }
    for(int i=0; i<8; i++){
        int j = 0;
        while(j < 8){
            char pezzo;
            input >> std::noskipws >> pezzo;
            if(pezzo != '\n'){
                switch(pezzo){
                    case 'o':
                        pimpl->history->table[i][j] = piece::o;
                        break;
                    case 'O':
                        pimpl->history->table[i][j] = piece::O;
                        break;
                    case 'x':
                        pimpl->history->table[i][j] = piece::x;
                        break;
                    case 'X':
                        pimpl->history->table[i][j] = piece::X;
                        break;
                    case ' ':
                        pimpl->history->table[i][j] = piece::e;
                        break;
                    default:
                        throw player_exception{player_exception::invalid_board,string{"Scacchiera non valida!"}};
                }
                j++;
            }
        }
    }
}

void Player::store_board(const string& filename, int history_offset)const{
    std::ofstream output{filename};
    if(!output.good()){
         throw player_exception{player_exception::missing_file,string{"File di output " + filename + " non è scrivibile"}};
    }
    int sum = 0;
    while(pimpl->history != nullptr){
        if(sum == history_offset){
            for(int i=0; i<8; i++){
                for(int j=0; j<8; j++){
                    switch(pimpl->history->table[i][j]){
                        case piece::x:
                        output << 'x';
                        break;
                        case piece::X:
                        output << 'X';
                        break;
                        case piece::o:
                        output << 'o';
                        break;
                        case piece::O:
                        output << 'O';
                        break;
                        case piece::e:
                        output << ' ';
                        break;
                        default:
                            throw player_exception{player_exception::invalid_board,string{"Scacchiera non valida!"}};
                    }
                }
                if(i < 7){
                    output << "\n";
                }
            }
        }
        sum++;
        pimpl->history = pimpl->history->next;
    }
    if(history_offset >= sum){
           throw player_exception{player_exception::index_out_of_bounds,string{"Parametri non presenti in memoria!"}};
           
       }
    
    if(!output.good())
        throw player_exception{player_exception::missing_file,string{"Impossibile scrivere sul file di output " + filename}};
    output.close();
    if(output.fail())
        throw player_exception{player_exception::missing_file,string{"Impossibile chiudere il file " + filename}};
}

void Player::init_board(const string& filename)const{
    pimpl->prepend(pimpl->history);
    std::ofstream  output{filename};
    if(!output.good())
        throw player_exception{player_exception::missing_file,string{"File di output " + filename + " non è scrivibile"}};
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(i == 0 || i == 2){
                if(j%2 == 0){
                    pimpl->history->table[i][j] = piece::o;
                    output << 'o';
                }else{
                    pimpl->history->table[i][j] = piece::e;
                    output << ' ';
                }
            }else if(i == 1){
                if(j%2 == 1){
                    pimpl->history->table[i][j] = piece::o;
                    output << 'o';
                }else{
                    pimpl->history->table[i][j] = piece::e;
                    output << ' ';
                }
            }else if(i == 5 || i == 7){
                if(j%2 == 1){
                    pimpl->history->table[i][j] = piece::x;
                    output << 'x';
                }else{
                    pimpl->history->table[i][j] = piece::e;
                    output << ' ';
                }
            }else if(i == 6){
                if(j%2 == 0){
                    pimpl->history->table[i][j] = piece::x;
                    output << 'x';
                }else{
                    pimpl->history->table[i][j] = piece::e;
                    output << ' ';
                }
            }else{
                pimpl->history->table[i][j] = piece::e;
                output << ' ';
            } 
        }
        if(i < 7){
            output << "\n";
        }
    }

    if(!output.good())
        throw player_exception{player_exception::missing_file,string{"Impossibile scrivere sul file di output " + filename}};
    output.close();
    if(output.fail())
        throw player_exception{player_exception::missing_file,string{"Impossibile chiudere il file " + filename}};
}

void Player::move(){
    bool mosso = false;
    pimpl->prepend(pimpl->history);
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            pimpl->history->table[i][j] = pimpl->history->next->table[i][j];
        }
    }

    if(pimpl->player_nr == 1){
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                if(i == 1){
                    if(j == 0){
                        if(pimpl->history->table[i][j] == piece::x){
                            if(pimpl->history->table[i-1][j+1] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::X;
                                mosso = true;
                            }
                        }
                    }else if(j == 7){
                        if(pimpl->history->table[i][j] == piece::x){
                            if(pimpl->history->table[i-1][j-1] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::X;
                                mosso = true;
                            }
                        }
                    }else{
                        if(pimpl->history->table[i][j] == piece::x){
                            if(pimpl->history->table[i-1][j-1] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j+1] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::X;
                                mosso = true;
                            }
                        }
                    }
                }else if(i == 0){
                    if(j == 0){
                        if(pimpl->history->table[i][j] == piece::X){
                            if(pimpl->history->table[i+1][j+1] == piece::O && pimpl->history->table[i+2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j+1] = piece::e;
                                pimpl->history->table[i+2][j+2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j+1] == piece::o && pimpl->history->table[i+2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j+1] = piece::e;
                                pimpl->history->table[i+2][j+2] = piece::X;
                                mosso = true;
                            }
                        }
                    }else if(j == 7){
                        if(pimpl->history->table[i][j] == piece::X){
                            if(pimpl->history->table[i+1][j-1] == piece::O && pimpl->history->table[i+2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j-1] = piece::e;
                                pimpl->history->table[i+2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j-1] == piece::o && pimpl->history->table[i+2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j-1] = piece::e;
                                pimpl->history->table[i+2][j-2] = piece::X;
                                mosso = true; 
                            }
                        }
                    }else{
                        if(pimpl->history->table[i][j] == piece::X){
                            if(pimpl->history->table[i+1][j-1] == piece::O && pimpl->history->table[i+2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j-1] = piece::e;
                                pimpl->history->table[i+2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j+1] == piece::O && pimpl->history->table[i+2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j+1] = piece::e;
                                pimpl->history->table[i+2][j+2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j-1] == piece::o && pimpl->history->table[i+2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j-1] = piece::e;
                                pimpl->history->table[i+2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j+1] == piece::o && pimpl->history->table[i+2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j+1] = piece::e;
                                pimpl->history->table[i+2][j+2] = piece::X;
                                mosso = true;
                            }
                        }
                    }
                }else if(i == 7){
                     if(j == 0){
                        if(pimpl->history->table[i][j] == piece::X){
                            if(pimpl->history->table[i-1][j+1] == piece::O && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j+1] == piece::o && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::X;
                                mosso = true;
                            }
                        }else if(pimpl->history->table[i][j] == piece::x){
                            if(pimpl->history->table[i-1][j+1] == piece::o && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::x;
                                mosso = true;
                            }
                        }
                    }else if(j == 7){
                        if(pimpl->history->table[i][j] == piece::X){
                            if(pimpl->history->table[i-1][j-1] == piece::O && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j-1] == piece::o && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::X;
                                mosso = true;
                            }
                        }else if(pimpl->history->table[i][j] == piece::x){
                            if(pimpl->history->table[i-1][j-1] == piece::o && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::x;
                                mosso = true; 
                            }
                        }
                    }else{
                        if(pimpl->history->table[i][j] == piece::X){
                            if(pimpl->history->table[i-1][j-1] == piece::O && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j+1] == piece::O && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j-1] == piece::o && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::X; 
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j+1] == piece::o && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::X; 
                                mosso = true;
                            }
                        }else if(pimpl->history->table[i][j] == piece::x){
                            if(pimpl->history->table[i-1][j-1] == piece::o && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::x; 
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j+1] == piece::o && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::x; 
                                mosso = true;
                            }
                        }
                    }
                }else{
                    if(j == 0){
                        if(pimpl->history->table[i][j] == piece::X){
                            if(pimpl->history->table[i-1][j+1] == piece::O && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j+1] == piece::O && pimpl->history->table[i+2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j+1] = piece::e;
                                pimpl->history->table[i+2][j+2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j+1] == piece::o && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::X; 
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j+1] == piece::o && pimpl->history->table[i+2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j+1] = piece::e;
                                pimpl->history->table[i+2][j+2] = piece::X; 
                                mosso = true;
                            }
                        }else if(pimpl->history->table[i][j] == piece::x){
                            if(pimpl->history->table[i-1][j+1] == piece::o && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::x; 
                                mosso = true;
                            }
                        }
                    }else if(j == 7){
                        if(pimpl->history->table[i][j] == piece::X){
                            if(pimpl->history->table[i-1][j-1] == piece::O && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j-1] == piece::O && pimpl->history->table[i+2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j-1] = piece::e;
                                pimpl->history->table[i+2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j-1] == piece::o && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j-1] == piece::o && pimpl->history->table[i+2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j-1] = piece::e;
                                pimpl->history->table[i+2][j-2] = piece::X; 
                                mosso = true;
                            }
                        }else if(pimpl->history->table[i][j] == piece::x){
                            if(pimpl->history->table[i-1][j-1] == piece::o && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::x; 
                                mosso = true;
                            }
                        }
                    }else{
                        if(pimpl->history->table[i][j] == piece::X){
                            if(pimpl->history->table[i-1][j-1] == piece::O && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j+1] == piece::O && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j-1] == piece::O && pimpl->history->table[i+2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j-1] = piece::e;
                                pimpl->history->table[i+2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j+1] == piece::O && pimpl->history->table[i+2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j+1] = piece::e;
                                pimpl->history->table[i+2][j+2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j-1] == piece::o && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j+1] == piece::o && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j-1] == piece::o && pimpl->history->table[i+2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j-1] = piece::e;
                                pimpl->history->table[i+2][j-2] = piece::X;
                                mosso = true;
                            }else if(pimpl->history->table[i+1][j+1] == piece::o && pimpl->history->table[i+2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i+1][j+1] = piece::e;
                                pimpl->history->table[i+2][j+2] = piece::X;
                                mosso = true;
                            }
                        }else if(pimpl->history->table[i][j] == piece::x){
                            if(pimpl->history->table[i-1][j-1] == piece::o && pimpl->history->table[i-2][j-2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j-1] = piece::e;
                                pimpl->history->table[i-2][j-2] = piece::x; 
                                mosso = true;
                            }else if(pimpl->history->table[i-1][j+1] == piece::o && pimpl->history->table[i-2][j+2] == piece::e){
                                pimpl->history->table[i][j] = piece::e;
                                pimpl->history->table[i-1][j+1] = piece::e;
                                pimpl->history->table[i-2][j+2] = piece::x;
                                mosso = true; 
                            }
                        }
                    }
                }
                while(mosso == false){
                    int nPezzi = 0;
                    for(int k=0; k<8; k++){
                        for(int l=0; l<8; l++){
                            if(pimpl->history->table[k][l] == piece::X || pimpl->history->table[k][l] == piece::x){
                                nPezzi++;
                            }
                        }
                    }
                    if(nPezzi == 0){
                        //FUNZIONE CHE PERDE
                    }else{
                        srand(time(NULL));
                        nPezzi++;
                        int r = rand() % nPezzi + 1;
                        std::cout<< r << std::endl;
                        int count = 1;
                        for(int i=0; i<8; i++){
                            for(int j=0; j<8; j++){
                                if(pimpl->history->table[i][j] == piece::X || pimpl->history->table[i][j] == piece::x){
                                    if(count == r){
                                        if(i == 0){
                                            if(j == 0){
                                                if(pimpl->history->table[i][j] == piece::X){
                                                    if(pimpl->history->table[i+1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i+1][j+1] = piece::X;
                                                        mosso = true;
                                                    }
                                                }
                                            }else if(j == 7){
                                                if(pimpl->history->table[i][j] == piece::X){
                                                    if(pimpl->history->table[i+1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i+1][j-1] = piece::X;
                                                        mosso = true;
                                                    }
                                                }
                                            }else{
                                                if(pimpl->history->table[i][j] == piece::X){
                                                    if(pimpl->history->table[i+1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i+1][j-1] = piece::X;
                                                        mosso = true;
                                                    }else if(pimpl->history->table[i+1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i+1][j+1] = piece::X;
                                                        mosso = true;
                                                    }
                                                }
                                            }
                                        }else if(i == 7){
                                            if(j == 0){
                                                if(pimpl->history->table[i][j] == piece::X){
                                                    if(pimpl->history->table[i-1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j+1] = piece::X;
                                                        mosso = true;
                                                    }
                                                }else if(pimpl->history->table[i][j] == piece::x){
                                                    if(pimpl->history->table[i-1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j+1] = piece::x;
                                                        mosso = true;
                                                    }
                                                }
                                            }else if(j == 7){
                                                if(pimpl->history->table[i][j] == piece::X){
                                                    if(pimpl->history->table[i-1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j-1] = piece::X;
                                                        mosso = true;
                                                    }
                                                }else if(pimpl->history->table[i][j] == piece::x){
                                                    if(pimpl->history->table[i-1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j-1] = piece::x;
                                                        mosso = true; 
                                                    }
                                                }
                                            }else{
                                                if(pimpl->history->table[i][j] == piece::X){
                                                    if(pimpl->history->table[i-1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j-1] = piece::X;
                                                        mosso = true;
                                                    }else if(pimpl->history->table[i-1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j+1] = piece::X;
                                                        mosso = true;
                                                    }
                                                }else if(pimpl->history->table[i][j] == piece::x){
                                                    if(pimpl->history->table[i-1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j-1] = piece::x;
                                                        pimpl->history->table[i-2][j-2] = piece::x; 
                                                        mosso = true;
                                                    }else if(pimpl->history->table[i-1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j+1] = piece::x;
                                                        mosso = true;
                                                    }
                                                }
                                            }
                                        }else{
                                            if(j == 0){
                                                if(pimpl->history->table[i][j] == piece::X){
                                                    if(pimpl->history->table[i-1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j+1] = piece::X;
                                                        mosso = true;
                                                    }else if(pimpl->history->table[i+1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i+1][j+1] = piece::X;
                                                        mosso = true;
                                                    }
                                                }else if(pimpl->history->table[i][j] == piece::x){
                                                    if(pimpl->history->table[i-1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j+1] = piece::x;
                                                        mosso = true;
                                                    }
                                                }
                                            }else if(j == 7){
                                                if(pimpl->history->table[i][j] == piece::X){
                                                    if(pimpl->history->table[i-1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j-1] = piece::X;
                                                        mosso = true;
                                                    }else if(pimpl->history->table[i+1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i+1][j-1] = piece::X;
                                                        mosso = true;
                                                    }
                                                }else if(pimpl->history->table[i][j] == piece::x){
                                                    if(pimpl->history->table[i-1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j-1] = piece::x;
                                                        mosso = true;
                                                    }
                                                }
                                            }else{
                                                if(pimpl->history->table[i][j] == piece::X){
                                                    if(pimpl->history->table[i-1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j-1] = piece::X;
                                                        mosso = true;
                                                    }else if(pimpl->history->table[i-1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j+1] = piece::X;
                                                        mosso = true;
                                                    }else if(pimpl->history->table[i+1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i+1][j-1] = piece::X;
                                                        mosso = true;
                                                    }else if(pimpl->history->table[i+1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i+1][j+1] = piece::X;
                                                        mosso = true;
                                                    }
                                                }else if(pimpl->history->table[i][j] == piece::x){
                                                    if(pimpl->history->table[i-1][j-1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j-1] = piece::x;
                                                        mosso = true;
                                                    }else if(pimpl->history->table[i-1][j+1] == piece::e){
                                                        pimpl->history->table[i][j] = piece::e;
                                                        pimpl->history->table[i-1][j+1] = piece::x;
                                                        mosso = true; 
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    count++;
                                }
                            }
                        }
                    }
                    
                }
            }
        }
        for(int j=0; j<8; j++){
            if(pimpl->history->table[0][j] == piece::x){
                pimpl->history->table[0][j] = piece::X;
            }
        }
    }else{
        //CONTINUARE PER GIOCATORE 2
    }


}

/**
 * Funzioni aggiuntive
 */
int Player::getPlayer_nr(){
    return pimpl->player_nr;
}

void Player::printScacchiera(){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            std::cout << pimpl->history->table[i][j] << '\t';
        }
        std::cout << std::endl;
    }
}


int main(){

Player p;
p.load_board("scacchiera.txt");
p.move();
p.store_board("scacchiera2.txt");


  
  std::cout << "Tutto apposto bro" << std::endl;
    return 0;
}



