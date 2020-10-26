#include <iostream>
#include <cstdlib>
#include <stdio.h>

using namespace std;

/* Prototipos */
int clearScreen();
int pauseScreen();

/* ********** */





/* Clear and pause */
int clearScreen(){
    system("clear");
    return 0;
}

int pauseScreen(){
    cout<<"Presione enter para continuar..."<<'\n';
    system("read _");
    return 0;
}
/* *************** */