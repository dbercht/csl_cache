#include <stdio.h>
#include <math.h>


//This method updates the LRU
//Each entry in the LRU has a value of [0 - (setSize - 1)], and every time a value is updated in
//the array, the entries with a value smaller than the order of the entry that was changed is inremented by one
//the Most Recently Used entry is then modified to 0. This way, the LRU will always be the entry with (setSize - 1)
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

//Line size is the value of K
int calculateLineSize(int C, int K, int L){
    int size; //Logic for cache size
    size = K;
    return size;
}

//The number of sets is (Cache Size)/(Number of sets * Bytes per Line)
int calculateSetSize(int C, int K, int L){
    int size; //Logic for cache size
    size = C/(K*L);
    return size;
}

//This method takes a given index and modifies the validBit of the given entry to 1
void validateData(int validBitArray[], int set, int line, int lineSize){
    validBitArray[line+lineSize*set] = 1;
}

//This value writes the tag value to the given index given by the index (set*lineSize + line)
void writeToAddress(int tagArray[], int set, int line, int tag, int lineSize){
    tagArray[line+lineSize*set] = tag;
}

//This value checks for the index of the entry with the largest LRU value then writes to the given index
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


//Checks for the valid bit of the given index and returns 1 for valid, -1 for invalid
int dataValid(int validBitArray[], int set, int line, int lineSize){
    if(validBitArray[line+lineSize*set]==1){
        return 1;
    }
    else{
        return -1;
    }
}

//This method checks if the tag is found in a given set
int tagHit(int tagArray[],int dataSet, int dataTag, int lines){
    int i;
    for(i = 0; i<lines; i++){
        if(tagArray[i+lines*dataSet] == dataTag){
            return i;
        }
    }
    return -1;
}

//This method decodes the set by bitshifting the offset, and then anding the set field with all ones, and zeroes in all other positions
//in order to isolate the set number.
int decodeSet(int address, int C, int K, int L, int sets){
    int set;
    int andAmount = log2(L)+log2(sets);
    int byteOffset = log2(L);
    andAmount = pow(2, (andAmount)) - 1;
    set = (address & andAmount);

    set = set >> (byteOffset);
    return set;
}

//Decodes the tag by shifting the offset and set out.
int decodeTag(int address, int C, int K, int L, int sets){
    int tag;
    int shiftamount = log2(L) + log2(sets);
    tag = address >> shiftamount;
    return tag;
}

//This method replaces all values in the validBitArray to 0 in order to devalidate the cache
void clearCache(int validBitArray[], int lineSize, int setSize){
    int i;
    for(i = 0; i<lineSize*setSize; i++){
        validBitArray[i] = 0;
    }
}

//Simple method to print the entire cache
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


void simulatorPartF(int C, int K, int L){

    int setSize = calculateSetSize(C, K, L);
    int lineSize = calculateLineSize(C, K, L);
    int LRUArray[lineSize*setSize], tagArray[lineSize*setSize], validBitArray[lineSize*setSize];
    //Initializes all array entries to 0
    int i,j;
    printf("%d Cache, %d Lines, %d Bytes Per Block\n", C, K, L);
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
    address = 0;
    while(address < 4000){
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
        address= address+4;
        totalMemoryReferences++;
        int scan;
        //printCache(tagArray, validBitArray, LRUArray, setSize, lineSize);
    }

    double missRate = missNumber/totalMemoryReferences*100;
    printf("%.0f misses, ", missNumber);
    printf("%.0f total memory references, ", totalMemoryReferences);
    printf("%2.4f  miss rate, \n\n", missRate);
}

