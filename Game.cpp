//PAC-MAN
//Mariela Teresa Serna Davila
//Jessica Guerrero Carrera

#include <iostream>
#include <string.h>
#include "Game.h"

using namespace std;

Game::Game(){
    usuario = new char;
    usuario = " ";
    contrasena = new char;
    contrasena = " ";
}
Game::Game(char *user, char *contra){
    int tam;
    tam=strlen(user);
    usuario = new char[tam];
    strcpy(usuario, user);
    tam=strlen(contra);
    contrasena = new char [tam];
    strcpy(contrasena, contra);
}
Game::~Game(){
    delete [] usuario;
    delete [] contrasena;
}

ostream& operator<<(ostream& out, const Game& obj){
    out<<endl<<"Usuario: "<<obj.usuario<<endl;
    out<<"Contrasena: "<<obj.contrasena<<endl;
}
