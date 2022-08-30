#include <elf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_INPUT 100
#define NUM_OF_ITEMS 5

struct menuItem {
    char* name;
    void (*f)();
};

int debugMode;
int Currentfd = -1;
char* map_start;
int flength;
struct stat fd_stat;

void toggle_debug_mode();
void examine_elf_file();
void print_section_names();
void print_symbols();
void quit();

int main(int argc,char** argv) {
    struct menuItem menu[NUM_OF_ITEMS] = {{"Toggle Debug Mode", toggle_debug_mode}, {"Examine ELF File", examine_elf_file}, {"Print Section Names", print_section_names}, {"Print Symbols", print_symbols}, {"Quit", quit}};

    
    while(1){
        printf("Choose action:\n");
        for(int i = 0; i < NUM_OF_ITEMS; i++){
            printf("(%d) %s\n", i, menu[i].name);
        }
        
        char input[MAX_INPUT];
        int choice = atoi(fgets(input, MAX_INPUT, stdin));
        
        menu[choice].f();
    }
}


void toggle_debug_mode(){
    if(debugMode){
        printf("Debug flag now off\n\n");
        debugMode = 0;
    }
    else{
        printf("Debug flag now on\n\n");
        debugMode = 1;
    }
}

void examine_elf_file(){
    char filename[MAX_INPUT];
    Elf32_Ehdr* header;
    char* magicNumbers;
    
    printf("Enter file name:\n");
    printf("Input: ");
    // GETTING USER INPUT - FILENAME
    fgets(filename, MAX_INPUT, stdin);
    filename[strlen(filename)-1] = 0;
    // CLOSING PREVIOUS FILES
    if(Currentfd >= 0){
        close(Currentfd);
    }
    // OPENNING FILE
    if((Currentfd = open(filename, O_RDONLY)) < 0){
        fprintf(stderr, "ERROR: failed to open file\n");
        exit(-1);
    }
    // GETTING LENGTH OF BYTES TO MAP - FILE SIZE=
    if( fstat(Currentfd, &fd_stat) != 0 ) {
        fprintf(stderr, "ERROR: fstat failed\n");
        exit(-1);
    }    
    // USING MMAP TO READ THE FILE
    if ( (map_start = mmap(NULL, fd_stat.st_size, PROT_READ, MAP_SHARED, Currentfd, 0)) == MAP_FAILED) {
        fprintf(stderr, "ERROR: mmap failed\n");
        exit(-1);
    }
    // DISPLAYING THE ELF FILE INFORMATION
    header = (Elf32_Ehdr*)map_start;
    printf("\n");

    magicNumbers = (char *)header->e_ident;
    if(*(magicNumbers+1) == 69 && *(magicNumbers+2) == 76 && *(magicNumbers+3) == 70){
        printf("Magic numbers:\t%X  %X  %X\n", *(magicNumbers+1), *(magicNumbers+2), *(magicNumbers+3));
    }
    else{
        if(map_start != MAP_FAILED){
                munmap(map_start, fd_stat.st_size);
        }
        Currentfd = -1;
        fprintf(stderr, "ERROR: Not an ELF file\n");
        exit(-1);
        printf("\n");
    }
    
    if(header->e_ident[5] == 0){
        printf("Data:\t\t\t\tInvalid data encoding\n");
    }
    else if(header->e_ident[5] == 1){
        printf("Data:\t\t\t\t2's complement, little endian\n");
    }
    else{
        printf("Data:\t\t\t\t2's complement, big endian\n");
    }
    
    printf("Entry point address:\t\t0x%X\n", header->e_entry);
    printf("Start of section headers:\t%d (bytes into file)\n", header->e_shoff);
    printf("Number of section headers:\t%d\n", header->e_shnum);
    printf("Size of section headers:\t%d (bytes)\n", header->e_shentsize);
    printf("Start of program headers:\t%d (bytes into file)\n", header->e_phoff);
    printf("Number of program headers:\t%d\n", header->e_phnum);
    printf("Size of program headers:\t%d (bytes)\n", header->e_phentsize);
    printf("\n");
    
    //munmap(map_start, fd_stat.st_size);
}

