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

    bool muovi_rand();

    bool is_equal(piece source[8][8], piece dest[8][8]);

    void copyBoard(piece source[8][8], piece dest[8][8]);

    bool valid_muovi(int i, int j);
    bool valid_promuovi(int i, int j);
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
        if(history->table[i][j] != piece::o && history->table[i-1][j-1] == da_mangiare && (i-2) >= 0 && (j-2) >= 0 && history->table[i-2][j-2] == piece::e){
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
        if(history->table[i][j] != piece::o && history->table[i-1][j+1] == da_mangiare && (i-2) >= 0 && (j+2) <= 7 && history->table[i-2][j+2] == piece::e){
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
        if(history->table[i][j] != piece::x && history->table[i+1][j-1] == da_mangiare && (i+2) <= 7 && (j-2) >= 0 && history->table[i+2][j-2] == piece::e){
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
        if(history->table[i][j] != piece::x && history->table[i+1][j+1] == da_mangiare && (i+2) <= 7 && (j+2) <= 7  && history->table[i+2][j+2] == piece::e){
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
        if(history->table[i][j] != piece::o && history->table[i-1][j-1] == piece::e && (i-2) >= 0 && (j-2) >= 0 && history->table[i-2][j-2] != predatore){
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
        if(history->table[i][j] != piece::o && history->table[i-1][j+1] == piece::e && (i-2) >= 0 && (j+2) <= 7 && history->table[i-2][j+2] != predatore){
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
        if(history->table[i][j] != piece::x && history->table[i+1][j-1] == piece::e && (i+2) <= 7 && (j-2) >= 0 && history->table[i+2][j-2] != predatore){
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
        if(history->table[i][j] != piece::x && history->table[i+1][j+1] == piece::e && (i+2) <= 7 && (j+2) <= 7 && history->table[i+2][j+2] != predatore){
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
                    bool cond1 = false, cond2 = false;
                        srand(time(NULL));
                        int r = rand()%2;
                        if(r == 0){
                            mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                            cond1 = true;
                        }else if(r == 1){
                            mosso = muovi_giu_sinistra(i,j,da_muovere,predatore);
                            cond2 = true;
                        }
                    if(cond1 == false && mosso == false){
                        mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                    }else if(cond2 == false && mosso == false){
                        mosso = muovi_giu_sinistra(i,j,da_muovere,predatore);
                    }    
                }
            }else if(i == 7){
                if(j == 7){
                    if(mosso == false)
                        mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                }else{
                   bool cond1 = false, cond2 = false;
                        srand(time(NULL));
                        int r = rand()%2;
                        if(r == 0){
                            mosso = muovi_su_destra(i,j,da_muovere,predatore);
                            cond1 = true;
                        }else if(r == 1){
                            mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                            cond2 = true;
                        }
                    if(cond1 == false && mosso == false){
                        mosso = muovi_su_destra(i,j,da_muovere,predatore);
                    }else if(cond2 == false && mosso == false){
                        mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                    }
                }
            }else{
                if(j == 0){
                    bool cond1 = false, cond2 = false;
                        srand(time(NULL));
                        int r = rand()%2;
                        if(r == 0){
                            mosso = muovi_su_destra(i,j,da_muovere,predatore);
                            cond1 = true;
                        }else if(r == 1){
                            mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                            cond2 = true;
                        }
                    if(cond1 == false && mosso == false){
                        mosso = muovi_su_destra(i,j,da_muovere,predatore);
                    }else if(cond2 == false && mosso == false){
                        mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                    }
                }else if(j == 7){
                    bool cond1 = false, cond2 = false;
                        srand(time(NULL));
                        int r = rand()%2;
                        if(r == 0){
                            mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                            cond1 = true;
                        }else if(r == 1){
                            mosso = muovi_giu_sinistra(i,j,da_muovere,predatore);
                            cond2 = true;
                        }
                    if(cond1 == false && mosso == false){
                        mosso = muovi_su_destra(i,j,da_muovere,predatore);
                    }else if(cond2 == false && mosso == false){
                        mosso = muovi_giu_sinistra(i,j,da_muovere,predatore);
                    }
                }else{
                        srand(time(NULL));
                        int r = rand()%4;
                        if(r == 0){
                            mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                        }else if(r == 1){
                            mosso = muovi_giu_sinistra(i,j,da_muovere,predatore);
                        }else if(r == 2){
                            mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                        }else if(r == 3){
                            mosso = muovi_su_destra(i,j,da_muovere,predatore);
                        }
                    if(mosso == false)
                         mosso = muovi_giu_destra(i,j,da_muovere,predatore);
                    if(mosso == false)
                         mosso = muovi_giu_sinistra(i,j,da_muovere,predatore);
                    if(mosso == false)
                        mosso = muovi_su_destra(i,j,da_muovere,predatore);
                    if(mosso == false)
                         mosso = muovi_su_sinistra(i,j,da_muovere,predatore);
                }
            }
            return mosso;    
}

bool Player::Impl::muovi(int i, int j){
    bool mosso = false;
            if(player_nr == 1){
                if(history->table[i][j] == piece::X){
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::X,piece::O);
                    }
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::X);
                    }
                }else if(history->table[i][j] == piece::x){
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::x,piece::O);
                    }
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::x,piece::o);
                    }
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::x);
                    }
                }
            }else{
                if(history->table[i][j] == piece::O){
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::O,piece::X);
                    }
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::O);
                    }
                }else if(history->table[i][j] == piece::o){
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::o,piece::X);
                    }
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::o,piece::x);
                    }
                    if(mosso == false){
                        mosso = p_muovi(i,j,piece::o);
                    }
                }
            }
    
    return mosso;
}

