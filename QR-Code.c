#include "a2.h"

// Helper Function Declarations
char *get_bit_representation(unsigned char x);
char get_decimal_representation(char* bin_rep);
int my_pow(int base, int exponent);
int update_ind(int occupied_spaces[], int ind);
void visualize(char* code);
int get_min(int a, int b, int c);
char binary_to_hex(char* bin_rep);
char* hex_to_binary(char* hex_rep);


int bitwise_xor(int value){
    return value^KEY;
}

char *get_bit_representation(unsigned char x){
    char* bit_rep = malloc(sizeof(char)*8);
	bit_rep[7] = '\0';
    int ind = 0;
    unsigned char tmp = x;
    unsigned char mask = (~0b0) ^ ((unsigned char)(~0b0) >> 1);
    unsigned char width = (~0b0);
	x <<= 1;
	tmp <<= 1;
	width <<=1;

    while (width != 0){
        if ((mask & tmp) == 0){
            bit_rep[ind] = '0';
        } else {
            bit_rep[ind] = '1';
        }
        tmp <<= 1;
        width <<= 1;
        ind++;
    }
    return bit_rep;
}

char *xor_encrypt(char c){
    unsigned char x = bitwise_xor(c);

    return get_bit_representation(x);
}

int my_pow(int base, int exponent){
	int result = 1;
	for (int i=0; i < exponent; i++){
		result = result*base;
	} 
	return result;
}

char get_decimal_representation(char* bin_rep){
	char decimal_rep = 0;
	int exp = 6;
	for (int i = 0; i < 7; i++){
		if (bin_rep[i] == '1'){
			decimal_rep += my_pow(2, exp);
		}
		exp--;
	}
	return decimal_rep;
}

char xor_decrypt(char *s){
    char c = get_decimal_representation(s);
    return c^KEY;
}

int update_ind(int occupied_spaces[], int ind){
    for (int i = 0; i < 76; i++){
        if (occupied_spaces[i] == ind){
            ind++;
        }
    }
    return ind;
}

void visualize(char* code){
    for (int i = 0; i<256;i++)
    {
        if(code[i] == '1')
        {
            printf(" ■ ");
            
        }
        else
        {
            printf("   ");
        }
        
        if((i+1)%16 == 0 )
        {
            printf("\n");
        }
    }
}

char *gen_code(char *msg){
    char* SC_code = malloc(sizeof(char)*257);
    SC_code[256] = '\0';
    int occupied_spaces[] = {0,1,2,3,4,11,12,13,14,15,16,17,18,19,20,27,28,29,30,31,32,33,34,35,36,43,44,45,46,47,48,49,50,51,52,59,60,61,62,63,64,65,66,67,68,75,76,77,78,79,176,177,178,179,180,192,193,194,195,196,208,209,210,211,212,224,225,226,227,228,240,241,242,243,244,255};
    int ind = 5;

    // initializing entire array of SC code to '0'
    for (int i = 0; i < 256; i++){
        SC_code[i] = '0';
    }
    
    // filling in known sections of SC code
    SC_code[0] = '1'; SC_code[1] = '1'; SC_code[2] = '1'; SC_code[3] = '1'; SC_code[4] = '1';
    SC_code[11] = '1'; SC_code[12] = '1'; SC_code[13] = '1'; SC_code[14] = '1'; SC_code[15] = '1';
    SC_code[16] = '1'; SC_code[17] = '0'; SC_code[18] = '0'; SC_code[19] = '0'; SC_code[20] = '1';
    SC_code[27] = '1'; SC_code[28] = '0'; SC_code[29] = '0'; SC_code[30] = '0'; SC_code[31] = '1';
    SC_code[32] = '1'; SC_code[33] = '0'; SC_code[34] = '1'; SC_code[35] = '0'; SC_code[36] = '1';
    SC_code[43] = '1'; SC_code[44] = '0'; SC_code[45] = '1'; SC_code[46] = '0'; SC_code[47] = '1';
    SC_code[48] = '1'; SC_code[49] = '0'; SC_code[50] = '0'; SC_code[51] = '0'; SC_code[52] = '1';
    SC_code[59] = '1'; SC_code[60] = '0'; SC_code[61] = '0'; SC_code[62] = '0'; SC_code[63] = '1';
    SC_code[64] = '1'; SC_code[65] = '1'; SC_code[66] = '1'; SC_code[67] = '1'; SC_code[68] = '1';
    SC_code[75] = '1'; SC_code[76] = '1'; SC_code[77] = '1'; SC_code[78] = '1'; SC_code[79] = '1';

    SC_code[176] = '1'; SC_code[177] = '1'; SC_code[178] = '1'; SC_code[179] = '1'; SC_code[180] = '1';
    SC_code[192] = '1'; SC_code[193] = '0'; SC_code[194] = '0'; SC_code[195] = '0'; SC_code[196] = '1';
    SC_code[208] = '1'; SC_code[209] = '0'; SC_code[210] = '1'; SC_code[211] = '0'; SC_code[212] = '1';
    SC_code[224] = '1'; SC_code[225] = '0'; SC_code[226] = '0'; SC_code[227] = '0'; SC_code[228] = '1';
    SC_code[240] = '1'; SC_code[241] = '1'; SC_code[242] = '1'; SC_code[243] = '1'; SC_code[244] = '1';

    SC_code[255] = '1';

    // adding message into SC code
    for (int i = 0; i <= strlen(msg); i++){
        char* enc_char = xor_encrypt(msg[i]);
        for (int j = 0; j < 7; j++){
            SC_code[ind] = enc_char[j];    
            ind++;
            ind = update_ind(occupied_spaces, ind);
        }
        free(enc_char);
    }
    return SC_code;
}