void simulatorPartE(int C, int K, int L){

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

void simulatorPartD(int C, int K, int L){

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
                //printf("Address: %x \n Tag: %d \n Set: %d \n\n", address, tag, set);
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

void simulator(int C, int K, int L){

    //Calculates number of sets, size of lines and initializes the three arrays LRUArray, tagArray, validBitArray for the simulation
    int setSize = calculateSetSize(C, K, L);
    int lineSize = calculateLineSize(C, K, L);
    int LRUArray[lineSize*setSize], tagArray[lineSize*setSize], validBitArray[lineSize*setSize];
    //Initializes all array entries to 0
    int i,j;
    printf("%d Cache Size, %d Lines, %d Bytes Per Block \n", C, K, L);
    //Initializing every LRU of the cache to a value between 0 - (lineSize-1)
    for(i = 0; i<lineSize; i++ ){
        for(j = 0; j<setSize; j++){
            LRUArray[i+j*lineSize] = i;
            validBitArray[i+j*lineSize] = 0;
            tagArray[i+j*lineSize] = 0;
        }

    }

    int instruction, address, tag, set, setLine;
    double missNumber, totalMemoryReferences; //miss will take care of maintaining track of how many misses the simulator has, toatlIteratiosn just counts how many total addresses the simulator has.
    missNumber = 0;
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
            case 2:
            //Decoding tag and set for given address
                tag = decodeTag(address, C, K, L, setSize);
                set = decodeSet(address, C, K, L, setSize);
                //Printing information
                //printf("Address: %x \n Tag: %x \n Set: %d \n\n", address, tag, set);
                //setLine will become an index value if there is a tag hit in the given set
                if((setLine = tagHit(tagArray, set, tag, lineSize)) > -1){
                    //If there is a tag hit, the simulator checks if the data in the given index is valid
                    if(dataValid(validBitArray, set, setLine, lineSize)>0){
                        //If the data is valid, simulator updates LRU
                        updateLRU(LRUArray, set, setLine, lineSize);
                    }else{
                        //If data is not valid, simulator writes given tag to given address, validates data, and updates LRU, and the miss is accoutned for
                        writeToAddress(tagArray, set, tag, setLine, lineSize);
                        validateData(validBitArray, set, setLine, lineSize);
                        updateLRU(LRUArray, set, setLine, lineSize);
                        missNumber++;
                    }
                }else{
                    //If the tag is not found within the set, writeToCache returns the index of the LRU, the miss is accoutned for.
                    setLine = writeToCache(tagArray, LRUArray, set, tag, lineSize);
                    validateData(validBitArray, set, setLine, lineSize);
                    updateLRU(LRUArray, set, setLine, lineSize);
                    missNumber++;
                }
                break;

            case 3:break;
            case 4:
                //if instruction is 4, the cache is cleared
                clearCache(validBitArray, lineSize, setSize);
                break;
        }
        totalMemoryReferences++;
        //End iteration
    }

    fclose(ifp); //  Close file
    double missRate = missNumber/totalMemoryReferences*100;
    //printCache(tagArray, validBitArray, LRUArray, setSize, lineSize);
    printf("%.0f misses, ", missNumber);
    printf("%.0f total memory references, ", totalMemoryReferences);
    printf("%2.4f  miss rate, \n\n", missRate);
}

/************************************************************

PART A, B, C
************************************************************/
int main(){
    printf("hi");
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
}

/************************************************************
PART A, B, C

************************************************************/

/************************************************************

PART D
************************************************************/
/*
int main(){
    simulatorPartD(1024, 4, 16);

}
*/
/***********************************************************
PART D

************************************************************/

/************************************************************

PART E
************************************************************/
/*
int main(){
    int i;
    printf("Part b) \n");
    for (i =64; i<5000; i=i*2){
        simulatorPartE(i, 4, 16);
    }

}
*/

/************************************************************
PART E

************************************************************/


/************************************************************

PART F
************************************************************/
/*
int main(){
    int i;
    printf("Part c) \n");
    for (i =1; i<2050; i=i*2){
        simulatorPartF(1024, 2, i);
    }
}
*/
/************************************************************
PART F

************************************************************/

