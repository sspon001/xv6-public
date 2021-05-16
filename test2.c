#include "test.h"

int
main(int argc, char *argv[]){
    set_prior(15) ;
    int i, k ;
    for(i = 0 ; i < 50000 ; i++){
        asm("nop") ;
        for(k = 0 ; k < 50000 ; k++) asm("nop") ;
    }
    exit() ;
}