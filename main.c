#include <stdio.h>
//Instruction and address are read from the file.
//Set and tag are calculated from the instruction and is used to generate an index

void main(){
    int LRUArray[], cache[], validBitArray[];
    buildCache(C, K, L);
    int instruction, address, tag, index, cacheIndex;
//Loop for reading the trace and setting instruction
    switch (instruction){
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: break;
        case 4: break;
    }
    tag = decodeTag(address);
    index = decodeIndex(address);
    if((cacheIndex = tagHit(index, tag)) > -1){
        if(dataValid(cacheIndex)){
            //Hit
        }else{
            writeToAddress(cacheIndex, tag);
            miss(void);
        }
    }else{
        writeToCache(set, tag);
        miss(void);
    }
}

void buildCache(int C, int K, int L){
    //decode size

}

int calculateIndex(int dataIndex, dataTag){
    int index = dataIndex;
    int tag = dataTag;
    int cacheIndex;
    //this method calculates the index of the cache based on dataSet and dataTag and returns and index
}

void validateData(int index){
    int cacheIndex = index;
    //Make validBitArray[validIndex] = 1
}

void writeToCache(int dataSet, int dataTag){
    int set = dataSet;
    int tag = dataTag;
    //Find the [index] with matching tag and LRU
    writeToAddress(index);
}

void writeToAddress(int dataIndex, ){
    //Write to the current index
    validateData(dataIndex);
}

boolean dataValid(int validIndex){
    //if validBitArray[validIndex] == 1 reutrn true else return false
}

int tagHit(int dataSet, int dataTag){
    //Use index = calculateIndex(set, tag) to find the index
    //Search cache and find the given tag, if found return index
    //Base case return -1
}

int decodeSet(int address){
    //Logic for decoding set
    return set;
}

int decodeTag(int address){
    //Logic for decoding tag
    return tag;
}

