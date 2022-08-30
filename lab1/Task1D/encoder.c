#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int getHex(char c);
// TO READ: Enums {REG, ENC, DEC}

int main(int argc, char **argv) {
  	FILE* in = stdin;
	FILE* out = stdout;
    int counter = 0;
    int debugg = 0;
    int encKEY = 0;
    int addKEY = 0;
    int encVAL;
	int i = 1;
    int ch;
	int enc;
    int first = -1;
    int skip = 0;
    
	for(i = 1; i < argc; i++){
		if(strcmp(argv[i],"-D") == 0){
            debugg = 1;
        }
        else if(strlen(argv[i]) > 2 && argv[i][1] == 'e'){
            if(argv[i][0] == '+' || argv[i][0] == '-'){
                if(argv[i][0] == '+'){
                   addKEY = 1;
                }
                encKEY = 1;
                encVAL = getHex(argv[i][2]);
                if(encVAL == -1){
                    printf("invalid encryption key\n");
                    return 0;
                }
            }
        }
        else if(strlen(argv[i]) > 2 && argv[i][0] == '-' && argv[i][1] == 'i'){
            FILE* INPUT = fopen(argv[i]+2, "r");
            if(INPUT == NULL){
                printf("input file does not exist\n");
                return 0;
            }
            in = INPUT;
        }
        else{
            if(debugg){
                fprintf(stderr, "%s\n", argv[i]);
            }
        }
	}

    if(encKEY && !addKEY){
        skip = encVAL;
    }
	while((ch = fgetc(in)) != EOF){
        if(first == -1 && ch != '\n')
            first = ch;
        if(ch == '\n'){
            if(first != -1 && encKEY && addKEY){
                int i = 0;
                for(i = 0; i < encVAL; i++){
                    fputc(first, out);
                }
                first = -1;
            }
            if(encKEY && !addKEY){
                fprintf(out, skip > 0 ? "NONE" : "");
                skip = encVAL;
                printf("\n");
                continue;
            }
        }
        if(ch == '\n' && debugg){
            fprintf(stderr, "the number of letters: %d\n",counter);
            counter = 0;
            printf("\n");
            continue;
        }
		if(ch >= 'A' && ch <= 'Z'){
			enc = '.';
            counter++;
		}
		else{
			enc = ch;
		}
		if(encKEY){
            if(!skip)
                fputc(ch, out);
            else
                skip--;
        }
        else
            fputc(enc, out);
        if(debugg){
            fprintf(stderr, "%d %d\n",ch, enc);
        }
	}
	fclose(out);
	fclose(in);
  	return 0;
}

int getHex(char c){
    if(c >= '0' && c <= '9')
        c = c - '0';
    else if(c >= 'A' && c <= 'F')
        c = c - 'A' + 10;
    else
        return -1;
    return c;
}
