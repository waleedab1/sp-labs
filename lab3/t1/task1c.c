#include <stdio.h>
#include <stdlib.h>

typedef struct virus {
    unsigned short SigSize;
    unsigned char* sig;
    char virusName[16];
} virus;

typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

struct fun_desc {
    char* name;
    void (*fun)();
};

void PrintHex(unsigned char* buffer, int length, FILE* output);
void readVirus(virus* vir, FILE* input);
void printVirus(virus* vir, FILE* output);
void list_print(link *virus_list, FILE* output);
link* list_append(link* virus_list, link* to_add);
void list_free(link *virus_list);
void load_signatures();
void print_signatures();
void quit();
void map(void(*f)());
void Detect_viruses();
void detect_virus(char *buffer, unsigned int size, link *virus_list);



link* virus_list = NULL;

int main(int argc, char **argv)
{
    struct fun_desc menu[] =  {{NULL, NULL},{"Load signatures", load_signatures},{"Print signatures", print_signatures},{"Detect viruses", Detect_viruses},{"Quit", quit},{NULL, NULL}};
  
    int lower_bound = 1;
    int upper_bound = (sizeof(menu)/sizeof(struct fun_desc)) - 1;
    
    while(1){
        for(int i = 1; i < upper_bound; i++){
            printf("(%d) %s\n", i, menu[i].name);
        }
    
        printf("Option: ");
        char option[10];
        int op = atoi(fgets(option, 10, stdin));
        
        if(op < lower_bound || op > upper_bound){
            printf("Not within bounds\n");
            return 0;
        }
        else
            printf("Within bounds\n");
            
        map(menu[op].fun);
    }
    
    return 0;
}

void PrintHex(unsigned char* buffer, int length, FILE* output){
    int i;
    for(i = 0; i < length; i++){
        fprintf(output,"%02X ", (unsigned int)(buffer[i] & 0xFF));
    }
    fprintf(output, "\n\n");
}

void readVirus(virus* vir, FILE* input){
    
    // READING THE LENGTH
    fread(&(vir->SigSize), sizeof(char), 2, input);
    
    // READING SIGNATURE
    vir->sig = malloc(vir->SigSize);
    fread(vir->sig, sizeof(char), vir->SigSize, input);
    
    // READING VIRUS NAME
    fread(&(vir->virusName), sizeof(char), 16, input);
}

void printVirus(virus* vir, FILE* output){
    fprintf(output, "Virus name: %s\n", vir->virusName);
    fprintf(output, "Virus size: %d\n", vir->SigSize);
    fprintf(output, "signature:\n");
    PrintHex(vir->sig, vir->SigSize, output);
}

void list_print(link *virus_list, FILE* output){
    link* current = virus_list;
    while(current != NULL){
        printVirus(current->vir, output);
        current = current->nextVirus;
    }
}
 
link* list_append(link* virus_list, link* to_add){
    if(virus_list == NULL)
        return to_add;
    to_add->nextVirus = virus_list;
    return to_add;
}
 
void list_free(link *virus_list){
    if(virus_list == NULL)
        return;
    free(virus_list->vir->sig);
    free(virus_list->vir);
    list_free(virus_list->nextVirus);
}

void load_signatures(){
    int filesize = 0;
    char filename[20];
    char temp[10];
    scanf("%s", filename);
    fgets(temp, 10, stdin);
    FILE* sigs = fopen(filename, "r");
    
    fseek(sigs, 0, SEEK_END);
    filesize = ftell(sigs);
    fseek(sigs, 0, SEEK_SET);
    
    while(ftell(sigs) < filesize){
        virus* vir = malloc(sizeof(virus));
        link* add = malloc(sizeof(link));
        readVirus(vir, sigs);
        add->vir = vir;
        virus_list = list_append(virus_list, add);
    }
    fclose(sigs);
}

void print_signatures(){
    list_print(virus_list, stdout);
}

void map(void(*f)()){
    f();
}

void quit(){
    exit(0);
}

void Detect_viruses(){
    char filename[20];
    char temp[10];
    scanf("%s", filename);
    fgets(temp, 10, stdin);
    FILE* file = fopen(filename, "r");
    char buffer[10000];
    int size;
    
    // GETTING LENGTH OF FILE
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    fread(buffer, sizeof(char), size, file);
    detect_virus(buffer, size, virus_list);
    fclose(file);
}

void detect_virus(char *buffer, unsigned int size, link *virus_list)
{
  while (virus_list != NULL)
  {
    for (int i = 0; i < size; i++)
    {
      if (memcmp(buffer + i, virus_list->vir->sig, virus_list->vir->SigSize) == 0)
      {
        printf("Virus detected ~\nStarting byte: %d\nVirus name: %s\nVirus signature size: %d\n\n", i, virus_list->vir->virusName, virus_list->vir->SigSize);
      }
    }
    virus_list = virus_list->nextVirus;
  }
  printf("\n");
}
