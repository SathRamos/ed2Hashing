#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#define FOLD_SIZE 3

class Association{
    string key;
    int type;
    void *value;
    Association *prox;
};

class Dictionary{
    Association data[17];
};

Dictionary dict;

bool isNumeric(string str) {
   for (int i = 0; str[i] != '\0'; i++)
      if (isdigit(str[i]) == false)
         return false; //retorna falso quando um valor não numérico é encontrado
      return true;
}

string shift_folding(string key){
    int size =  key.size();
    string subB = "0";

    if(size>FOLD_SIZE){
        for(int i = 0; i < size; i+=(FOLD_SIZE)){
            string subA;
            if(i+(FOLD_SIZE) > (size-1)){
                cout<<"\nsubA = ("<<i<<", "<<size-1<<")";
                subA = key.substr(i, size-1);//substring A recebe a última porção da chave
            }
            else{
                cout<<"\nsubA = ("<<i<<", "<<i+FOLD_SIZE-1<<")";
                subA = key.substr(i, (FOLD_SIZE));//substring A recebe a próxima porção de "FOLD-SIZE" caracteres da chave
            } 
            cout<<"\nSubstring: "<<subA<<"\n";
            subB = to_string(stoi(subA)+stoi(subB));//soma cumulativa do valor das porções
        }
        return subB;
    }
    else return key;
}

int hash (string key){
    int hashed_key = stoi(key);
    return hashed_key%11;
}

//int main(){
//    char k[20] = "a";
//    while(!isNumeric(k)){
//        cout<<"\nInsira uma chave numérica\n";
//        cin>>k;
//    }
//    int foldedKey = stoi(shift_folding(k));
//    cout<<"\nShift Folding (fold = 3) da chave: "<<shift_folding(k);
//
//    cout<<"\nHashing (mod 11): "<< foldedKey%11;
//    return 0;
//}