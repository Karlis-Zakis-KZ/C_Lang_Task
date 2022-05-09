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
#include <unistd.h>
#include <stdlib.h>

#define MAX_CARD_TYPES 10

typedef struct {
    char min_number[17];
    char max_number[17];
    char card_name[11];
} card_types;

// Inputs the array, counts the characters and returns an int value
int length_of_char(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        i++;
    }
    return i;
}

// Checks whenever the char is valid character
int is_digit_or_letter(char c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z'));
}


// Inputs the card number array and checks whenever its a valid card number
int validate_card_number(const char *input) {
    int err = 0;
    if (length_of_char(input)!=16){
        err = 1;
        return err;
    }
    for (int n = 0; n < 16; n++) {
        if (!(input[n] >= '0' && input[n] <= '9')) {
            err = 1;
            break;
        }
    }
    return err;
}


// Compares the card number to min and max to see if it fits a card type
int compare_numbers(const char *card_number,const char *min,const char *max,int parse_limit){

    int err = 0;
    char *stop_array;
    char temp_card_double[17];
    char temp_min[17];
    char temp_max[17];

    // Fully optional step, just makes me feel better not having a double with 16 digits.
    for (int x=0; x<parse_limit+1;x++){
        if(x==3){
            temp_card_double[x] = ',';
            temp_min[x] = ',';
            temp_max[x] = ',';
        }else{
            temp_card_double[x] = card_number[x];
            temp_min[x] = min[x];
            temp_max[x] = max[x];
        }
    }

    // Comparing the numbers by simply converting the array to a Double type
    if (!(strtod(temp_card_double,&stop_array) >= strtod(temp_min,&stop_array) && strtod(temp_card_double,&stop_array) <= strtod(temp_max,&stop_array))) {
        err = 1;
    }

    return err;
}


// Checks whenever the withdraw amount is valid and accepts both dot and comma
int validate_withdraw_amount(const char *input) {
    int err = 0;
    int count_euros=0;
    int count_cents=0;
    int count_dot=0;
    int flag = 0;
    for (int n = 0; n < length_of_char(input); n++) {
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

// frees up the stdin so it dosent overflow and inpact the next card number entry
void flush_stdin() {
    int c;
    while ((c = fgetc(stdin)) != '\n' && c != EOF); /* Flush stdin */
}

// asks the user to input the card number
void get_card_number(char *buf) {
    for (;;) {
        printf("Enter card number (16 digits) \n");

        scanf("%s", buf);
        flush_stdin();

        if (validate_card_number(buf) == 0) {
            break;
        }
        printf("Card was entered in incorrectly \n");
        usleep(2000000);
    }
}

// asks the user to input a valid withdraw amount
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
    FILE *file_pointer = fopen("file.txt", "r");

    card_types card_types[MAX_CARD_TYPES] = {0};

    if (file_pointer == NULL) {
        printf("File is not available \n");
        system("pause");
        return 1;
    }

    // fills the card_types structure with values
    while ((ch = fgetc(file_pointer)) != EOF) {
        if (ch == ';') {
            separator_count++;
            counter = 0;
            if (separator_count == 3) {
                separator_count = 0;
                card_type_counter++;
            }
        } else if (is_digit_or_letter(ch)) {
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

    // repeated loop that asks the user to input values and saves them in the file trans.txt
    while (counter!=1) {
        int card_type_index = -1; // -1 not found
        char card_number[17] = {0};
        get_card_number(card_number);

        for (int n = 0; n < card_type_counter; n++) {

            if (compare_numbers(card_number,card_types[n].min_number,card_types[n].max_number,length_of_char(card_types[n].max_number))!=1) {
                card_type_index = n;
                break;
            }
        }
        if(card_type_index != -1){
            for(;;){
                char withdraw_amount[7] = {0};
                get_withdraw_amount(withdraw_amount);

                FILE * fptr;
                fptr = fopen("trans.txt", "a");

                fprintf(fptr,"%s %s %s \n",card_number,card_types[card_type_index].card_name,withdraw_amount);
                fclose(fptr);


                break;
            }
        }

    }
    return 0;
}