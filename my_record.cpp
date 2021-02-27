//aplicar o hash


#include <iostream> //cout
#include <fstream> //fstream
#include <cstring> //strcpy
#include <limits>
#include <math.h>
#include "hashing.cpp"

using namespace std;

//struct do registro (referencia bibliográfica)
struct MyRecord {
  char title[80];
  char author[80];
  char publication[80];
  int year;
  int pageStart;
  int pageEnd;
  char id[20];
};

//nó da lista de registros
struct ListElement{
    MyRecord record;
    ListElement *prox;
};

//insere um elemento na posição final da lista de registros
ListElement* insere_final (ListElement* head, MyRecord thisRecord) {
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

//escreve um novo registro na lista de registros
ListElement*  write(ListElement* regs, int numRegs) {
    MyRecord mr;

    cout << " -> inserindo." << endl;

    char titleInsert[80];
    char authorInsert[80];
    char publicationInsert[80];
    int yearInsert;
    int pageStartInsert;
    int pageEndInsert;
    char idInsert[20];  
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

    cout<<"Código do artigo: ";
    cin>>idInsert;
    strcpy(mr.id, idInsert);

    regs = insere_final(regs, mr);

    return regs;
}

//imprime todos os elementos na lista de registros
void print_all (ListElement* records) {
    ListElement* current = records;
    while(current != NULL){
        //puts("entro");
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
}

//escreve os registros da lista, no arquivo
//escreve num outro arquivo o número de registros na lista que foram escritos para o arquivo
void store (ListElement* head){
    int array_size = 0;
    ListElement* current = head;
    FILE *fp = fopen("myRecord.txt", "wb");

    if(head == NULL){
        return;
    }
    else{
        int num = 0;
        while (current != NULL) {
            puts("**entro**");
            fwrite(&(current->record), 1, sizeof(current->record), fp);
            current = current->prox;
            num++;
        }
        FILE *num_reg = fopen("numberOfRecordsStored.txt", "wb");
        putw(num, num_reg);
    }
}

int main() {
    //procedimento de "load": carrega os registros do arquivo para a memória
    FILE *fp;
    bool isFile = NULL;
    fp = fopen("myRecord.txt", "rb");
    ListElement* head = NULL;

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
        ListElement* current = head;
        //insere os registros do arquivo numa lista de registros
        MyRecord mr;
        for(int i = 0; i < num; i++){
            fread(&mr, sizeof(mr), 1, fp);
            head = insere_final(head, mr);
        }
    }


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