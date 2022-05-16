#include "player.hpp"
struct Player::Impl{
    int player_nr;
    struct Cell{
        piece table[8][8];
        Cell* next;
    };
    typedef Cell* List;
    
    List history;

    void prepend(List& t);
    bool m_promuovi();
    void promuovi();

    bool mangia_su_sinistra(int i, int j, int mangia, int da_mangiare);
    bool mangia_su_destra(int i, int j, int mangia, int da_mangiare);
    bool mangia_giu_sinistra(int i, int j, int mangia, int da_mangiare);
    bool mangia_giu_destra(int i, int j, int mangia, int da_mangiare);

    bool d_mangia(int i, int j, int mangia, int da_mangiare);

    bool mangia();

    bool muovi_su_sinistra(int i, int j, int da_muovere, int predatore = -1);
    bool muovi_su_destra(int i, int j, int da_muovere, int predatore = -1);
    bool muovi_giu_sinistra(int i, int j, int da_muovere, int predatore = -1);
    bool muovi_giu_destra(int i, int j, int da_muovere, int predatore = -1);

    bool p_muovi(int i, int j, int da_muovere, int predatore = -1);

    bool muovi(int i, int j);

    bool muovi_r();
};

void Player::Impl::prepend(List& t){
    List nuova;
        nuova = new Cell;
        nuova->next = t;
        t = nuova;
}

bool Player::Impl::m_promuovi(){
    bool mosso = false;
    if(player_nr == 1){
        for(int j=0; j<8; j++){
            if(j == 0){
                if(history->table[1][j] == piece::x){
                    if(history->table[1-1][j+1] == piece::e){
                        history->table[1][j] = piece::e;
                        history->table[1-1][j+1] = piece::X;
                        mosso = true;
                    }
                }
            }else if(j == 7){
                if(history->table[1][j] == piece::x){
                    if(history->table[1-1][j-1] == piece::e){
                        history->table[1][j] = piece::e;
                        history->table[1-1][j-1] = piece::X;
                        mosso = true;
                    }
                }
            }else{
                if(history->table[1][j] == piece::x){
                    if(history->table[1-1][j-1] == piece::e){
                        history->table[1][j] = piece::e;
                        history->table[1-1][j-1] = piece::X;
                        mosso = true;
                    }else if(history->table[1-1][j+1] == piece::e){
                        history->table[1][j] = piece::e;
                        history->table[1-1][j+1] = piece::X;
                        mosso = true;
                    }
                }
            }
        }
    }else{
      for(int j=0; j<8; j++){
            if(j == 0){
                if(history->table[6][j] == piece::o){
                    if(history->table[6+1][j+1] == piece::e){
                        history->table[6][j] = piece::e;
                        history->table[6+1][j+1] = piece::O;
                        mosso = true;
                    }
                }
            }else if(j == 7){
                if(history->table[6][j] == piece::o){
                    if(history->table[6+1][j-1] == piece::e){
                        history->table[6][j] = piece::e;
                        history->table[6+1][j-1] = piece::O;
                        mosso = true;
                    }
                }
            }else{
                if(history->table[6][j] == piece::o){
                    if(history->table[6+1][j-1] == piece::e){
                        history->table[6][j] = piece::e;
                        history->table[6+1][j-1] = piece::O;
                        mosso = true;
                    }else if(history->table[6+1][j+1] == piece::e){
                        history->table[6][j] = piece::e;
                        history->table[6+1][j+1] = piece::O;
                        mosso = true;
                    }
                }
            }
        }  
    }
    return mosso;
}

void Player::Impl::promuovi(){
    for(int j=0; j<8; j++){
        if(player_nr == 1){
            if(history->table[0][j] == piece::x){
                history->table[0][j] = piece::X;
            }
        }else{
            if(history->table[7][j] == piece::o){
                history->table[7][j] = piece::O;
            }
        }         
    }
}

bool Player::Impl::mangia_su_sinistra(int i, int j, int mangia, int da_mangiare){
    bool mosso = false;
    if(history->table[i][j] == mangia){
        if(history->table[i-1][j-1] == da_mangiare && history->table[i-2][j-2] == piece::e){
            history->table[i][j] = piece::e;
            history->table[i-1][j-1] = piece::e;
            history->table[i-2][j-2] = (piece)mangia;
            mosso = true;
        }
    }
    return mosso;
}

