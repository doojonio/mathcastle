#include <string>
using std::string;
typedef unsigned char ubyte;

void gotoxy(ubyte X1, ubyte Y1);
void drawFrame(ubyte l_X, ubyte r_X, ubyte up_Y, ubyte dn_Y);
void scoreWriter(char name[25], int SCORE);
int play();
void scoreboard();
void help();
bool exit();
void art(ubyte x, ubyte y, string name);
void cursorMenu();
ubyte choice();