
#include <stdio.h>
#include <math.h>
#include <time.h>


//This method updates the LRU
void updateLRU(int LRUArray[], int dataSet, int dataLine, int lineSize){
    int i;
    int order = LRUArray[dataLine+lineSize*dataSet]; //LRU index of line

    for(i = 0; i<lineSize; i++){
        if(LRUArray[i+lineSize*dataSet] < order){
            LRUArray[i+lineSize*dataSet]++;
        }
    }
    LRUArray[dataLine+lineSize*dataSet] = 0;
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


void validateData(int validBitArray[], int set, int line, int lineSize){
    validBitArray[line+lineSize*set] = 1;
}

void writeToAddress(int tagArray[], int set, int line, int tag, int lineSize){
    tagArray[line+lineSize*set] = tag;
}

int writeToCache(int tagArray[], int LRUArray[], int dataSet, int dataTag, int lineSize){
    int line;
    int i;
    for (i = 0; i<lineSize; i++){
        if(LRUArray[i+lineSize*dataSet] == (lineSize - 1)){
           line = i;
           }
    }
    //Iterate through the set(index) to find the [line][set] with LRU
    writeToAddress(tagArray, dataSet, line, dataTag, lineSize);
    return line;
}


int dataValid(int validBitArray[], int set, int line, int lineSize){
    if(validBitArray[line+lineSize*set]==1){
        return 1;
    }
    else{
        return -1;
    }
}

int tagHit(int tagArray[],int dataSet, int dataTag, int lines){
    int i;
    for(i = 0; i<lines; i++){
        if(tagArray[i+lines*dataSet] == dataTag){
            return i;
        }
    }
    return -1;
}

int decodeSet(int address, int C, int K, int L, int sets){
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

int decodeTag(int address, int C, int K, int L, int sets){
    int tag;
    int shiftamount = log2(L) + log2(sets);
    tag = address >> shiftamount;
    return tag;
}

void clearCache(int validBitArray[], int lineSize, int setSize){
    int i;
    for(i = 0; i<lineSize*setSize; i++){
        validBitArray[i] = 0;
    }
}

void printCache(int tagArray[], int validBitArray[], int LRUArray[], int set, int line){
    int i,j;

    for(j = 0; j<set; j++){

        printf("\n Set %d \n", j);
        for(i = 0; i<line; i++ ){
//          printf("%d",test[i][j]);
            printf("%d | %3d | %10x \n", validBitArray[j*line+i], LRUArray[j*line+i], tagArray[j*line+i]);
        }
    }
    printf("\n");

}

void simulator(int C, int K, int L){

    int setSize = calculateSetSize(C, K, L);
    int lineSize = calculateLineSize(C, K, L);
    int LRUArray[lineSize*setSize], tagArray[lineSize*setSize], validBitArray[lineSize*setSize];
    //Initializes all array entries to 0
    int i,j;
    //Initializing every LRU of the cache to a value between 0 - (lineSize-1)
    for(i = 0; i<lineSize; i++ ){
        for(j = 0; j<setSize; j++){
            LRUArray[i+j*lineSize] = i;
            validBitArray[i+j*lineSize] = 0;
            tagArray[i+j*lineSize] = 0;
        }

    }

    int iter, instruction, address, tag, set, setLine;
    double missNumber, totalMemoryReferences; //miss will take care of maintaining track of how many misses the simulator has, toatlIteratiosn just counts how many total addresses the simulator has.
    missNumber = 0;
    totalMemoryReferences = 0;
    instruction = 1;
    iter = 0;
    srand(4342);
    while(iter < 100000){

        address = rand();
        //printf("%d\n", address);
        switch (instruction){ // cases 0 -3 run the same thing
            case 0:
            case 1:
            case 2:
                tag = decodeTag(address, C, K, L, setSize);
                set = decodeSet(address, C, K, L, setSize);
                //Printing information
                //printf("Address: %x \n Tag: %d \n Set: %d \n\n", address, tag, set);
                if((setLine = tagHit(tagArray, set, tag, lineSize)) > -1){
                    if(dataValid(validBitArray, set, setLine, lineSize)>0){
                        updateLRU(LRUArray, set, setLine, lineSize);
                    }else{
                        writeToAddress(tagArray, set, tag, setLine, lineSize);
                        validateData(validBitArray, set, setLine, lineSize);
                        updateLRU(LRUArray, set, setLine, lineSize);
                        missNumber++;
                    }
                }else{
                    setLine = writeToCache(tagArray, LRUArray, set, tag, lineSize);
                    validateData(validBitArray, set, setLine, lineSize);
                    updateLRU(LRUArray, set, setLine, lineSize);
                    missNumber++;
                }
                break;

            case 3:break;
            case 4:
                clearCache(validBitArray, lineSize, setSize);
                break;
        }
        iter++;
        totalMemoryReferences++;
    }

    double missRate = missNumber/totalMemoryReferences*100;
    //printCache(tagArray, validBitArray, LRUArray, setSize, lineSize);
    printf("%.0f misses, ", missNumber);
    printf("%.0f total memory references, ", totalMemoryReferences);
    printf("%2.4f  miss rate, \n\n", missRate);
}

int main(){
    int i;
    printf("Part b) \n");
    for (i =64; i<5000; i=i*2){
        simulator(i, 4, 16);
    }

}