bool Player::Impl::mangia_su_destra(int i, int j, int mangia, int da_mangiare){
    bool mosso = false;
    if(history->table[i][j] == mangia){
        if(history->table[i-1][j+1] == da_mangiare && history->table[i-2][j+2] == piece::e){
            history->table[i][j] = piece::e;
            history->table[i-1][j+1] = piece::e;
            history->table[i-2][j+2] = (piece)mangia;
            mosso = true;
        }
    }
    return mosso;
}

bool Player::Impl::mangia_giu_sinistra(int i, int j, int mangia, int da_mangiare){
    bool mosso = false;
    if(history->table[i][j] == mangia){
        if(history->table[i+1][j-1] == da_mangiare && history->table[i+2][j-2] == piece::e){
            history->table[i][j] = piece::e;
            history->table[i+1][j-1] = piece::e;
            history->table[i+2][j-2] = (piece)mangia;
            mosso = true;
        }
    }
    return mosso;
}

bool Player::Impl::mangia_giu_destra(int i, int j, int mangia, int da_mangiare){
    bool mosso = false;
    if(history->table[i][j] == mangia){
        if(history->table[i+1][j+1] == da_mangiare && history->table[i-2][j+2] == piece::e){
            history->table[i][j] = piece::e;
            history->table[i+1][j+1] = piece::e;
            history->table[i+2][j+2] = (piece)mangia;
            mosso = true;
        }
    }
    return mosso;
}

bool Player::Impl::d_mangia(int i, int j,int mangia, int da_mangiare){
    bool mosso = false;
            if(i == 0){
                if(j == 0){
                    if(mosso == false)
                        mosso = mangia_giu_destra(i,j,mangia,da_mangiare);
                }else{
                    if(mosso == false)
                        mosso = mangia_giu_destra(i,j,mangia,da_mangiare);
                    if(mosso == false)
                        mosso = mangia_giu_sinistra(i,j,mangia,da_mangiare);
                }
            }else if(i == 7){
                if(j == 7){
                    if(mosso == false)
                        mosso = mangia_su_sinistra(i,j,mangia,da_mangiare);
                }else{
                    if(mosso == false)
                        mosso = mangia_su_destra(i,j,mangia,da_mangiare);
                    if(mosso == false)
                        mosso = mangia_su_sinistra(i,j,mangia,da_mangiare);
                }
            }else{
                if(j == 0){
                    if(mosso == false)
                        mosso = mangia_su_destra(i,j,mangia,da_mangiare);
                    if(mosso == false)
                        mosso = mangia_giu_destra(i,j,mangia,da_mangiare);
                }else if(j == 7){
                    if(mosso == false)
                        mosso = mangia_su_sinistra(i,j,mangia,da_mangiare);
                    if(mosso == false)
                        mosso = mangia_su_destra(i,j,mangia,da_mangiare);
                }else{
                    if(mosso == false)
                        mosso = mangia_su_destra(i,j,mangia,da_mangiare);
                    if(mosso == false)
                        mosso = mangia_su_sinistra(i,j,mangia,da_mangiare);
                    if(mosso == false)
                        mosso = mangia_giu_destra(i,j,mangia,da_mangiare);
                    if(mosso == false)
                        mosso = mangia_giu_sinistra(i,j,mangia,da_mangiare);
                }
            }
            return mosso;    
}

bool Player::Impl::mangia(){
    bool mosso = false;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(player_nr == 1){
                if(history->table[i][j] == piece::X){
                    if(mosso == false)
                        mosso = d_mangia(i,j,piece::X,piece::O);
                    if(mosso == false)
                        mosso = d_mangia(i,j,piece::X,piece::o);
                }else if(history->table[i][j] == piece::x){
                    if(mosso == false)
                        mosso = d_mangia(i,j,piece::x,piece::o);
                }
            }else{
                if(history->table[i][j] == piece::O){
                    if(mosso == false)
                        mosso = d_mangia(i,j,piece::O,piece::X);
                    if(mosso == false)
                        mosso = d_mangia(i,j,piece::O,piece::x);
                }else if(history->table[i][j] == piece::o){
                    if(mosso == false)
                        mosso = d_mangia(i,j,piece::o,piece::x);
                }
            }
        }
    }
    return mosso;
}

bool Player::Impl::muovi_su_sinistra(int i, int j, int da_muovere, int predatore){
    bool mosso = false;
    if(history->table[i][j] == da_muovere){
        if(history->table[i-1][j-1] == piece::e && history->table[i-2][j-2] != predatore){
            history->table[i][j] = piece::e;
            history->table[i-1][j-1] = (piece)da_muovere;
            mosso = true;
        }
    }
    return mosso;
}

