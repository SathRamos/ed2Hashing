//aplicar o hash
#include <iostream> //cout
#include <fstream> //fstream
#include <cstring> //strcpy
#include <limits>
#include <math.h>
#include "hashing.cpp"
#define MAX_BUCKET_SIZE 3 //tamanho em que se considera uma entrada da tabela hash como "cheia"
#define ID_SIZE 20 // tamanho do identificador da referencia bibliografica

using namespace std;


//struct do registro (referencia bibliográfica)
struct MyRecord {
  char title[80];
  char author[80];
  char publication[80];
  int year;
  int pageStart;
  int pageEnd;
  string id;
};

//nó da lista de registros
struct ListElement{
    MyRecord record;
    ListElement *prox;
};

//nó da tabela hash
struct HashTable{
    ListElement* listHead;
    HashTable *prox;
};


//insere um elemento na posição final da lista de registros
ListElement* insere_final_lista (ListElement* head, MyRecord thisRecord) {
    ListElement* new_element = (ListElement *) malloc(sizeof(ListElement));
    new_element->record = thisRecord;

    
    if(head == NULL) head = new_element;
    else{
        ListElement* current = head;
        while(current->prox != NULL)
        {
                current = current->prox;
        }
        current->prox = new_element;
        new_element->prox = NULL;
    }

    return head;
}

int tam_lista (ListElement* head){
    ListElement* current = head;
    int tam = 0;
    while(current != NULL){
        tam++;
        current = current->prox;
    }
    return tam;
}

HashTable* insere_tabela_hash(HashTable* hashHead, MyRecord record){
    HashTable* current = hashHead;
    int table_size = 0;
    while(current != NULL){
        table_size++;
        current = current->prox;
    }
    current = hashHead;

    if(hashHead == NULL){//tabela hash vazia
        //cria uma posição na tabela
        HashTable* new_element = (HashTable *) malloc(sizeof(HashTable));
        hashHead = new_element;
        new_element->listHead = NULL;
        new_element->prox = NULL;

        //insere o novo registro na lista daquela posição.
        new_element->listHead = insere_final_lista(new_element->listHead, record);
        new_element->prox = NULL;
    }
    else{//tabela hash já populada
        int key = (stoi(shift_folding(record.id))%table_size);// calcula o hash em cima do id do registro e o tamanho atual da tabela
        bool inserted = false;
        for(int count = 0; !inserted ; count++){
            if(key == count){//chegou na posição da tabela referente ao hash calculado
                if(tam_lista(current->listHead) > MAX_BUCKET_SIZE){//se a lista naquela posição da tabela já atingiu o tamanho máximo
                    
                    // expande a lista
                    HashTable* aux = current;
                    while(aux != NULL) current = current->prox;//alcança o final da tabela hash
                    //cria uma nova posição ao final da tabela.
                    HashTable* new_element = (HashTable *) malloc(sizeof(HashTable));
                    hashHead = new_element;
                    new_element->listHead = NULL;
                    new_element->prox = NULL;

                    //atualiza o tamanho da tabela
                    table_size++;
                    key = (stoi(shift_folding(record.id))%table_size);// calcula o hash em cima do id do registro e o novo tamanho da tabela
                    //reseta o ponteiro e o contador, para percorrer a tabela de novo desde o começo
                    current = hashHead;
                    count = 0;
                }
                else{//se a lista naquela posição ainda suportar mais registros (número abaixo do máximo).
                    current->listHead = insere_final_lista(current->listHead, record);//insere o novo registro naquela lista
                    inserted = true;
                } 
            }
            current = current->prox;
        }
    }

    return hashHead;
}

