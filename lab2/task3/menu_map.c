#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct fun_desc {
    char* name;
    char (*fun)(char);
};

char encrypt(char c);
char decrypt(char c);
char dprt(char c);
char cprt(char c);
char my_get(char c);
char quit(char c);
 
char censor(char c) {
  if(c == '!')
    return '*';
  else
    return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for(int i = 0; i < array_length; i++){
      mapped_array[i] = (*f)(*(array + i));
  }
  return mapped_array;
}
 
int main(int argc, char **argv){
    
  int base_len = 5;
  char array[base_len];
  char* carray = array;
  
  struct fun_desc menu[] =  {{"Censor", censor},{"Encrypt", encrypt},{"Decrypt", decrypt},{"Print dec", dprt},{"Print string", cprt},{"Get string", my_get},{"Quit", quit},{NULL, NULL}};
  
  int lower_bound = 0;
  int upper_bound = (sizeof(menu)/sizeof(struct fun_desc)) - 2;
  
  while(1){
      
    printf("Please choose a function:\n");
    for(int i = 0; i < 7; i++){
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
    
    carray = map(carray, 5, menu[op].fun);
    printf("DONE.\n\n");
    }
  free(carray);
  return 0;
}


char encrypt(char c){
    if(c >= 0x41 && c <= 0x7a)
        c = c + 2;
    return c;
}
char decrypt(char c){
    if(c >= 0x41 && c <= 0x7a)
        c = c - 2;
    return c;
}
char dprt(char c){
    printf("%d", c);
    printf("\n");
    return c;

}
char cprt(char c){
    if(c >= 0x41 && c <= 0x7a){
        printf("%c", c);
        printf("\n");
    }
    else{
        printf("*");
        printf("\n");
    }
    return c;
}
char my_get(char c){
    char ch = fgetc(stdin);
    return ch;
}

char quit(char c){
    if(c == 'q')
        exit(0);
    return c;
}