char *read_code(char *code){
    int occupied_spaces[] = {0,1,2,3,4,11,12,13,14,15,16,17,18,19,20,27,28,29,30,31,32,33,34,35,36,43,44,45,46,47,48,49,50,51,52,59,60,61,62,63,64,65,66,67,68,75,76,77,78,79,176,177,178,179,180,192,193,194,195,196,208,209,210,211,212,224,225,226,227,228,240,241,242,243,244,255};
    char* string = NULL;
    int code_ind = 5;
    char* null_char = get_bit_representation(KEY);

    for (int i = 0; i < 25; i++){
        char bin[8] = {};
        bin[7] = '\0';
        for (int j = 0; j < 7; j++){
            bin[j] = code[code_ind];
            code_ind++;
            code_ind = update_ind(occupied_spaces, code_ind);
        }
        string = (char*)realloc(string, i + 1);
        string[i] = xor_decrypt(bin);
        if (strcmp(bin, null_char) == 0){
            break;
        }
    }

    free(null_char);
    return string;
}

char binary_to_hex(char* bin_rep){
    if(strcmp(bin_rep, "0000") == 0){
        return '0';
    }else if(strcmp(bin_rep,"0001") == 0){
        return '1';
    }else if(strcmp(bin_rep,"0010") == 0){
        return '2';
    }else if(strcmp(bin_rep,"0011") == 0){
        return '3';
    }else if(strcmp(bin_rep,"0100") == 0){
        return '4';
    }else if(strcmp(bin_rep,"0101") == 0){
        return '5';
    }else if(strcmp(bin_rep,"0110") == 0){
        return '6';
    }else if(strcmp(bin_rep,"0111") == 0){
        return '7';
    }else if(strcmp(bin_rep,"1000") == 0){
        return '8';
    }else if(strcmp(bin_rep,"1001") == 0){
        return '9';
    }else if(strcmp(bin_rep,"1010") == 0){
        return 'A';
    }else if(strcmp(bin_rep,"1011") == 0){
        return 'B';
    }else if(strcmp(bin_rep,"1100") == 0){
        return 'C';
    }else if(strcmp(bin_rep,"1101") == 0){
        return 'D';
    }else if(strcmp(bin_rep,"1110") == 0){
        return 'E';
    }else if(strcmp(bin_rep,"1111") == 0){
        return 'F';
    } 
}

char *compress(char *code){
    char* hex_string = malloc(sizeof(char)*65);
    hex_string[64] = '\0';
    char bin_segment[5];
    bin_segment[4] = '\0';

    int hex_string_ind = 0;
    for (int i = 0; i < 256; i += 4){
        bin_segment[0] = code[i];
        bin_segment[1] = code[i + 1];
        bin_segment[2] = code[i + 2];
        bin_segment[3] = code[i + 3];
        hex_string[hex_string_ind] = binary_to_hex(bin_segment);
        hex_string_ind++;
    }

    return hex_string;
}

