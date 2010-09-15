#include <stdio.h>
//Instruction and address are read from the file.
//Set and tag are calculated from the instruction and is used to generate an index


void updateLRU(int dataCacheIndex, int dataSet, int dataTag){
    //updates the values of the vlues in the given data Index on the LRUArray
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

int calculateIndex(int dataSet, int dataTag){
    int set = dataSet;
    int tag = dataTag;
    int cacheIndex;
    //this method calculates the index of the cache based on dataSet and dataTag and returns and index
}

void validateData(int index){
    int cacheIndex = index;
    //Make validBitArray[cacheIndex] = 1
}

void writeToAddress(int dataIndex){
    //Write to the current index
    validateData(dataIndex);
}
void writeToCache(int dataSet, int dataTag){
    int set = dataSet;
    int tag = dataTag;
    //Iterate through the set(index) to find the [cacheIndex] with LRU
    writeToAddress(cacheIndex);
}


boolean dataValid(int validIndex){
    //if validBitArray[validIndex] == 1 reutrn true else return false
}

int tagHit(int dataSet, int dataTag){
    //Use index = calculateIndex(set, tag) to find the index
    //Search cache and find the given tag, if found return index
    //Base case return -1
}

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


void main(){
    int setSize = calculateSetSize(C, K, L);
    int lineSize = calculateLineSize(C, K, L);
    int LRUArray[lineSize][setSize], cache[lineSize][setSize], validBitArray[lineSize][setSize];
    int instruction, address, tag, set, cacheIndex;
//Loop for reading the trace and setting instruction
    switch (instruction){
        case 0: break;
        case 1: break;
        case 2: break;
        case 3: break;
        case 4: break;
    }
    tag = decodeTag(address, C, K, L);
    set = decodeSet(address, C, K, L);
    if((cacheIndex = tagHit(set, tag)) > -1){
        if(dataValid(cacheIndex)){
	    updateLRU(cacheIndex, set, tag);
        }else{
            writeToAddress(cacheIndex, tag);
            updateLRU(cacheIndex, set, tag);
            miss(void);
        }
    }else{
        writeToCache(set, tag);
        updateLRU(cacheIndex, set, tag);
        miss(void);
    }
}
