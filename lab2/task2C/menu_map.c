#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
  /* TODO: Complete during task 2.a */
  for(int i = 0; i < array_length; i++){
      mapped_array[i] = (*f)(*(array + i));
  }
  return mapped_array;
}
 
int main(int argc, char **argv){
  /* TODO: Test your code */
  int base_len = 5;
  char arr1[base_len];
  char* arr2 = map(arr1, base_len, my_get);
  char* arr3 = map(arr2, base_len, encrypt);
  char* arr4 = map(arr3, base_len, dprt);
  char* arr5 = map(arr4, base_len, decrypt);
  char* arr6 = map(arr5, base_len, cprt);
  free(arr2);
  free(arr3);
  free(arr4);
  free(arr5);
  free(arr6);
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
