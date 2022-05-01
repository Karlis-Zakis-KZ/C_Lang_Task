/*
Uzdevuma risināšanā lūdzu neizmantot "string" un "long long" tipus; goto; malloc.



Uzdevums ir vienkāršs bez īpašiem slēptiem “zemūdens akmeņiem” un domāts C pamat zināšanu un koda veidošanas struktūras iemaņu pārbaudei.



Uzdevums.



Pievienotajā .txt failā atrodas karšu numuru tabula, kurā atrodas sekojoši lauki: 1. Range Start; 2. Range End; 3 Name.

[Range Start] un [Range End] lauki var būt no 1 līdz 16 simbolu (ciparu) gari.

Programmas uzdevums būtu sekojošs:

1. Pieprasīt ievadīt 16 ciparus garu kartes numuru.

    Kartes numura ievadei, pārbaudei un apstrādei jāizmanto char[n] tips (simbolu masīvs), lai pārbaudītu zināšanas par masīviem un norādēm (pointer).

    Ja kartes numurs ievadīts nekorekti, uz ekrāna izvadīt atbilstošu paziņojumu un atgriezties pie kartes numura ievades.

2. Pārbaudīt, vai ieraksts, kas atbilst ievadītajam kartes numuram, atrodams teksta failā (pārbaudīt pēc ievadītā numura pirmajiem n cipariem, kur n=ciparu skaits attiecīgajā kolonā failā file.txt). Piemēram, faila pirmā rinda satur sekojošu informāciju:"400000000000;499999999999;VISA;" Tas nozīmē, ka ievadītais kartes numurs atbilst šai rindai, ja skaitlis, kas atbilst tā pirmajiem 12 cipariem >=400000000000 un <=499999999999. Ja ievadītais numurs neatbilst nevienai rindai failā, 2 sekundes uz ekrāna parādīt brīvi izvēlētu kļūdas paziņojumu un atgriezties uz punktu 1.

3. Pieprasīt ievadīt summu formātā "nnnn.mm", kur nnnn- 1 līdz 4 ciparu gara summa eiro, bet mm - 2 ciparu summa centos.

    Ja summa ievadīta nekorekti, uz ekrāna izvadīt atbilstošu paziņojumu un atgriezties pie summa ievades.

4. Saglabāt failā "trans.txt" ievadīto kartes numuru, numuram atbilstošo "Name" lauku no file.txt un ievadīto summu.

5. Atgriezties uz punktu 1.



Ja ir kādas neskaidrības par uzdevuma nostādni, dod ziņu un centīšos izskaidrot nesaprotamās lietas.



Laika limita uzdevuma izpildei nav, galvenais ir kvalitāte!



Lai veicās!
*/

#include <stdio.h>

#define MAX_CARD_TYPES 10

typedef struct {
    char min_number[17];
    char max_number[17];
    char card_name[10];
} card_types;

int strlen(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

long str_to_long(const char *input, int parse_limit) {
    long int value = 0;

    for (int n = 0; n < parse_limit && n < strlen(input); n++) {
        value *= 10;
        value += input[n] - '0';
    }

    return value;

}

int validate_card_number(const char *input) {
    int err = 0;
    for (int n = 0; n < 16; n++) {
        if (!(input[n] >= '0' && input[n] <= '9')) {
            err = 1;
            break;
        }
    }
    return err;
}
int validate_withdraw_amount(const char *input) {
    int err = 0;
    int count_euros=0;
    int count_cents=0;
    int count_dot=0;
    int flag = 0;
    for (int n = 0; n < strlen(input); n++) {
        if (!(input[n] >= '0' && input[n] <= '9' || input[n] == ',' || input[n] == '.')) {
            err = 1;
            break;
        }else{
            if((input[n] == ',' || input[n] == '.') && count_dot<=1 ){
                count_dot++;
                flag++;
            }else if(flag == 0 && count_euros<4){
                count_euros++;
            }else if(flag == 1 && count_cents<2) {
                count_cents++;
            }else{
                err = 1;
                break;
            }
        }
    }
    return err;
}

void flush_stdin() {
    int c;
    while ((c = fgetc(stdin)) != '\n' && c != EOF); /* Flush stdin */
}

void get_card_number(char *buf) {
    for (;;) {
        printf("Enter card number (16 digits) \n");

        scanf("%16s", buf);
        flush_stdin();

        if (validate_card_number(buf) == 0) {
            break;
        }
        printf("Card was entered in incorrectly \n");
    }
}

void get_withdraw_amount(char *withdraw) {
    for (;;) {
        printf("Enter the amount you wish to withdraw [format 'nnnn.mm'] \n");

        scanf("%7s", withdraw);
        flush_stdin();

        if (validate_withdraw_amount(withdraw) == 0) {
            break;
        }
        printf("Withdraw was entered in incorrectly \n");
    }
}


int main() {
    int separator_count = 0;
    int counter = 0;
    int card_type_counter = 0;

    char ch;
    FILE *file_pointer = fopen("../file.txt", "r");

    card_types card_types[MAX_CARD_TYPES] = {0};

    if (file_pointer == NULL) {
        printf("File is not available \n");
        return 1;
    }

    while ((ch = fgetc(file_pointer)) != EOF) {
        if (ch == ';') {
            separator_count++;
            counter = 0;
            if (separator_count == 3) {
                separator_count = 0;
                card_type_counter++;
            }
        } else {
            switch (separator_count) {
                case 0:
                    card_types[card_type_counter].min_number[counter] = ch;
                    break;
                case 1:
                    card_types[card_type_counter].max_number[counter] = ch;
                    break;
                case 2:
                    card_types[card_type_counter].card_name[counter] = ch;
                    break;
                default:
                    break;
            }
            counter += 1;
        }
    }

    while (1) {
        int card_type_index = -1; // -1 not found
        char card_number[17] = {0};
        get_card_number(card_number);
        for (int n = 0; n < card_type_counter; n++) {
            long card_number_int = str_to_long(card_number, strlen(card_types[n].max_number));
            long card_min_number = str_to_long(card_types[n].min_number, strlen(card_types[n].min_number));
            long card_max_number = str_to_long(card_types[n].max_number, strlen(card_types[n].max_number));

            if (card_number_int >= card_min_number && card_number_int <= card_max_number) {
                card_type_index = n;
                break;
            }
        }
        if(card_type_index != -1){
            for(;;){
                char withdraw_amount[7] = {0};
                get_withdraw_amount(withdraw_amount);

                FILE * fptr;
                fptr = fopen("../trans.txt", "w");

                fprintf(fptr,"Card Number %s Type %s Withdraw %s",card_number,card_types[card_type_index].card_name,withdraw_amount);
                fclose(fptr);


                break;
            }
        }
    }
    return 0;
}
