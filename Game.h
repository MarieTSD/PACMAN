//PAC-MAN
//Mariela Teresa Serna Davila
//Jessica Guerrero Carrera

#ifndef Game_h
#define Game_h
#include <allegro.h>


using namespace std;

class Game{
    friend ostream& operator<< (ostream&, const Game& );
private:
    char *usuario, *contrasena;
public:
    Game();
    Game(char *, char *);
    ~Game();
};
#endif // Game_h
