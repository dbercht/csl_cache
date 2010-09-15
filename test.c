#include <stdio.h>
#include <math.h>
//Instruction and address are read from the file.
//Set and tag are calculated from the instruction and is used to generate an index


int decodeIndex(int address, int C, int K, int L, int sets){
    int set;
    int shiftAmount = log2(L);
    int andAmount = log2(C)-log2(K)-log2(L);
    set = address >> shiftAmount;
    int andValue = pow(2, andAmount);
    andValue--;
    set = set & andValue;
    set = set%sets;
    return set;
}

int decodeTag(int address, int C, int K, int L){
    int tag;
    int shiftAmount = log2(K) + log2(L) + log2(L);
    tag = address >> shiftAmount;
    return tag;
}

int calculateLineSize(int C, int K, int L){
    int size; //Logic for cache size
    size = K;
    return size;
}

int calculateSetSize(int C, int K, int L){
    int size; //Logic for cache size
    size = C/(K*L);
    return size;
}

int main(void){
    int C = pow(2, 10);
    int L = pow(2, 2);
    int K = 2;
    int tag, set;
    int lines = calculateLineSize(C, K, L);
    printf("%d %d %d\n", C, L, K);
    int sets = calculateSetSize(C, K, L);
    for(int address = 1; address<1064; address = address*2){
        tag = decodeTag(address, C, K, L);
        set = decodeIndex(address, C, K, L, sets);
        printf("%d, set: %d, tag: %d\n", address, set, tag);
    }
}
