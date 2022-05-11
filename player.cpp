#include "player.hpp"

struct Player::Impl{
    int player_nr;
    struct Cell{
        piece table[8][8];
        Cell* next;
        Cell* prev;
        int history_offset;
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
    pimpl->history->history_offset = copy.pimpl->history->history_offset;
}

Player& Player::operator=(const Player& copy){
    pimpl->player_nr = copy.pimpl->player_nr;
    pimpl->history = copy.pimpl->history;
    pimpl->history->history_offset = copy.pimpl->history->history_offset;
    return *this;
}

/**
 * @brief 
 * FIXTO: sistemare l'eccezione con hystory offset non presente in memoria
 * @param r 
 * @param c 
 * @param hystory_offset 
 * @return Player::piece 
 */
Player::piece Player::operator()(int r,int c,int history_offset) const{
    piece p;
    bool flag = false;
    if(pimpl->history == nullptr || r > 8 || r < 0 || c > 8 || c < 0 || history_offset < 0){
        throw player_exception{player_exception::index_out_of_bounds,string{"Parametri non presenti in memoria!"}};
    }else{
        if(history_offset == 0){
            p = pimpl->history->table[r][c];
        }else{
            while(pimpl->history != nullptr || flag == true){
               if(pimpl->history->history_offset = history_offset){
                   p = pimpl->history->table[r][c];
                   flag = true;
               }
                pimpl->history = pimpl->history->prev;
            }
        }
    }
    
    return p;
}

void Player::load_board(const string& filename){
    pimpl->history = new Player::Impl::Cell;
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

/**
 * @brief 
 * FIXTO: Manca la parte history offset != 0
 * @param filename 
 * @param history_offset 
 */
void Player::store_board(const string& filename, int history_offset)const{
    std::ofstream output{filename};
    if(!output.good()){
         throw player_exception{player_exception::missing_file,string{"File di output " + filename + " non è scrivibile"}};
    }
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
    if(!output.good())
        throw player_exception{player_exception::missing_file,string{"Impossibile scrivere sul file di output " + filename}};
    output.close();
    if(output.fail())
        throw player_exception{player_exception::missing_file,string{"Impossibile chiudere il file " + filename}};
}

void Player::init_board(const string& filename)const{
    pimpl->history = new Player::Impl::Cell;
    pimpl->history->history_offset = 0;
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
    int i = 0;
    while(i < 8 && mosso == true){
        int j = 0;
        while(j < 8){
            if(pimpl->history->table[i][j] == piece::x && pimpl->history->table[i-1][j+1] == piece::e){
                pimpl->history->table[i][j] = piece::e;
                pimpl->history->table[i-1][j+1] = piece::x;
                mosso = true;
            }
            j++;
        }
        i++;
    }
    pimpl->history->history_offset++;
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
p.init_board("scacchiera.txt");
p.move();
p.store_board("scacchieraMossa.txt");
  
  std::cout << "Tutto apposto bro" << std::endl;
    return 0;
}



