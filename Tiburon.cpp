#include <iostream>
#include "Tiburon.h"
#include <allegro.h>

#define REN 20
#define COL 31

using namespace std;
/*
Tiburon::Tiburon(int x, int y){
    Tdireccion=rand()%4;
    Tx=x;
    Ty=y;

    buffer = create_bitmap(880,770);
    tiburon = create_bitmap(40,40);
    tiburon_bmp = load_bitmap("shark.bmp", NULL);
    death = load_wav("pacman_death.wav");

}
void Tiburon::dibujar_fantasma()const{
    blit(tiburon_bmp, tiburon, 0, 0, 0, 0, 43, 43);//dibujarPacman();//dibujar personaje
    draw_sprite(buffer, tiburon, Tx, Ty);  //para que se respete la transparencia
}

void Tiburon::movimientos_Tiburon(){
    dibujar_fantasma();
    murioElPamanQueNoEsPacman();

    if(mapa_fondo[Ty/40][Tx/40] == '|'){
        Tdireccion=rand()%4;
    }
    if(Tdireccion == 0){
        if(mapa_fondo[Ty/40][(Tx-40)/40] != 'X'){
            Tx-=40;
        }else{
            Tdireccion = rand()%4;
        }
    }
    if(Tdireccion == 1){
        if(mapa_fondo[Ty/40][(Tx+40)/40] != 'X'){
            Tx+=40;
        }else{
            Tdireccion = rand()%4;
        }
    }
    if(Tdireccion == 2){
        if(mapa_fondo[(Ty-40)/40][Tx/40] != 'X'){
            Ty-=40;
        }else{
            Tdireccion = rand()%4;
        }
    }
    if(Tdireccion == 3){
        if(mapa_fondo[(Ty+40)/40][Tx/40] != 'X'){
            Ty+=40;
        }else{
            Tdireccion = rand()%4;
        }
    }

    //para el atajo
        if(Tx <= -40){
            Tx=880;
        }else if(Tx >= 880){
            Tx =-30;
        }
    //fin para el atajo
}

void Tiburon::murioElPamanQueNoEsPacman(){
    if(py == Ty && px == Tx || (Ty == anteriorpy && Tx == anteriorpx)){
       // destroy_sample(mus_inicio);
      //  vidasTotales--;
        play_sample(death,200,150,1000,0);

        for(int i=0; i<=5; i++){
           clear(pacman);
           clear(buffer);
           dibujar_mapa();

           blit(murio_bmp, pacman, i*43, 0, 0, 0, 43, 43);
           draw_sprite(buffer, pacman, px, py);

           pantalla();
           rest(80);
        }

        //para ponerlo donde debe de ir
        px = 40*10;
        py = 40*12;
        direccion = 4;
    }
}
*/