//escreve um novo registro na lista de registros
HashTable*  write(HashTable* regs, int numRegs) {
    MyRecord mr;

    cout << " -> inserindo." << endl;

    char titleInsert[80];
    char authorInsert[80];
    char publicationInsert[80];
    int yearInsert;
    int pageStartInsert;
    int pageEndInsert;
    string idInsert = "a";  
    fflush(stdin);

    cout<<"Título do artigo: ";
    cin.getline(titleInsert, 100);
    strcpy(mr.title, titleInsert);

    cout<<"Autor(a) do artigo: ";
    cin.getline(authorInsert, 100);
    strcpy(mr.author, authorInsert);

    cout<<"Veículo de publicação: ";
    cin.getline(publicationInsert, 100);
    strcpy(mr.publication, publicationInsert);

    cout<<"Ano de publicação: ";
    cin>>yearInsert;
    mr.year = yearInsert;

    cout<<"Página de início do artigo: ";
    cin>>pageStartInsert;
    mr.pageStart = pageStartInsert;

    cout<<"Página final do artigo: ";
    cin>>pageEndInsert;
    mr.pageEnd = pageEndInsert;

    while(!isNumeric(idInsert)){
        cout<<"Código do artigo (numérico): ";
        cin>>mr.id;
    }
    regs = insere_tabela_hash(regs, mr);

    return regs;
}

//imprime todos os elementos na lista de registros
void print_all (HashTable* records) {
    HashTable* iter = records;

    while (iter != NULL) {
        ListElement* current = iter->listHead;
        while(current != NULL){
            cout << "\nTitulo: " << current->record.title << endl;
            cout << "Autor: " << current->record.author << endl;
            cout << "Publicado em: " <<current->record.publication << endl;
            cout.setf(ios::fixed);
            cout << "Ano: " << current->record.year << endl;
            cout << "Página de início: " << current->record.pageStart << endl;
            cout << "Página final: " << current->record.pageEnd << endl;
            cout << "Identificação: " << current->record.id << endl;
            cout<<"------------------------------------\n";

            current = current->prox;
        }
        iter = iter->prox;
    }
}

//escreve os registros da lista, no arquivo
//escreve num outro arquivo o número de registros na lista que foram escritos para o arquivo
void store (HashTable* head){
    HashTable* iter = head;
    FILE *fp = fopen("myRecord.txt", "wb");

    //caso lista esteja vazia
    if(head == NULL){
        return;
    }
    else{
        int num = 0;
        while (iter != NULL) {
            ListElement* current = iter->listHead;
            while(current != NULL){
                fwrite(&(current->record), 1, sizeof(current->record), fp);
                current = current->prox;
                num++;
            }
            iter = iter->prox;
        }

        FILE *num_reg = fopen("numberOfRecordsStored.txt", "wb");
        putw(num, num_reg);
    }
}

int main() {
    //antes do fluxo principal,
    //procedimento de "load": carrega os registros do arquivo para a memória
    FILE *fp;
    bool isFile = NULL;
    fp = fopen("myRecord.txt", "rb");
    HashTable* head = NULL;
    //carrega o número de registros contidos no arquivo, para a memória
    FILE* num_reg = fopen("numberOfRecordsStored.txt", "rb");
    int num = getw(num_reg);
    if(fp == NULL){
        printf("Arquivo não pode ser aberto\n");
        isFile = false;
    }
    else{
        isFile = true;
        printf("%d", num);
        HashTable* current = head;
        //insere os registros do arquivo numa lista de registros
        MyRecord mr;
        for(int i = 0; i < num; i++){
            fread(&mr, sizeof(mr), 1, fp);
            head = insere_tabela_hash(head, mr);
        }
    }

    //fluxo principal do programa, de selecionar opções
    fflush(stdin);
    while(true){
        int ans = 0;
        cout<<"Inserir registro: 1\nExibir registros: 2\nSalvar e Sair: 3\n";
        cin>>ans;
        cin.clear();  // clears the error flags  
        //this line discards all the input waiting in the stream
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        fflush(stdin);
        if(ans == 1) head = write(head, num);
        else{
            if(ans == 2) print_all(head);
            else{
                if(ans == 3){
                    store(head);
                    return 0;
                }
            }
        }
    }
    return 0;
}