bool Player::Impl::muovi_rand(){
    bool mosso = false;
    bool m = false;
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
        while(m == false){
        srand(time(NULL));
        int r = rand() % nPezzi;
        int count = 0;
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                if(player_nr == 1){
                    if(history->table[i][j] == piece::X || history->table[i][j] == piece::x){
                        if(count == r)
                            m = muovi(i,j);
                        count++;
                    }
                }else{
                    if(history->table[i][j] == piece::O || history->table[i][j] == piece::o){
                        if(count == r)
                            m = muovi(i,j);
                    count++;
                    }
                }
                mosso = m;
                if(nPezzi == 1 && m == false){
                    m = true;
                }
            }
        }
        }
    
    return mosso;
}

bool Player::Impl::is_equal(piece source[8][8], piece dest[8][8]){
    bool eq = true;
    int i = 0;
    while(i < 8){
        int j = 0;
        while(j < 8){
            if(source[i][j] != dest[i][j]){
                eq = false;
            }
            j++;
        }
        i++;
    }
    return eq;
}

void Player::Impl::copyBoard(piece source[8][8], piece dest[8][8]){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            dest[i][j] = source[i][j];
        }
    }
}

bool Player::Impl::valid_muovi(int i, int j){
    bool valid = false;
    if(i == 0){
        if(j == 0){
            if(history->table[i+1][j+1] == history->next->table[i][j]){
                if(history->table[i+1][j+1] != history->next->table[i+1][j+1]){
                    valid = true;
               }
            }
        }else{
            if(history->table[i+1][j+1] == history->next->table[i][j]){
                if(history->table[i+1][j+1] != history->next->table[i+1][j+1]){
                    valid = true;
               }
            }
            if(valid == false){
                if(history->table[i+1][j-1] == history->next->table[i][j]){
                    if(history->table[i+1][j-1] != history->next->table[i+1][j-1]){
                        valid = true;
                    }
                }
            }
        }
    }else if(i == 7){
        if(j == 7){
           if(history->table[i-1][j-1] == history->next->table[i][j]){
                if(history->table[i-1][j-1] != history->next->table[i-1][j-1]){
                    valid = true;
                }
            } 
        }else{
            if(history->table[i-1][j-1] == history->next->table[i][j]){
                if(history->table[i-1][j-1] != history->next->table[i-1][j-1]){
                    valid = true;
                }
            }
            if(valid == false){
                if(history->table[i-1][j+1] == history->next->table[i][j]){
                    if(history->table[i-1][j+1] != history->next->table[i-1][j+1]){
                        valid = true;
                    }
                }
            }
        }
    }else{
        if(j == 0){
            if(history->table[i+1][j+1] == history->next->table[i][j]){
                if(history->table[i+1][j+1] != history->next->table[i+1][j+1]){
                    valid = true;
               }
            }
            if(valid == false){
                if(history->table[i-1][j+1] == history->next->table[i][j]){
                    if(history->table[i-1][j+1] != history->next->table[i-1][j+1]){
                        valid = true;
                    }
                }
            }
        }else if(j == 7){
            if(history->table[i-1][j-1] == history->next->table[i][j]){
                if(history->table[i-1][j-1] != history->next->table[i-1][j-1]){
                    valid = true;
                }
            }
            if(valid == false){
                if(history->table[i+1][j-1] == history->next->table[i][j]){
                    if(history->table[i+1][j-1] != history->next->table[i+1][j-1]){
                        valid = true;
                    }
                }
            }
        }else{
            if(history->table[i+1][j+1] == history->next->table[i][j]){
                if(history->table[i+1][j+1] != history->next->table[i+1][j+1]){
                    valid = true;
               }
            }
            if(valid == false){
                if(history->table[i+1][j-1] == history->next->table[i][j]){
                    if(history->table[i+1][j-1] != history->next->table[i+1][j-1]){
                        valid = true;
                    }
                }
            }
            if(valid == false){
                if(history->table[i-1][j-1] == history->next->table[i][j]){
                    if(history->table[i-1][j-1] != history->next->table[i-1][j-1]){
                        valid = true;
                    }
                }
            }
            if(valid == false){
               if(history->table[i-1][j+1] == history->next->table[i][j]){
                    if(history->table[i-1][j+1] != history->next->table[i-1][j+1]){
                        valid = true;
                    }
                } 
            }
        }
    }
    return valid;
}