bool Player::Impl::muovi_su_destra(int i, int j, int da_muovere, int predatore){
    bool mosso = false;
    if(history->table[i][j] == da_muovere){
        if(history->table[i-1][j+1] == piece::e && history->table[i-2][j+2] != predatore){
            history->table[i][j] = piece::e;
            history->table[i-1][j+1] = (piece)da_muovere;
            mosso = true;
        }
    }
    return mosso;
}

bool Player::Impl::muovi_giu_sinistra(int i, int j, int da_muovere, int predatore){
    bool mosso = false;
    if(history->table[i][j] == da_muovere){
        if(history->table[i+1][j-1] == piece::e && history->table[i+2][j-2] != predatore){
            history->table[i][j] = piece::e;
            history->table[i+1][j-1] = (piece)da_muovere;
            mosso = true;
        }
    }
    return mosso;
}

bool Player::Impl::muovi_giu_destra(int i, int j, int da_muovere, int predatore){
    bool mosso = false;
    if(history->table[i][j] == da_muovere){
        if(history->table[i+1][j+1] == piece::e && history->table[i+2][j+2] != predatore){
            history->table[i][j] = piece::e;
            history->table[i+1][j+1] = (piece)da_muovere;
            mosso = true;
        }
    }
    return mosso;
}

bool Player::Impl::p_muovi(int i, int j, int da_muovere, int predatore){
    bool mosso = false;
        if(i == 0){
            if(j == 0){
                if(mosso == false)
                    mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                }else{
                    if(mosso == false)
                        mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                    if(mosso == false)
                        mosso = muovi_giu_sinistra(i,j,da_muovere,predatore);
                }
            }else if(i == 7){
                if(j == 7){
                    if(mosso == false)
                        mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                }else{
                    if(mosso == false)
                        mosso = muovi_su_destra(i,j,da_muovere,predatore);
                    if(mosso == false)
                        mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                }
            }else{
                if(j == 0){
                    if(mosso == false)
                        mosso = muovi_su_destra(i,j,da_muovere,predatore);
                    if(mosso == false)
                        mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                }else if(j == 7){
                    if(mosso == false)
                        mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                    if(mosso == false)
                        mosso = muovi_su_destra(i,j,da_muovere,predatore);
                }else{
                    if(mosso == false)
                        mosso = muovi_su_destra(i,j,da_muovere,predatore);
                    if(mosso == false)
                        mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                    if(mosso == false)
                        mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                    if(mosso == false)
                        mosso = muovi_giu_sinistra(i,j,da_muovere,predatore);
                }
            }
            return mosso;    
}

bool Player::Impl::muovi(int i, int j){
    bool mosso = false;
            if(player_nr == 1){
                if(history->table[i][j] == piece::X){
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::X,piece::O);
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::X);
                }else if(history->table[i][j] == piece::x){
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::x,piece::O);
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::x,piece::o);
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::x);
                }
            }else{
                if(history->table[i][j] == piece::O){
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::O,piece::X);
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::O);
                }else if(history->table[i][j] == piece::o){
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::o,piece::X);
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::o,piece::x);
                    if(mosso == false)
                        mosso = p_muovi(i,j,piece::o);
                }
            }
    
    return mosso;
}

bool Player::Impl::muovi_r(){
    bool mosso = false;
    int nPezzi = 0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(player_nr == 1){
                if(history->table[i][j] == piece::X || history->table[i][j] == piece::x){
                    nPezzi++;
                }
            }else{
                if(history->table[i][j] == piece::O || history->table[i][j] == piece::o){
                    nPezzi++;
                }
            }
        }
    }

    if(nPezzi == 0){
        //FUNZIONE CHE PERDE
    }else{
        while(mosso == false){
        srand(time(NULL));
        nPezzi++;
        int r = rand() % nPezzi + 1;
        std::cout<< "random:" << r << std::endl;
        int count = 1;
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                if(player_nr == 1){
                    if(history->table[i][j] == piece::X || history->table[i][j] == piece::x){
                        if(count == r)
                            mosso = muovi(i,j);
                        count++;
                    }
                }else{
                    if(history->table[i][j] == piece::O || history->table[i][j] == piece::o){
                        if(count == r)
                            mosso = muovi(i,j);
                    count++;
                    }
                }
            }
        }
        }
    }
    return mosso;
}

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

    if(mosso == false)
        mosso = pimpl->m_promuovi();
    if(mosso == false)
        mosso = pimpl->mangia();
    if(mosso == false)
        mosso = pimpl->muovi_r();
 
    pimpl->promuovi();
              
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



