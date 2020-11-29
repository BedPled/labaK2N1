

/*
Задача:
1. тип числа - вещественное 16 разрядное число
2. число делится по 3 цифры

функции, которые необходимо разработать:
1)readTLong (f: text; *A: TLong): boolean - считываем ровнор одно число
2)writeTlong (f: text; A: Tlong )
3)sum (A: TLong, B: TLong):TLong
4)sub (A: TLong, B: TLong):TLong
5)Less (A: TLong, B: TLong) boolean
6)EQ (A: TLong, B: TLong) boolean
*/

/*    что делать

*/
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
const char num_hex[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const int num_I = 20; // кол-во полей в массиве, который хранит целую часть
const int num_R = 20; // кол-во полей в массиве, который хранит вещественную часть
const int length_buff = num_I + num_R + 3; // 3 тк '-' '.' '\0'

struct TLong {
    char Integer[num_I][3]; // хранение целой части
    char Real[num_R][3]; // хранение дробной части
    int countReal = 0; // кол-во занятых ячеек дробной части
    int countInt = 0; // кол-во занятых ячеек целой части
    bool sign = true; // 1: +  |   0: -
};

bool LessTLong (TLong A, TLong B);

int pos(char a){ // поиск цифры в массиве шестнадцетиричных цифр 0 - F (/0 == 0)
    int i = 0;
    while (num_hex[i] != a && i < 16 && (int)a != 0){
        i++;
    }
    if (i < 16) return i;
    else return -1;
}

TLong readTLong(ifstream& inFile){
    TLong num;
    int dot_num = 0;
    int dot_count = 0;
    //bool checkdot = false;
    bool correct = true;
    int null_num = 0; // номер '/0' в буфере

    if (inFile.is_open()) {
        // буфер промежуточного хранения ['-'] + int + '.' + real + ' '

        char buff[length_buff];
        //for (int i = 0; i < length_buff; i++) buff[i] = '\0'; //!!!!! заполение буфера (убрать и искать \0)!!!!!!
        inFile >> buff;


        // проверки кореектности
        if (buff[0] == '-') { // проверки кореектности для отрицательного

            for(int i = 0; i < length_buff; i++){ // проверка длины для отрицательного
                if ((int)buff[i] == 0){
                    null_num = i;
                    break;
                }
            }
            if (null_num == 0){
                cout << "Переполнение буффера числа" << endl;
                correct = false;
            }

            // !!! проверка длины int и real !!!

            if (correct) {  // проверка символов и поиск точки для отрицательного
                for (int i = 1; i < null_num; i++){                                             // i = 1 тк buff[0] == '-'
                    if (buff[i] == '.') {
                        dot_count++;
                        dot_num = i;
                    }
                    if ((buff[i] < '0' || buff[i] > '9') &&
                        (buff[i] < 'A' || buff[i] > 'F') && (buff[i] != '.') && ((int)buff[i] != 0)){
                        cout << "Некорректный символ: " << buff[i] << endl;
                        correct = false;
                    }
                }
            }

        } else { //проверки кореектности для положительного

            for(int i = 0; i < length_buff - 1; i++){ // проверка длины для положительного
                if ((int)buff[i] == 0){
                    null_num = i;
                    break;
                }
            }
            if (null_num == 0){
                cout << "Переполнение буффера числа" << endl;
                correct = false;
            }

            // !!! проверка длины int и real !!!


            if (correct) { // 1.3) проверка символов и поиск точки для положительного
                for (int i = 0; i < null_num; i++){

                    if (buff[i] == '.') {
                        dot_count++;
                        dot_num = i;
                    }
                    if ((buff[i] < '0' || buff[i] > '9') &&
                        (buff[i] < 'A' || buff[i] > 'F') && (buff[i] != '.') && ((int)buff[i] != 0)){
                        cout << "Некорректный символ: " << buff[i] << endl;
                        correct = false;
                    }
                }
            }
        }


        // обработка числа
        if (dot_count == 1 && correct){
            int a = 0;

            for (int i = dot_num - 1; i > 0; i--){  //Integer
                if (a % 3 == 0) {
                    num.countInt++;
                    num.Integer[num.countInt - 1][0] = 0;
                    num.Integer[num.countInt - 1][1] = 0;
                    num.Integer[num.countInt - 1][2] = 0;
                }
                num.Integer[num.countInt - 1][a % 3] = buff[i];
                a++;
            }

            if(buff[0] == '-'){ // обрадотка первого символа буффера
                num.sign = false;
            } else {
                if (a % 3 == 0) {
                    num.countInt++;
                    num.Integer[num.countInt - 1][0] = 0;
                    num.Integer[num.countInt - 1][1] = 0;
                    num.Integer[num.countInt - 1][2] = 0;
                }
                num.Integer[num.countInt - 1][a % 3] = buff[0];
            }

            int b = dot_num + 1; //символ буффера R
            int k = 0; //номер цифры R

            while( (int) buff[b] != 0){
                if (k % 3 == 0) {
                    num.countReal++;
                    num.Real[num.countReal-1][0] = 0;
                    num.Real[num.countReal-1][1] = 0;
                    num.Real[num.countReal-1][2] = 0;
                }
                num.Real[num.countReal-1][k % 3] = buff[b];
                k++;
                b++;
            }
        } else cout << "более одной точки в числе или неверный символ" << endl;
    }
    return num;
}

void writeTlong(TLong num){ // вывод числа типа TLong
    for(int i =  num.countInt - 1; i >= 0; i--){
        for(int j = 2; j >= 0 ; j--){
            cout << num.Integer[i][j];
        }
    }

    cout << ".";
    for(int i = 0; i < num.countReal; i++){
        for(int j = 0; j < 3 ; j++){
            cout << num.Real[i][j];
        }
    }
}

TLong sumTLong (TLong A, TLong B){ // A + B
    TLong C;
    int help = 0;

    if (A.countReal >= B.countReal) {C.countReal = A.countReal;}
    else {C.countReal = B.countReal;}

    if (A.countInt >= B.countInt) C.countInt = A.countInt;
    else C.countInt = B.countInt;

    for (int i = C.countReal - 1; i >= 0; i--){ // складываем Real
            for(int j = 2; j >= 0 ; j--){
                // складываем real A + B
                if (i + 1 > A.countReal || A.Real[i][j] == '\0'){
                    help = pos(B.Real[i][j]) + (help / 16);
                    C.Real[i][j] = num_hex[help % 16];
                } else if (i + 1 > B.countReal || B.Real[i][j] == '\0') {
                    help = pos(A.Real[i][j]) + (help / 16);
                    C.Real[i][j] = num_hex[help % 16];
                } else {
                    help = pos(A.Real[i][j]) + pos(B.Real[i][j]) + (help / 16);
                    C.Real[i][j] = num_hex[help % 16];
                }
            }
    }

    for (int i = 0; i < C.countInt; i++){ // складываем Int
        for (int j = 0; j <= 2; j++){
            if (A.Integer[i][j] == '\0' && B.Integer[i][j] == '\0'){
                help = (help / 16);
                if (help != 0) C.Integer[i][j] = num_hex[1];
                else C.Integer[i][j] = 0;
            } else if (i + 1 > A.countInt || A.Integer[i][j] == '\0'){
                help = pos(B.Integer[i][j]) + (help / 16);
                C.Integer[i][j] = num_hex[help % 16];
            } else if (i + 1 > B.countInt || B.Integer[i][j] == '\0') {
                help = pos(A.Integer[i][j]) + (help / 16);
                C.Integer[i][j] = num_hex[help % 16];
            } else {
                help = pos(A.Integer[i][j]) + pos(B.Integer[i][j]) + (help / 16);
                C.Integer[i][j] = num_hex[help % 16];
            }
        }
    }
    // прописать для случая когда + 1 countInt
    if (help / 16 == 1){
        C.countInt++;
        C.Integer[C.countInt - 1][0] = '1';
        C.Integer[C.countInt - 1][1] = 0;
        C.Integer[C.countInt - 1][2] = 0;
    }
    return C;
}

TLong subTLong (TLong A, TLong B){ // A - B
    TLong C;
    int help = 0;
    int remainder = 0;
///////////////////////////////////////////////

//    if (A.countReal >= B.countReal) {C.countReal = A.countReal;}
//    else {C.countReal = B.countReal;}
//
//    if (A.countInt >= B.countInt) C.countInt = A.countInt;
//    else C.countInt = B.countInt;

if (LessTLong(A, B)){
    C.countInt = B.countInt;
    if (A.countReal >= B.countReal) {
        C.countReal = A.countReal;
    } else {
        C.countReal = B.countReal;
    }

    for (int i = C.countReal - 1; i >= 0; i--){ // складываем Real    A < B => B - A
        for(int j = 2; j >= 0 ; j--){
            if (i + 1 > A.countReal || A.Real[i][j] == '\0'){
                C.Real[i][j] = B.Real[i][j];
            } else if (i + 1 > B.countReal || B.Real[i][j] == '\0') {
                if (pos(A.Real[i][j]) != 0) {
                    C.Real[i][j] = num_hex[16 - pos(A.Real[i][j]) - remainder];
                    remainder = 1;
                } else {
                    C.Real[i][j] = num_hex[0];
                }
            } else {
                help = pos(B.Real[i][j]) - pos(A.Real[i][j]) - remainder;
                if (help < 0) {
                    C.Real[i][j] = num_hex[16 + help];
                    remainder = 1;
                } else {
                    C.Real[i][j] = num_hex[help];
                    remainder = 0;
                }
            }

        }
    }

} else {



}

//    for (int i = C.countReal - 1; i >= 0; i--){ // складываем Real
//        for(int j = 2; j >= 0 ; j--){
//            // складываем real A + B
//            if (i + 1 > A.countReal || A.Real[i][j] == '\0'){
//                help = pos(B.Real[i][j]) + (help / 16);
//                C.Real[i][j] = num_hex[help % 16];
//            } else if (i + 1 > B.countReal || B.Real[i][j] == '\0') {
//                help = pos(A.Real[i][j]) + (help / 16);
//                C.Real[i][j] = num_hex[help % 16];
//            } else {
//                help = pos(A.Real[i][j]) + pos(B.Real[i][j]) + (help / 16);
//                C.Real[i][j] = num_hex[help % 16];
//            }
//        }
//    }
//
//    for (int i = 0; i < C.countInt; i++){ // складываем Int
//        for (int j = 0; j <= 2; j++){
//            if (A.Integer[i][j] == '\0' && B.Integer[i][j] == '\0'){
//                help = (help / 16);
//                if (help != 0) C.Integer[i][j] = num_hex[1];
//                else C.Integer[i][j] = 0;
//            } else if (i + 1 > A.countInt || A.Integer[i][j] == '\0'){
//                help = pos(B.Integer[i][j]) + (help / 16);
//                C.Integer[i][j] = num_hex[help % 16];
//            } else if (i + 1 > B.countInt || B.Integer[i][j] == '\0') {
//                help = pos(A.Integer[i][j]) + (help / 16);
//                C.Integer[i][j] = num_hex[help % 16];
//            } else {
//                help = pos(A.Integer[i][j]) + pos(B.Integer[i][j]) + (help / 16);
//                C.Integer[i][j] = num_hex[help % 16];
//            }
//        }
//    }
//    // прописать для случая когда + 1 countInt
//    if (help / 16 == 1){
//        C.countInt++;
//        C.Integer[C.countInt - 1][0] = '1';
//        C.Integer[C.countInt - 1][1] = 0;
//        C.Integer[C.countInt - 1][2] = 0;
//    }

///////////////////////////////////
    return C;
}

bool LessTLong (TLong A, TLong B){ // A < B
    if (A.countInt < B.countInt){
        return true;
    } else if (B.countInt < A.countInt) {
        return false;
    } else {
        for (int i =  A.countInt - 1; i >= 0; i--){
            for(int j = 2; j >= 0 ; j--){
                if (pos(A.Integer[i][j]) > pos(B.Integer[i][j])) return false;
                else if (pos(A.Integer[i][j]) < pos(B.Integer[i][j])) return true;
            }
        }

        if (A.countReal < B.countReal){
            for (int i = 0; i < A.countReal; i++){
                for(int j = 0; j < 3 ; j++){
                    if (pos(A.Real[i][j]) > pos(B.Real[i][j])) return false;
                    else if (pos(A.Real[i][j]) < pos(B.Real[i][j])) return true;
                }
            }
            return true;
        } else {
            for (int i = 0; i < B.countReal; i++){
                for(int j = 0; j < 3 ; j++){
                    if (pos(A.Real[i][j]) > pos(B.Real[i][j])) return false;
                    else if (pos(A.Real[i][j]) < pos(B.Real[i][j])) return true;
                }
            }
            return false;
        }
    }
}

bool EQTLong (TLong A, TLong B){ // A == B
    if (A.countReal == B.countReal && A.countInt == B.countInt){
        for (int i =  A.countInt - 1; i >= 0; i--){
            for(int j = 2; j >= 0 ; j--){
                if (A.Integer[i][j] != B.Integer[i][j]) return false;
            }
        }

        for (int i = 0; i < A.countReal; i++){
            for(int j = 0; j < 3 ; j++){
                if (A.Real[i][j] != B.Real[i][j]) return false;
            }
        }
    } else return false;
    return true;
}

int main() {
    TLong num, num2;
    ifstream inFile("C:/Users/BedTed/CLionProjects/labaK2N1/cmake-build-debug/inFile.txt");

    num = readTLong(inFile);
    num2 = readTLong(inFile);
    //writeTlong(num);
    //cout << " < ";
    //writeTlong(num2);
    writeTlong(subTLong(num, num2));


   // cout << EQTLong(num,num2);
    cout << endl;
    //writeTlong(sumTLong(num,num2));
    //cout << LessTLong(num,num2); // num < num2
    //cout << (LessTLong(num,num2) ? "Да" : "Нет"); // num < num2
    cout << endl;
    inFile.close();
}