bool Player::Impl::valid_promuovi(int i, int j){
    bool valid = false;
    if(i == 1){
        if(j == 7){
            if(history->table[i-1][j-1] == piece::X && history->next->table[i][j] == piece::x){
                if(history->table[i][j] == piece::e){
                    valid = true;
                }
            }
        }else{
            if(history->table[i-1][j-1] == piece::X && history->next->table[i][j] == piece::x){
                if(history->table[i][j] == piece::e){
                    valid = true;
                }
            }
            if(valid == false){
                if(history->table[i-1][j+1] == piece::X && history->next->table[i][j] == piece::x){
                if(history->table[i][j] == piece::e){
                    valid = true;
                }
            }
            }
        }
    }else if(i == 6){
        if(j == 0){
            if(history->table[i+1][j+1] == piece::O && history->next->table[i][j] == piece::o){
                if(history->table[i][j] == piece::e){
                    valid = true;
                }
            }
        }else{
            if(history->table[i+1][j+1] == piece::O && history->next->table[i][j] == piece::o){
                if(history->table[i][j] == piece::e){
                    valid = true;
                }
            }
            if(valid == false){
               if(history->table[i+1][j-1] == piece::O && history->next->table[i][j] == piece::o){
                if(history->table[i][j] == piece::e){
                    valid = true;
                }
            } 
            }
        }
    }else if(i == 2){
        if(j == 0){
            if(history->table[i-2][j+2] == piece::X && history->next->table[i][j] == piece::x){
                if(history->table[i][j] == piece::e && history->table[i-1][j+1] == piece::e){
                    valid = true;
                }
            }
        }else if(j == 6){
            if(history->table[i-2][j-2] == piece::X && history->next->table[i][j] == piece::x){
                if(history->table[i][j] == piece::e && history->table[i-1][j-1] == piece::e){
                    valid = true;
                }
            }
        }else{
            if(history->table[i-2][j-2] == piece::X && history->next->table[i][j] == piece::x){
                if(history->table[i][j] == piece::e && history->table[i-1][j-1] == piece::e){
                    valid = true;
                }
            }
            if(valid == false){
                if(history->table[i-2][j+2] == piece::X && history->next->table[i][j] == piece::x){
                    if(history->table[i][j] == piece::e && history->table[i-1][j+1] == piece::e){
                        valid = true;
                    }
                }
            }
        }
    }else if(i == 5){
        if(j == 7){
            if(history->table[i+2][j-2] == piece::O && history->next->table[i][j] == piece::o){
                if(history->table[i][j] == piece::e && history->table[i+1][j-1] == piece::e){
                    valid = true;
                }
            }
        }else if(j == 1){
            if(history->table[i+2][j+2] == piece::O && history->next->table[i][j] == piece::o){
                if(history->table[i][j] == piece::e && history->table[i+1][j+1] == piece::e){
                    valid = true;
                }
            }
        }else{
            if(history->table[i+2][j-2] == piece::O && history->next->table[i][j] == piece::o){
                if(history->table[i][j] == piece::e && history->table[i+1][j-1] == piece::e){
                    valid = true;
                }
            }
            if(valid == false){
                if(history->table[i+2][j+2] == piece::O && history->next->table[i][j] == piece::o){
                if(history->table[i][j] == piece::e && history->table[i+1][j+1] == piece::e){
                    valid = true;
                }
            } 
            }
        }
    }
    return valid;
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
    Player::Impl::List currentNode = pimpl->history;
    Player::Impl::List temp;
    while(currentNode != nullptr){
        temp = currentNode->next;
        delete currentNode;
        currentNode = temp;
    }
    
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
    Player::Impl::List pCell = pimpl->history;
    int sum = 0;
    while(pCell != nullptr){
        if(sum == history_offset){
            for(int i=0; i<8; i++){
                for(int j=0; j<8; j++){
                    switch(pCell->table[i][j]){
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
        pCell = pCell->next;
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
    std::ofstream  output{filename};
    if(!output.good())
        throw player_exception{player_exception::missing_file,string{"File di output " + filename + " non è scrivibile"}};
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(i == 0 || i == 2){
                if(j%2 == 0){
                    output << 'o';
                }else{
                    output << ' ';
                }
            }else if(i == 1){
                if(j%2 == 1){
                    output << 'o';
                }else{
                    output << ' ';
                }
            }else if(i == 5 || i == 7){
                if(j%2 == 1){
                    output << 'x';
                }else{
                    output << ' ';
                }
            }else if(i == 6){
                if(j%2 == 0){
                    output << 'x';
                }else{
                    output << ' ';
                }
            }else{
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
        mosso = pimpl->muovi_rand();
    if(mosso == false){
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                pimpl->history->table[i][j] = pimpl->history->next->table[i][j];
            }
        }
    }
 
    pimpl->promuovi();

    if(pimpl->history == nullptr){
        throw player_exception{player_exception::index_out_of_bounds,string{"History vuota!"}};
    }
              
}

bool Player::valid_move()const{
    bool valid = false;
    if(pimpl->history == nullptr){
        throw player_exception{player_exception::index_out_of_bounds,string{"History vuota!"}};
    }

    if(pimpl->history->next == nullptr){
        throw player_exception{player_exception::index_out_of_bounds,string{"Ci sono meno di due scacchiere nella history!"}};
    }
    if(pimpl->is_equal(pimpl->history->table, pimpl->history->next->table)){
        valid = false;
    }else{ 
        int i = 0;
        while(i<8 && valid == false){
            int j = 0;
            while(j < 8 && valid == false){
                if(pimpl->history->next->table[i][j] != pimpl->history->table[i][j]){
                    if(pimpl->history->table[i][j] == piece::e){
                        valid = pimpl->valid_muovi(i,j);
                        if(valid == false){
                            valid = pimpl->valid_promuovi(i,j);
                        }
                        if((pimpl->player_nr == 1 && (pimpl->history->next->table[i][j] == piece::O || pimpl->history->next->table[i][j] == piece::o)) || (pimpl->player_nr == 2 && (pimpl->history->next->table[i][j] == piece::X || pimpl->history->next->table[i][j] == piece::x))){
                            if(valid == false){
                                if(pimpl->history->table[i+1][j+1] != piece::e && pimpl->history->table[i+1][j+1] == pimpl->history->next->table[i-1][j-1]){
                                    valid = true;
                                }
                            }
                            if(valid == false){
                                if(pimpl->history->table[i+1][j-1] != piece::e && pimpl->history->table[i+1][j-1] == pimpl->history->next->table[i-1][j+1]){
                                    valid = true;
                                }
                            }
                            if(valid == false){
                                if(pimpl->history->table[i-1][j-1] != piece::e && pimpl->history->table[i-1][j-1] == pimpl->history->next->table[i+1][j+1]){
                                    valid = true;
                                }
                            }
                            if(valid == false){
                                if(pimpl->history->table[i-1][j+1] != piece::e && pimpl->history->table[i-1][j+1] == pimpl->history->next->table[i+1][j-1]){
                                    valid = true;
                                }
                            }
                        }
                    }
                }
                j++;
            }
            i++;
        }
    }
    return valid;
}

void Player::pop(){
    Player::Impl::List pCell;
    if(pimpl->history == nullptr)
         throw player_exception{player_exception::index_out_of_bounds,string{"History vuota!"}};

    pCell = pimpl->history;
    pimpl->history = pimpl->history->next;
    delete pCell;
}

bool Player::wins(int player_nr) const{
    bool control = false;
    if(pimpl->history == nullptr)
         throw player_exception{player_exception::index_out_of_bounds,string{"History vuota!"}};
    if(player_nr != 1 && player_nr != 2)
         throw player_exception{player_exception::index_out_of_bounds,string{"Giocatore non valido!"}};
    int count = 0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(player_nr == 1){
                if(pimpl->history->table[i][j] == piece::o || pimpl->history->table[i][j] == piece::O){
                    count++;
                }
            }else{
                if(pimpl->history->table[i][j] == piece::x || pimpl->history->table[i][j] == piece::X){
                    count++;
                }
            }
        }
    }
    if(count == 0)
        control = true;
    return control;
}

bool Player::wins() const{
    bool control = false;
    if(pimpl->history == nullptr)
         throw player_exception{player_exception::index_out_of_bounds,string{"History vuota!"}};
    int count = 0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if(pimpl->player_nr == 1){
                if(pimpl->history->table[i][j] == piece::o || pimpl->history->table[i][j] == piece::O){
                    count++;
                }
            }else{
                if(pimpl->history->table[i][j] == piece::x || pimpl->history->table[i][j] == piece::X){
                    count++;
                }
            }
        }
    }
    if(count == 0){
        control = true;
    }

    return control;
}

bool Player::loses(int player_nr)const{
    if(pimpl->history == nullptr)
         throw player_exception{player_exception::index_out_of_bounds,string{"History vuota!"}};
    if(player_nr != 1 && player_nr != 2)
         throw player_exception{player_exception::index_out_of_bounds,string{"Giocatore non valido!"}};
    return !wins(player_nr);
}

bool Player::loses()const{
    if(pimpl->history == nullptr)
         throw player_exception{player_exception::index_out_of_bounds,string{"History vuota!"}};
    return !wins();
}

int Player::recurrence() const{
    Player::Impl::List pCell;
    pCell = pimpl->history;
    piece table[8][8];
    int count = 0;
    if(pimpl->history == nullptr)
         throw player_exception{player_exception::index_out_of_bounds,string{"History vuota!"}};

    pimpl->copyBoard(pimpl->history->table,table);

    while(pCell != nullptr){

        if(pimpl->is_equal(table,pCell->table)){
            count++;
        }

        pCell = pCell->next;
    }
    return count;
}











