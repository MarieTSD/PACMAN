//PAC-MAN
//Mariela Teresa Serna Davila
//Jessica Guerrero Carrera

#include <iostream>
#include <allegro.h>
#include <fstream>
#include <cstdlib>//rand
#include "Game.h"
#include "matriz.h"

using namespace std;

BITMAP *buffer, *pacman, *pacbmp, *murio_bmp, *lose, *pasto, *puntoAmarillo, *cerezas, *puntoBlanco, *win;
SAMPLE *guero, *fruta, *mus_inicio, *muerte_final, *blanco;


class Control{
public:
    struct Cont{
        int direccion;
        int px, py, anteriorpx, anteriorpy;
        int puntos;
        int vidasTotales;
    };
    Cont x;
    int nivel;
public:
    Control(){
        x.direccion=4;
        x.px=40*10;
        x.py=40*12;
        x.puntos=4;
        x.vidasTotales=3;
        nivel=1;
        x.anteriorpx=0;
        x.anteriorpy=0;
    }
    inline void SetPuntos(int q){
        x.puntos=x.puntos+q;
    }
    inline int getPuntos(){return x.puntos;}

    void dibujar_mapa(){
        for(int i=0; i<REN; i++){
            for(int j=0; j<COL; j++){
                if(mapa_fondo[i][j]=='X'){
                    draw_sprite(buffer, pasto, j*40, i*40);
                }
                else if(mapa_fondo[i][j]=='o'||mapa_fondo[i][j]=='|'){
                    draw_sprite(buffer, puntoAmarillo, j*40, i*40);
                    if(x.py/40 == i && x.px/40 == j){
                        play_sample(guero,200,150,1000,0);
                        mapa_fondo[i][j]=' ';
                        SetPuntos(10);
                    }
                }else if(mapa_fondo[i][j]=='c'){
                    draw_sprite(buffer, cerezas, j*40, i*40);
                    if(x.py/40 == i && x.px/40 == j){
                        play_sample(fruta,200,150,1000,0);
                        mapa_fondo[i][j]=' ';
                        SetPuntos(100);
                    }
                }
                else if(mapa_fondo[i][j]=='p'){
                    draw_sprite(buffer, puntoBlanco, j*40, i*40);
                    if(x.py/40 == i && x.px/40 == j){
                        play_sample(blanco,200,150,1000,0);
                        mapa_fondo[i][j]=' ';
                        SetPuntos(50);
                    }
                }
            }
        }
    }

    void pantalla(){
        blit(buffer, screen, 0, 0, 0, 0, 880, 770);
    }

    void dibujar_personaje(){
        blit(pacbmp, pacman, x.direccion*43, 0, 0, 0, 43, 43);//dibujarPacman();//dibujar personaje
        draw_sprite(buffer, pacman, x.px, x.py);  //para que se respete la transparencia

    }

    bool fin_comida(){
        for(int i=0; i<REN; i++){
            for(int j=0; j<COL; j++){
                if(mapa_fondo[i][j]=='o'){
                   return true;
                }
            }
        }
        return false;
    }
};

//clase para el manjeo universal
Control con;

class Tiburon{
public:
    BITMAP *tiburon, *tiburon_bmp;
    SAMPLE *death;
    struct Posicion{
        int Tdireccion;
        int Tx, Ty;
    };
    Posicion pos;
public:
    Tiburon(int, int);
    virtual void dibujar_fantasma()const{}
    void murioElPamanQueNoEsPacman();
    virtual void movimientos_Tiburon()=0;
};

Tiburon::Tiburon(int x, int y){
    pos.Tdireccion=rand()%4;
    pos.Tx=x;
    pos.Ty=y;
    tiburon = create_bitmap(40,40);
    death = load_wav("pacman_death.wav");

}