void print_section_names(){
    Elf32_Ehdr* header;
    Elf32_Shdr* section_headers;
    Elf32_Shdr* section_header_str_table;
    char* string_table;
    if(Currentfd == -1){
        fprintf(stderr, "No mapped ELF file\n");
        return;
    }
    // WE GET THE ELF HEADER
    header = (Elf32_Ehdr*)map_start;
    // USING THE SECTION HEADER OFFSET WE ADD IT TO THE START OF THE MAP AND GET THE SECTION HEADERS
    section_headers = (Elf32_Shdr*)(map_start + header->e_shoff);
    // USING THE STRING TABLE OFFSET WE GET THE STR_TABLE SECTION HEADER
    section_header_str_table = &section_headers[header->e_shstrndx];
    // WE GET THE STRING TABLE BY USING THE STR_TABLE SECTION HEADER OFFSET
    string_table = map_start + section_header_str_table->sh_offset;
    // PRINTING THE SECTION HEADERS
    
    printf("\nThere are %d section headers, starting at offset %08X:\n\n", header->e_shnum, header->e_shoff);
    printf("Section Headers:\n");
    printf(" [index]\tsection_name\t\t\tsection_address\t\tsection_offset\t\tsection_size\tsection_type\n");
    
    for(int i = 0; i < header->e_shnum; i++){
        printf(" [%2d]\t\t%-16s\t\t%08x\t\t%06x\t\t\t%06x\t\t%08x\n", i, string_table + section_headers[i].sh_name, section_headers[i].sh_addr, section_headers[i].sh_offset, section_headers[i].sh_size, section_headers[i].sh_type);
    }
}

void print_symbols(){
    Elf32_Ehdr* header;
    Elf32_Shdr* section_headers;
    Elf32_Shdr* section_header_str_table;
    char* string_table1;
    char* string_table2;
    Elf32_Sym* symbol_table;
    int symbol_num; 
    if(Currentfd == -1){
        fprintf(stderr, "No mapped ELF file\n");
        return;
    }
    header = (Elf32_Ehdr*)map_start;
    section_headers = (Elf32_Shdr*)(map_start + header->e_shoff);
    section_header_str_table = &section_headers[header->e_shstrndx];
    string_table1 = map_start + section_header_str_table->sh_offset;
    
    for(int i = 0; i < header->e_shnum; i++){
        if(section_headers[i].sh_type == SHT_STRTAB && strcmp(".strtab", &string_table1[section_headers[i].sh_name])==0){
            string_table2 = (char*)map_start + section_headers[i].sh_offset;
        }
    }
    for(int i = 0; i < header->e_shnum; i++){
        if(section_headers[i].sh_type == SHT_SYMTAB || section_headers[i].sh_type == SHT_DYNSYM){
            symbol_table = (Elf32_Sym*)(map_start + section_headers[i].sh_offset);
            symbol_num = (section_headers[i].sh_size / section_headers[i].sh_entsize);
            printf("\nSymbol table '%s' contains %d entries:\n\n", (string_table1 + section_headers[i].sh_name), symbol_num);
            printf("[index]\t\tvalue\t\tsection_index\tsection_name\t\t\tsymbol_name\n");
            for(int j = 0; j < symbol_num; j++){
                printf("[%2d]\t\t%08x\t", j, symbol_table[j].st_value);
                if(symbol_table[j].st_shndx == SHN_UNDEF)
                   printf("UND\t\t%-16s\t\t", "");
                else if(symbol_table[j].st_shndx == SHN_COMMON)
                   printf("COM\t\t%-16s\t\t", "");
                else if(symbol_table[j].st_shndx == SHN_ABS)
                   printf("ABS\t\t%-16s\t\t", "");
                else{
                    printf("%d\t\t%-16s\t\t", symbol_table[j].st_shndx, (string_table1 + section_headers[symbol_table[j].st_shndx].sh_name));
                }
                printf("%-16s\n", string_table2 + symbol_table[j].st_name);
            }
        }
    }
}

void quit(){
    if(Currentfd != -1){
        close(Currentfd);
        munmap(map_start, fd_stat.st_size);
    }
    exit(0);
}
