#include <stdio.h>
#include <math.h>
//Instruction and address are read from the file.
//Set and tag are calculated from the instruction and is used to generate an index

void swit(int array[]){
    array[0] = 2000;
}

int main(){
    int test[5][6];
    int i,j;
    int test1d[30];
    for(i = 0; i<5; i++ ){
        for(j = 0; j<6; j++){
            test[i][j] = j*5+i;
            test1d[j*5+i] = j*5+i;
        }
    }
    for(i = 0; i<5; i++ ){
        for(j = 0; j<6; j++){
            printf("%d ",test[i][j]);
  //          printf("%d",test1d[i*j+i]);
        }
        printf("\n");
    }
    for(i = 0; i<5; i++ ){
        for(j = 0; j<6; j++){
//            printf("%d",test[i][j]);
            printf("%d ",test1d[j*5+i]);
        }
        printf("\n");
    }
        printf("\n");
        printf("\n");
    swit(test1d);
        for(i = 0; i<5; i++ ){
        for(j = 0; j<6; j++){
//            printf("%d",test[i][j]);
            printf("%d ",test1d[j*5+i]);
        }
        printf("\n");
    }

}