void Tiburon::murioElPamanQueNoEsPacman(){
    if(con.x.py == pos.Ty && con.x.px == pos.Tx || (pos.Ty == con.x.anteriorpy && pos.Tx == con.x.anteriorpx)){
        destroy_sample(mus_inicio);con.x.vidasTotales--;
        play_sample(death,200,150,1000,0);

        for(int i=0; i<=5; i++){
           clear(pacman);
           clear(buffer);
           con.dibujar_mapa();

           blit(murio_bmp, pacman, i*43, 0, 0, 0, 41, 41);
           draw_sprite(buffer, pacman, con.x.px, con.x.py);

           con.pantalla();
           rest(80);
        }

        //para ponerlo donde debe de ir
        con.x.px = 40*10;
        con.x.py = 40*12;
        con.x.direccion = 4;
    }
}

class TiburonRojo: public Tiburon{
    BITMAP *tiburon_rojo;
public:
    TiburonRojo(int i, int p): Tiburon(i,p){ tiburon_rojo = load_bitmap("shark_red.bmp", NULL);}

    void dibujar_fantasma()const{
        blit(tiburon_rojo, tiburon, 0, 0, 0, 0, 41, 41);//dibujar personaje
        draw_sprite(buffer, tiburon, pos.Tx, pos.Ty);  //para que se respete la transparencia
    }
    void movimientos_Tiburon(){
    dibujar_fantasma();
    murioElPamanQueNoEsPacman();

    if(mapa_fondo[pos.Ty/40][pos.Tx/40] == '|'){
        pos.Tdireccion=rand()%4;
    }
    if(pos.Tdireccion == 0){
        if(mapa_fondo[pos.Ty/40][(pos.Tx-40)/40] != 'X'){
            pos.Tx-=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 1){
        if(mapa_fondo[pos.Ty/40][(pos.Tx+40)/40] != 'X'){
            pos.Tx+=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 2){
        if(mapa_fondo[(pos.Ty-40)/40][pos.Tx/40] != 'X'){
            pos.Ty-=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 3){
        if(mapa_fondo[(pos.Ty+40)/40][pos.Tx/40] != 'X'){
            pos.Ty+=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }

    //para el atajo
        if(pos.Tx <= -40){
           pos.Tx=880;
        }else if(pos.Tx >= 880){
            pos.Tx =-40;
        }
    //fin para el atajo
    }
};

class TiburonCian: public Tiburon{
    BITMAP *tiburon_cian;
public:
    TiburonCian(int i, int p): Tiburon(i,p){ tiburon_cian = load_bitmap("shark_cian.bmp", NULL);}

    void dibujar_fantasma()const{
        blit(tiburon_cian, tiburon, 0, 0, 0, 0, 41, 41);//dibujar personaje
        draw_sprite(buffer, tiburon, pos.Tx, pos.Ty);  //para que se respete la transparencia
    }
    void movimientos_Tiburon(){
    dibujar_fantasma();
    murioElPamanQueNoEsPacman();

    if(mapa_fondo[pos.Ty/40][pos.Tx/40] == '|'){
        pos.Tdireccion=rand()%4;
    }
    if(pos.Tdireccion == 0){
        if(mapa_fondo[pos.Ty/40][(pos.Tx-40)/40] != 'X'){
            pos.Tx-=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 1){
        if(mapa_fondo[pos.Ty/40][(pos.Tx+40)/40] != 'X'){
            pos.Tx+=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 2){
        if(mapa_fondo[(pos.Ty-40)/40][pos.Tx/40] != 'X'){
            pos.Ty-=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 3){
        if(mapa_fondo[(pos.Ty+40)/40][pos.Tx/40] != 'X'){
            pos.Ty+=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }

    //para el atajo
        if(pos.Tx <= -40){
           pos.Tx=880;
        }else if(pos.Tx >= 880){
            pos.Tx =-40;
        }
    //fin para el atajo
    }
};

class TiburonNaranja: public Tiburon{
    BITMAP *tiburon_naranja;
public:
    TiburonNaranja(int i, int p): Tiburon(i,p){ tiburon_naranja = load_bitmap("shark_orange.bmp", NULL);}

    void dibujar_fantasma()const{
        blit(tiburon_naranja, tiburon, 0, 0, 0, 0, 41, 41);//dibujar personaje
        draw_sprite(buffer, tiburon, pos.Tx, pos.Ty);  //para que se respete la transparencia
    }
    void movimientos_Tiburon(){
    dibujar_fantasma();
    murioElPamanQueNoEsPacman();

    if(mapa_fondo[pos.Ty/40][pos.Tx/40] == '|'){
        pos.Tdireccion=rand()%4;
    }
    if(pos.Tdireccion == 0){
        if(mapa_fondo[pos.Ty/40][(pos.Tx-40)/40] != 'X'){
            pos.Tx-=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 1){
        if(mapa_fondo[pos.Ty/40][(pos.Tx+40)/40] != 'X'){
            pos.Tx+=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 2){
        if(mapa_fondo[(pos.Ty-40)/40][pos.Tx/40] != 'X'){
            pos.Ty-=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 3){
        if(mapa_fondo[(pos.Ty+40)/40][pos.Tx/40] != 'X'){
            pos.Ty+=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }

    //para el atajo
        if(pos.Tx <= -40){
           pos.Tx=880;
        }else if(pos.Tx >= 880){
            pos.Tx =-40;
        }
    //fin para el atajo
    }
};

class TiburonRosa: public Tiburon{
    BITMAP *tiburon_Rosa;
public:
    TiburonRosa(int i, int p): Tiburon(i,p){ tiburon_Rosa = load_bitmap("shark_pink.bmp", NULL);}

    void dibujar_fantasma()const{
        blit(tiburon_Rosa, tiburon, 0, 0, 0, 0, 41, 41);//dibujar personaje
        draw_sprite(buffer, tiburon, pos.Tx, pos.Ty);  //para que se respete la transparencia
    }
    void movimientos_Tiburon(){
    dibujar_fantasma();
    murioElPamanQueNoEsPacman();

    if(mapa_fondo[pos.Ty/40][pos.Tx/40] == '|'){
        pos.Tdireccion=rand()%4;
    }
    if(pos.Tdireccion == 0){
        if(mapa_fondo[pos.Ty/40][(pos.Tx-40)/40] != 'X'){
            pos.Tx-=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 1){
        if(mapa_fondo[pos.Ty/40][(pos.Tx+40)/40] != 'X'){
            pos.Tx+=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 2){
        if(mapa_fondo[(pos.Ty-40)/40][pos.Tx/40] != 'X'){
            pos.Ty-=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }
    if(pos.Tdireccion == 3){
        if(mapa_fondo[(pos.Ty+40)/40][pos.Tx/40] != 'X'){
            pos.Ty+=40;
        }else{
            pos.Tdireccion = rand()%4;
        }
    }

    //para el atajo
        if(pos.Tx <= -40){
           pos.Tx=880;
        }else if(pos.Tx >= 880){
            pos.Tx =-40;
        }
    //fin para el atajo
    }
};


void Juego(){
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 880, 770, 0, 0);
    buffer=create_bitmap(880, 770);
    textout(buffer,font,"A", 165,120, makecol(200,155,155));

    //polimorfismo
    TiburonRojo E(40*12, 40*10);
    Tiburon *dady=&E;

    TiburonCian A(40*13, 40*10);
    TiburonNaranja R(40*14, 40*10);
    TiburonRosa I(40*15, 40*10);

    play_sample(mus_inicio,200,150,1000,0);

    while(!key[KEY_ESC]){

    if(con.x.vidasTotales > 0 && con.fin_comida()==true){
        con.x.anteriorpx=con.x.px;
        con.x.anteriorpy=con.x.py;

        //con esTo se supone que se mueve pacman
        if(key[KEY_RIGHT]) con.x.direccion = 1;
        else if(key[KEY_LEFT]) con.x.direccion = 0;
        else if(key[KEY_UP]) con.x.direccion = 2;
        else if(key[KEY_DOWN]) con.x.direccion = 3;
        if(con.x.direccion == 0){
            if(mapa_fondo[con.x.py/40][(con.x.px-40)/40] != 'X'){
                con.x.px -= 40;
            }
        }
        if(con.x.direccion == 1){
            if(mapa_fondo[con.x.py/40][(con.x.px+40)/40] != 'X'){
                con.x.px += 40;
            }
        }
        if(con.x.direccion == 2){
            if(mapa_fondo[(con.x.py-40)/40][con.x.px/40] != 'X'){
                con.x.py -= 40;
            }
        }
        if(con.x.direccion == 3){
            if(mapa_fondo[(con.x.py+40)/40][con.x.px/40] != 'X'){
                con.x.py += 40;
            }
        }
        //fin con esgo se supone que se mueve pacman

        //para el atajo
        if(con.x.px <= -40){
            con.x.px=880;
        }else if(con.x.px >= 880){
            con.x.px =-40;
        }
        //fin para el atajo


        clear(buffer);
        con.dibujar_mapa();
        con.dibujar_personaje();

        A.movimientos_Tiburon();
        R.movimientos_Tiburon();
        I.movimientos_Tiburon();
        E.movimientos_Tiburon();    //es el del polimorfismo

        con.pantalla();
        rest(100);

        clear(pacman);
        blit(pacbmp, pacman, 4*43, 0, 0, 0, 43, 43);//dibujar personaje
        draw_sprite(buffer, pacman, con.x.px, con.x.py);  //para que se respete la transparencia
        con.pantalla();
        rest(70);
    }else{
        while(!key[KEY_ESC]){
            char points[20], cadena[30]="Tu puntaje es: ";
            itoa(con.getPuntos(), points, 10);
            strcat(cadena, points);
            clear(buffer);
                if(con.x.vidasTotales == 0){//SI PIERDE
                    play_sample(muerte_final,200,150,1000,0);
                       blit(lose, buffer, 0, 0, 0, 0, 880, 770);
                       textout(buffer,font,cadena,220,350,pallete_color[10]);
                    con.pantalla();
                }else{//EN CASO DE QUE GANE
                     blit(win, buffer, 0, 0, 0, 0, 880, 770);
                       textout(buffer,font,cadena,220,350,pallete_color[10]);
                        con.pantalla();
                }
        }
    }

    }
}

void Registro(){    //no tocar tampoco
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 500, 300, 0, 0);
    SAMPLE *btn = load_wav("ding.wav");
    set_window_title("                                          BIENVENIDO                                        ");

      char nom[30];
        char c;
        int i=0, x=165, y=120;
        textout(screen, font,"Registrate/Inicia sesion: ", 130, 80, makecol(150,10,0));
        textout(screen, font,"Usuario: ", 165, 100, makecol(150,10,1));
        while(c!=KEY_ENTER){
            c = readkey() >>8;
            switch(c){

                case KEY_A: textout(screen,font,"A", x,y, makecol(255,255,255));fflush(stdin);nom[i]='A';break;
                case KEY_B: textout(screen,font,"B", x,y, makecol(200,155,155));fflush(stdin);nom[i]='B';break;
                case KEY_C: textout(screen,font,"C", x,y, makecol(200,155,155));fflush(stdin);nom[i]='C';break;
                case KEY_D: textout(screen,font,"D", x,y, makecol(200,155,155));fflush(stdin);nom[i]='D';break;
                case KEY_E: textout(screen,font,"E", x,y, makecol(200,155,155));fflush(stdin);nom[i]='E';break;
                case KEY_F: textout(screen,font,"F", x,y, makecol(200,155,155));fflush(stdin);nom[i]='F';break;
                case KEY_G: textout(screen,font,"G", x,y, makecol(200,155,155));fflush(stdin);nom[i]='G';break;
                case KEY_H: textout(screen,font,"H", x,y, makecol(200,155,155));fflush(stdin);nom[i]='H';break;
                case KEY_I: textout(screen,font,"I", x,y, makecol(200,155,155));fflush(stdin);nom[i]='I';break;
                case KEY_J: textout(screen,font,"J", x,y, makecol(200,155,155));fflush(stdin);nom[i]='J';break;
                case KEY_K: textout(screen,font,"K", x,y, makecol(200,155,155));fflush(stdin);nom[i]='K';break;
                case KEY_L: textout(screen,font,"L", x,y, makecol(200,155,155));fflush(stdin);nom[i]='L';break;
                case KEY_M: textout(screen,font,"M", x,y, makecol(200,155,155));fflush(stdin);nom[i]='M';break;
                case KEY_N: textout(screen,font,"N", x,y, makecol(200,155,155));fflush(stdin);nom[i]='N';break;
                case KEY_O: textout(screen,font,"O", x,y, makecol(200,155,155));fflush(stdin);nom[i]='O';break;
                case KEY_P: textout(screen,font,"P", x,y, makecol(200,155,155));fflush(stdin);nom[i]='P';break;
                case KEY_Q: textout(screen,font,"Q", x,y, makecol(200,155,155));fflush(stdin);nom[i]='Q';break;
                case KEY_R: textout(screen,font,"R", x,y, makecol(200,155,155));fflush(stdin);nom[i]='R';break;
                case KEY_S: textout(screen,font,"S", x,y, makecol(200,155,155));fflush(stdin);nom[i]='S';break;
                case KEY_T: textout(screen,font,"T", x,y, makecol(200,155,155));fflush(stdin);nom[i]='T';break;
                case KEY_U: textout(screen,font,"U", x,y, makecol(200,155,155));fflush(stdin);nom[i]='U';break;
                case KEY_V: textout(screen,font,"V", x,y, makecol(200,155,155));fflush(stdin);nom[i]='V';break;
                case KEY_W: textout(screen,font,"W", x,y, makecol(200,155,155));fflush(stdin);nom[i]='W';break;
                case KEY_X: textout(screen,font,"X", x,y, makecol(200,155,155));fflush(stdin);nom[i]='X';break;
                case KEY_Y: textout(screen,font,"Y", x,y, makecol(200,155,155));fflush(stdin);nom[i]='Y';break;
                case KEY_Z: textout(screen,font,"Z", x,y, makecol(200,155,155));fflush(stdin);nom[i]='Z';break;
                case KEY_SPACE:textout(screen,font,"", x,y, makecol(200,155,155));fflush(stdin);nom[i]=' ';break;
                case KEY_ESC:textout(screen,font,"_", x,y, makecol(200,155,155));fflush(stdin);nom[i]='_';break;
                case KEY_0: textout(screen,font,"0", x,y, makecol(200,155,155));fflush(stdin);nom[i]='0';break;
                case KEY_1: textout(screen,font,"1", x,y, makecol(200,155,155));fflush(stdin);nom[i]='1';break;
                case KEY_2: textout(screen,font,"2", x,y, makecol(200,155,155));fflush(stdin);nom[i]='2';break;
                case KEY_3: textout(screen,font,"3", x,y, makecol(200,155,155));fflush(stdin);nom[i]='3';break;
                case KEY_4: textout(screen,font,"4", x,y, makecol(200,155,155));fflush(stdin);nom[i]='4';break;
                case KEY_5: textout(screen,font,"5", x,y, makecol(200,155,155));fflush(stdin);nom[i]='5';break;
                case KEY_6: textout(screen,font,"6", x,y, makecol(200,155,155));fflush(stdin);nom[i]='6';break;
                case KEY_7: textout(screen,font,"7", x,y, makecol(200,155,155));fflush(stdin);nom[i]='7';break;
                case KEY_8: textout(screen,font,"8", x,y, makecol(200,155,155));fflush(stdin);nom[i]='8';break;
                case KEY_9: textout(screen,font,"9", x,y, makecol(200,155,155));fflush(stdin);nom[i]='9';break;

                case KEY_ENTER:textout(screen,font," ", x,y, makecol(200,155,155));nom[i]=' ';
                play_sample(btn,200,150,1000,0);

                char cont[30];
                char cc;
                int ii=0, xx=165, yy=160;
                textout(screen, font,"Contrasena: ", 165, 140, makecol(150,10,0));
                while(cc!=KEY_ENTER){
                cc= readkey() >>8;
            switch(cc){
                case KEY_A: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='A';break;
                case KEY_B: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='B';break;
                case KEY_C: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='C';break;
                case KEY_D: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='D';break;
                case KEY_E: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='E';break;
                case KEY_F: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='F';break;
                case KEY_G: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='G';break;
                case KEY_H: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='H';break;
                case KEY_I: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='I';break;
                case KEY_J: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='J';break;
                case KEY_K: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='K';break;
                case KEY_L: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='L';break;
                case KEY_M: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='M';break;
                case KEY_N: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='N';break;
                case KEY_O: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='O';break;
                case KEY_P: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='P';break;
                case KEY_Q: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='Q';break;
                case KEY_R: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='R';break;
                case KEY_S: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='S';break;
                case KEY_T: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='T';break;
                case KEY_U: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='U';break;
                case KEY_V: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='V';break;
                case KEY_W: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='W';break;
                case KEY_X: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='X';break;
                case KEY_Y: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='Y';break;
                case KEY_Z: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='Z';break;
                case KEY_SPACE:textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]=' ';break;
                case KEY_ESC:textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='_';break;
                case KEY_0: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='0';break;
                case KEY_1: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='1';break;
                case KEY_2: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='2';break;
                case KEY_3: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='3';break;
                case KEY_4: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='4';break;
                case KEY_5: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='5';break;
                case KEY_6: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='6';break;
                case KEY_7: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='7';break;
                case KEY_8: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='8';break;
                case KEY_9: textout(screen,font,"*", xx,yy, makecol(200,155,155));fflush(stdin);cont[ii]='9';break;

                case KEY_ENTER:textout(screen,font,"", xx,yy, makecol(200,155,155));cont[ii]=' ';
                play_sample(btn,200,150,1000,0);
                Game captura1(nom,cont);    //capturamos el uuario en la clase
                cout<<captura1;
                //captura1.imprimir();  //PRUEBA
                Juego();//llamamos la siguiente funcion
                break;
            }
            ii++;
            xx=xx+8;
            }
            break;
        }
        i++;
        x=x+8;
        }
    allegro_exit();
    readkey();

}

//-------------------GIF---------------------
volatile int exit_program;
void fecha_programa() {//exit_program = TRUE;
    Registro();
}
volatile int ticks;
void tick_counter() { ticks++; }
volatile int milisegundos;
void msec_counter() { milisegundos++; }
//-------------------END_GIF---------------------

void inicio(){  //no tocar
  set_color_depth(32);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 880, 770, 0, 0);
  set_window_title("PAC-MAN");

  exit_program = FALSE;
  LOCK_FUNCTION(fecha_programa);
  LOCK_VARIABLE(exit_program);
  set_close_button_callback(fecha_programa);

  ticks = 0;
  LOCK_FUNCTION(tick_counter);
  LOCK_VARIABLE(ticks);
  install_int_ex(tick_counter, BPS_TO_TIMER(60));

  milisegundos = 0;
  LOCK_FUNCTION(msec_counter);
  LOCK_VARIABLE(milisegundos);
  install_int_ex(msec_counter, MSEC_TO_TIMER(1));


  ///BITMAPS
  buffer = create_bitmap(SCREEN_W, SCREEN_H);

  BITMAP* link_esq[15];
  link_esq[0] = load_bitmap("f1.bmp", NULL);
  link_esq[1] = load_bitmap("f2.bmp", NULL);
  link_esq[2] = load_bitmap("f3.bmp", NULL);
  link_esq[3] = load_bitmap("f4.bmp", NULL);
  link_esq[4] = load_bitmap("f5.bmp", NULL);
  link_esq[5] = load_bitmap("f6.bmp", NULL);
  link_esq[6] = load_bitmap("f7.bmp", NULL);
  link_esq[7] = load_bitmap("f8.bmp", NULL);
  link_esq[8] = load_bitmap("f9.bmp", NULL);
  link_esq[9] = load_bitmap("f10.bmp", NULL);
  link_esq[10] = load_bitmap("f11.bmp", NULL);
  link_esq[11] = load_bitmap("f12.bmp", NULL);
  link_esq[12] = load_bitmap("f13.bmp", NULL);
  link_esq[13] = load_bitmap("f14.bmp", NULL);
  link_esq[14] = load_bitmap("f15.bmp", NULL);
  link_esq[15] = load_bitmap("f16.bmp", NULL);

  play_sample(mus_inicio,200,150,1000,0);

  int num_frames = 17;
  int frame_atual;
  int tempo_troca = 200;

  while(!key[KEY_SPACE]){
     while(ticks > 0 && !exit_program){
      frame_atual = (milisegundos / tempo_troca) % num_frames;

      draw_sprite(buffer, link_esq[frame_atual], 0, 0);

      draw_sprite(screen, buffer, 0, 0);

      clear_to_color(buffer, makecol(255,255,255));

      ticks--;
     }
  }
    if(key[KEY_SPACE]){
        destroy_sample(mus_inicio);
        fecha_programa();
    }

    ///FINAL
  clear(buffer);
  destroy_bitmap(buffer);
  destroy_bitmap(link_esq[0]);
  destroy_bitmap(link_esq[1]);
  destroy_bitmap(link_esq[2]);
  destroy_bitmap(link_esq[3]);
  destroy_bitmap(link_esq[4]);
  destroy_bitmap(link_esq[5]);
  destroy_bitmap(link_esq[6]);
  destroy_bitmap(link_esq[7]);
  destroy_bitmap(link_esq[8]);
  destroy_bitmap(link_esq[9]);
  destroy_bitmap(link_esq[10]);
  destroy_bitmap(link_esq[11]);
  destroy_bitmap(link_esq[12]);
  destroy_bitmap(link_esq[13]);
  destroy_bitmap(link_esq[14]);
  destroy_bitmap(link_esq[15]);
  destroy_bitmap(link_esq[16]);
  destroy_sample(mus_inicio);
}

int main(){
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 880, 770, 0, 0);


    //declaracion de los bitmaps y saples a utilizar
    buffer = create_bitmap(880,770);
    pasto = load_bitmap("pasto_chido.bmp", NULL);
    pacbmp = load_bitmap("pacman_chido2.bmp", NULL);
    pacman = create_bitmap(41,41);
    puntoAmarillo = load_bitmap("punto_amarillo.bmp", NULL);
    cerezas= load_bitmap("lombrisilla.bmp", NULL);
    puntoBlanco= load_bitmap("punto_blanco.bmp", NULL);
    guero = load_wav("ponto.wav");
    fruta = load_wav("fruta.wav");
    blanco = load_wav("blanquito.wav");
    lose= load_bitmap("perder.bmp", NULL);
    win= load_bitmap("ganar.bmp", NULL);
    mus_inicio = load_wav("beggining.wav");
    muerte_final = load_wav("suspense.wav");

    murio_bmp = load_bitmap("muerte_chido.bmp", NULL);

    if(con.nivel==1){
        inicio();
    }

}END_OF_MAIN()


