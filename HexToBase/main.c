#include <stdio.h>
#include <stdlib.h>

char* hex_to_base64(char *hex, int size_of_input)
{
    int size64 = (size_of_input * 2) / 3;
    size64 += 1;
    char *base64 = calloc(size64, 1);
    size64 -= 1;
    for (int i = size_of_input-1; i>= 0; i-=3, size64-=2) {
        base64[size64] |= hex[i];
        if (i > 0) {
            base64[size64] |= ((hex[i - 1] << 4) & 0x3F);
            base64[size64 - 1] |= (hex[i - 1] >> 2);
        }
        if (i > 1) {
            base64[size64 - 1] |= ((hex[i - 2] << 2));
        }
    }
    return base64;
}


int CharToDec(char c){
    if(c>='0' && c<='9') return c-'0';
    if(c>='a' && c<='f') return c-'a'+10;
    if(c>='A' && c<='F') return c-'A'+10;
    return 0;
}



int main(int argc, char **argv)
{
    int i = 0;
    const char input_hex[] = "49276D206B696C6C696E6720796F757220627261696E206C696B65206120706F69736F6E6F7573206D757368726F6F6D";
    char formatted_input[sizeof(input_hex)];
    for(int x=0;x<sizeof(input_hex);x++){
        formatted_input[x] = CharToDec(input_hex[x]);
        printf("%d ", formatted_input[x]);
    }

    char *output;
    int output_size = ((sizeof(formatted_input)* 2) / 3) + 1;
    char *text = calloc( + 1, 1);
    char *formatted;
    output = hex_to_base64(formatted_input, sizeof(formatted_input));
    for (i = output_size-1; i >=0; i--) {
        if (output[i] < 26) {
            text[i] = output[i] + 65;
        }
        else if (output[i] < 52) {
            text[i] = output[i] + 97 - 26;
        }
        else if (output[i] < 62) {
            text[i] = output[i] + 48 - 52;
        }
        else if (output[i] == 62) {
            text[i] = '+';
        }
        else if (output[i] == 63) {
            text[i] = '/';
        }
    }
    i = 0;
    formatted = text;
    while (text[i++] == 'A') {
        formatted++;
    }
    printf("%s\n", formatted);
    free(text);
    return 0;
}