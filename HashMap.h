//
// Created by PC on 26.05.2022.
//

#ifndef COALESCED_HASHING_HASHMAP_H
#define COALESCED_HASHING_HASHMAP_H
#include <iostream>
#include <cmath>
#include "DynamicArr.h"
using namespace std;

class ArrayOfPrimeNum{
private:
    int oldsize = 1;
public:
    DynamicArr<int> PrimeArray;
    void set_newArr(int sizeforArray){
        int sizeOfArray = (sizeforArray) / 2 + 1;
        int *arr = new int[sizeOfArray];
        arr[0] = 2;
        for (int number = 3, j = 1; j < sizeOfArray; number += 2, j++) {
            arr[j] = number;
        }
        for (int placeOfNum = 1, step = 3; placeOfNum < sqrt(sizeOfArray); placeOfNum++, step += 2) {
            if (arr[placeOfNum] != 0) {
                for (int i = placeOfNum + step; i < sizeOfArray; i += step) {
                    arr[i] = 0;
                }
            }
        }
        for (int i = oldsize-1;i < sizeOfArray; i++){
            if (arr[i] != 0) {
                PrimeArray.push_back(arr[i]);
            }
        }
        oldsize = sizeOfArray;

        delete [] arr;
        return;
    }
    int FindNearest(int searchElement){
        int leftBord=0;
        int rightBord=PrimeArray.size()-1;
        while (rightBord > leftBord){
            int mid=(leftBord+rightBord)/2;
            if(PrimeArray.Array[mid]==searchElement){
                return PrimeArray.Array[mid];
            }
            if(PrimeArray.Array[mid]>searchElement){
                rightBord=mid-1;
            }
            else{
                leftBord=mid+1;
            }
        }
        if(rightBord != PrimeArray.size()-1)
            return PrimeArray.Array[rightBord+1];
        else
            return PrimeArray.Array[rightBord];
    }
    int get_last(){
        return PrimeArray.Array[(PrimeArray.size())-1];
    }
};

template<typename T>
struct HashMap
{
private:
    struct HashSigment{
        long long key;
        T data;
        int PSL = 0;
        bool Empty = 1;
    };
    void swap(HashSigment* a,HashSigment* b){
        HashSigment temp = *a;
        *a = *b;
        *b = temp;
    }
    HashSigment* array;
    int size, arrlen = 11;
    float coefload;
    int a;
    int b;

    ArrayOfPrimeNum PrimeArr;


    long long hash(long long key) const
    {
        return ((key * a + b) % 9149658775000477) % (arrlen);
    }


    void resize_arr()
    {
        int old_capacity = arrlen;
        arrlen = PrimeArr.FindNearest(arrlen * 2.0);
        PrimeArr.set_newArr(arrlen * 3);
        HashSigment *arr = new HashSigment[arrlen];

        for(int i = 0; i < old_capacity; i++)
        {
            if(array[i].Empty == 0){
                insertnewarr(array[i].key,array[i].data,arr);
            }
        }
        delete[] array;
        array = arr;
    }
    void insertnewarr(long long key, const T& value,HashSigment *arr){
        HashSigment A{key,value,0,0};
        long long index = hash(key);

        for(int i = 0;;i++){
            int insertIndex = (index+A.PSL)%arrlen;
            if(arr[insertIndex].key == A.key and arr[insertIndex].Empty == 0){
                arr[insertIndex].data = A.data;
                break;
            }
            if (arr[insertIndex].Empty == 1){
                arr[insertIndex] = A;
                break;
            }
            if (arr[insertIndex].PSL < A.PSL){
                swap(&A,&arr[insertIndex]);
                index = hash(A.key);
            }
            A.PSL++;
        }
    }
public:
    HashMap():array(nullptr),size(0),coefload(0.5),b(rand()%20)
    {
        PrimeArr.set_newArr(arrlen * 4);
        array = new HashSigment[arrlen];
        a = PrimeArr.FindNearest(rand()%20);
    }

    ~HashMap()
    {
        delete[] array;
    }
    void print(){
        for(int i = 0; i < arrlen;i++){
            std::cout << array[i].key << ' ';
        }
    }
    void insert(long long key, const T& value)
    {
        float coefload = (float)size/(float)arrlen;
        if(coefload > 0.5){
            resize_arr();
        }
        HashSigment A{key,value,0,0};
        long long index = hash(key);

        for(int i = 0;;i++){
            int insertIndex = (index+A.PSL)%arrlen;
            if(array[insertIndex].key == A.key and array[insertIndex].Empty == 0){
                array[insertIndex].data = A.data;
                return;
            }
            if (array[insertIndex].Empty == 1){
                array[insertIndex] = A;
                size++;
                return;
            }
            if (array[insertIndex].PSL < A.PSL){
                swap(&A,&array[insertIndex]);
                index = hash(A.key);
            }
            A.PSL++;
        }

    }

    T* find(long long key)
    {
        long long index = hash(key);
        for(int i = 0;;i++){
            int findIndex =(index+i)%arrlen;
            if(array[findIndex].key == key and array[findIndex].Empty == 0){
                return &array[(index+i)%arrlen].data;
            }
            if (array[findIndex].Empty == 1 or array[findIndex].PSL < i){
                return nullptr;
            }
        }
    }

    void erase(unsigned long long key)
    {
        long long index = hash(key);
        for(int i = 0;;i++){
            int eraseIndex =(index+i)%arrlen;
            if(array[eraseIndex].key == key and array[eraseIndex].Empty == 0){
                size--;
                array[eraseIndex].Empty = 1;
                for(int j = 0;;j++){
                    if(array[(index+i+j+1)%arrlen].Empty == 1 or array[(index+i+j+1)%arrlen].PSL == 0){
                        break;
                    }
                    array[(index+i+j+1)%arrlen].PSL--;
                    swap(&array[(index+i+j)%arrlen],&array[(index+i+j+1)%arrlen]);
                }
                break;
            }
            if (array[eraseIndex].Empty == 1 or array[eraseIndex].PSL < i){
                /*std::cerr<< "Error";*/
                return;
            }
        }
    }

    int sizeH() const {return size;}
};
#endif //COALESCED_HASHING_HASHMAP_H