char *decompress(char *code){
    char* bin_string = malloc(sizeof(char)*257);
    bin_string[256] = '\0';

    int ind = 0;
    for (int i = 0; i < 64; i++){
        if (code[i] == '0'){
            bin_string[ind] = '0';
            bin_string[ind + 1] = '0';
            bin_string[ind + 2] = '0';
            bin_string[ind + 3] = '0';
        } else if (code[i] == '1'){
            bin_string[ind] = '0';
            bin_string[ind + 1] = '0';
            bin_string[ind + 2] = '0';
            bin_string[ind + 3] = '1';
        } else if (code[i] == '2'){
            bin_string[ind] = '0';
            bin_string[ind + 1] = '0';
            bin_string[ind + 2] = '1';
            bin_string[ind + 3] = '0';
        } else if (code[i] == '3'){
            bin_string[ind] = '0';
            bin_string[ind + 1] = '0';
            bin_string[ind + 2] = '1';
            bin_string[ind + 3] = '1';
        } else if (code[i] == '4'){
            bin_string[ind] = '0';
            bin_string[ind + 1] = '1';
            bin_string[ind + 2] = '0';
            bin_string[ind + 3] = '0';
        } else if (code[i] == '5'){
            bin_string[ind] = '0';
            bin_string[ind + 1] = '1';
            bin_string[ind + 2] = '0';
            bin_string[ind + 3] = '1';
        } else if (code[i] == '6'){
            bin_string[ind] = '0';
            bin_string[ind + 1] = '1';
            bin_string[ind + 2] = '1';
            bin_string[ind + 3] = '0';
        } else if (code[i] == '7'){
            bin_string[ind] = '0';
            bin_string[ind + 1] = '1';
            bin_string[ind + 2] = '1';
            bin_string[ind + 3] = '1';
        } else if (code[i] == '8'){
            bin_string[ind] = '1';
            bin_string[ind + 1] = '0';
            bin_string[ind + 2] = '0';
            bin_string[ind + 3] = '0';
        } else if (code[i] == '9'){
            bin_string[ind] = '1';
            bin_string[ind + 1] = '0';
            bin_string[ind + 2] = '0';
            bin_string[ind + 3] = '1';
        } else if (code[i] == 'A'){
            bin_string[ind] = '1';
            bin_string[ind + 1] = '0';
            bin_string[ind + 2] = '1';
            bin_string[ind + 3] = '0';
        } else if (code[i] == 'B'){
            bin_string[ind] = '1';
            bin_string[ind + 1] = '0';
            bin_string[ind + 2] = '1';
            bin_string[ind + 3] = '1';
        } else if (code[i] == 'C'){
            bin_string[ind] = '1';
            bin_string[ind + 1] = '1';
            bin_string[ind + 2] = '0';
            bin_string[ind + 3] = '0';
        } else if (code[i] == 'D'){
            bin_string[ind] = '1';
            bin_string[ind + 1] = '1';
            bin_string[ind + 2] = '0';
            bin_string[ind + 3] = '1';
        } else if (code[i] == 'E'){
            bin_string[ind] = '1';
            bin_string[ind + 1] = '1';
            bin_string[ind + 2] = '1';
            bin_string[ind + 3] = '0';
        } else if (code[i] == 'F'){
            bin_string[ind] = '1';
            bin_string[ind + 1] = '1';
            bin_string[ind + 2] = '1';
            bin_string[ind + 3] = '1';
        }
        ind +=4;
    }
    return bin_string;
}

int get_min(int a, int b, int c){
    if(a <= b && a <= c){
		return a;
	}
	if(b <= a && b <= c){
		return b;
	}
	if(c <= a && c <= b){
		return c;
	}
}

int calc_ld(char *sandy, char *cima){
    int arr[strlen(sandy) + 1][strlen(cima) + 1];
    // initializing first row
    for (int i = 0; i < (strlen(cima) + 1); i++){
        arr[0][i] = i;
    }
    // initializing first column
    for (int i = 0; i < (strlen(sandy) + 1); i++){
        arr[i][0] = i;
    }

    // filling in remainder of array
    int substitution;
    int deletion;
    int insertion;
    for (int i = 1; i < (strlen(sandy) + 1); i++){ // iterates through rows
        for (int j = 1; j < (strlen(cima) + 1); j++){ // iterates through columns
            if (sandy[i - 1] == cima[j - 1]){
                arr[i][j] = arr[i - 1][j - 1];
            }
            else{
                substitution = arr[i - 1][j - 1];
                deletion = arr[i - 1][j];
                insertion = arr[i][j - 1];
                arr[i][j] = 1 + get_min(substitution, deletion, insertion);
            }
        }
    }

    return arr[strlen(sandy)][strlen(cima)];
}