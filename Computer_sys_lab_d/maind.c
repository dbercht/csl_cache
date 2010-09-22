#include <stdio.h>
#include <math.h>


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
    //First set of arrays for data
    int LRUArray[lineSize*setSize], tagArray[lineSize*setSize], validBitArray[lineSize*setSize];
    //First set of arrays for data
    int dataLRUArray[lineSize*setSize], dataTagArray[lineSize*setSize], dataValidBitArray[lineSize*setSize];
    //Second set of arrays for instruction
    int instructionLRUArray[lineSize*setSize], instructionTagArray[lineSize*setSize], instructionValidBitArray[lineSize*setSize];
    //Initializes all array entries to 0
    int i,j;
    //Initializing every LRU of the cache to a value between 0 - (lineSize-1)
    for(i = 0; i<lineSize; i++ ){
        for(j = 0; j<setSize; j++){
            LRUArray[i+j*lineSize] = i;
            validBitArray[i+j*lineSize] = 0;
            tagArray[i+j*lineSize] = 0;
            dataLRUArray[i+j*lineSize] = i;
            dataValidBitArray[i+j*lineSize] = 0;
            dataTagArray[i+j*lineSize] = 0;
            instructionLRUArray[i+j*lineSize] = i;
            instructionValidBitArray[i+j*lineSize] = 0;
            instructionTagArray[i+j*lineSize] = 0;
        }

    }

    int instruction, address, tag, set, setLine;
    double missNumber, totalMemoryReferences, dataMiss, instructionMiss, dataReferences, instructionReferences; //miss will take care of maintaining track of how many misses the simulator has, toatlIteratiosn just counts how many total addresses the simulator has.
    missNumber = 0;
    dataMiss = 0;
    instructionMiss = 0;
    dataReferences = 0;
    instructionReferences = 0;
    totalMemoryReferences = 0;
    instruction = 1;
    address = 0;
    FILE *ifp;   //Pointer to a file is declared
    ifp = fopen("trace.txt", "r"); // ifp points to file
                                  // trace.txt, opened for
                                  // reading
    while (!feof(ifp)) {  // exit if end-of-file reached
    fscanf(ifp, "%d %x", &instruction, &address); // read next line
        switch (instruction){ // cases 0 -3 run the same thing
            case 0:
            case 1:
                tag = decodeTag(address, C, K, L, setSize);
                set = decodeSet(address, C, K, L, setSize);
                //Printing information
                printf("Address: %x \n Tag: %d \n Set: %d \n\n", address, tag, set);
                if((setLine = tagHit(dataTagArray, set, tag, lineSize)) > -1){
                    if(dataValid(dataValidBitArray, set, setLine, lineSize)>0){
                        updateLRU(dataLRUArray, set, setLine, lineSize);
                    }else{
                        writeToAddress(dataTagArray, set, tag, setLine, lineSize);
                        validateData(dataValidBitArray, set, setLine, lineSize);
                        updateLRU(dataLRUArray, set, setLine, lineSize);
                        dataMiss++;
                        missNumber++;
                    }
                }else{
                    setLine = writeToCache(dataTagArray, dataLRUArray, set, tag, lineSize);
                    validateData(dataValidBitArray, set, setLine, lineSize);
                    updateLRU(dataLRUArray, set, setLine, lineSize);
                    dataMiss++;
                    missNumber++;
                }
                dataReferences++;
                totalMemoryReferences++;
                break;

            case 2:
                tag = decodeTag(address, C, K, L, setSize);
                set = decodeSet(address, C, K, L, setSize);
                //Printing information
                //printf("Address: %x \n Tag: %d \n Set: %d \n\n", address, tag, set);
                if((setLine = tagHit(instructionTagArray, set, tag, lineSize)) > -1){
                    if(dataValid(instructionValidBitArray, set, setLine, lineSize)>0){
                        updateLRU(instructionLRUArray, set, setLine, lineSize);
                    }else{
                        writeToAddress(instructionTagArray, set, tag, setLine, lineSize);
                        validateData(instructionValidBitArray, set, setLine, lineSize);
                        updateLRU(instructionLRUArray, set, setLine, lineSize);
                        instructionMiss++;
                        missNumber++;
                    }
                }else{
                    setLine = writeToCache(instructionTagArray, LRUArray, set, tag, lineSize);
                    validateData(instructionValidBitArray, set, setLine, lineSize);
                    updateLRU(instructionLRUArray, set, setLine, lineSize);
                    instructionMiss++;
                    missNumber++;
                }
                instructionReferences++;
                totalMemoryReferences++;

                break;
            //End iteration
        //address++;
            case 3:break;
            case 4:
                clearCache(dataValidBitArray, lineSize, setSize);
                clearCache(instructionValidBitArray, lineSize, setSize);
                break;
        }
    }
    fclose(ifp); //  Close file
    double missRate = missNumber/totalMemoryReferences*100;
    double instructionMissRate = instructionMiss/totalMemoryReferences*100;
    double dataMissRate = dataMiss/totalMemoryReferences*100;
    //printCache(tagArray, validBitArray, LRUArray, setSize, lineSize);
    printf("%.0f misses, ", missNumber);
    printf("%.0f total memory references, ", totalMemoryReferences);
    printf("%2.4f  miss rate, \n", missRate);

    printf("%.0f misses, ", dataMiss);
    printf("%.0f total memory references, ", totalMemoryReferences);
    printf("%2.4f data miss rate, \n", dataMissRate);

    printf("%.0f misses, ", instructionMiss);
    printf("%.0f total memory references, ", totalMemoryReferences);
    printf("%2.4f instruction miss rate, \n\n", instructionMissRate);
}

int main(){
    simulator(1024, 1, 16);
/*
    int i;
    printf("Part a) \n");
    for (i =4; i<257; i=i*2){
        simulator(1024, 1, i);
    }

    printf("Part b) \n");
    for (i =64; i<10000; i=i*2){
        simulator(i, 4, 16);
    }

    printf("Part c) \n");
    for (i =1; i<20; i=i*2){
        simulator(256, i, 16);
    }
    */
}
