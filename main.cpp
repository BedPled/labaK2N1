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

#include <string>
#include <iostream>
#include <fstream>

using namespace std;
const char num_hex[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
const int num_I = 5; // кол-во полей в массиве, который хранит целую часть
const int num_R = 5; // кол-во полей в массиве, который хранит вещественную часть
const int length_buff = num_I * 3 + num_R * 3 + 3; // 3 тк '-' '.' '\0'

struct TLong {
    char Integer[num_I][3]; // хранение целой части
    char Real[num_R][3]; // хранение дробной части
    int countReal = 0; // кол-во занятых ячеек дробной части
    int countInt = 0; // кол-во занятых ячеек целой части
    bool sign = true; // 1: +  |   0: -
};

int pos(char a); // поиск цифры в массиве шестнадцетиричных цифр 0 - F (/0 == 0)
TLong additionTLong (TLong A, TLong B); // |A| + |B|
TLong subtractionTLong (TLong A, TLong B); // |A| + |B| (A > B)
void makeItNormal (TLong& A); // убирает лишние нули и пустые

void readTLong(ifstream &inFile, TLong &A, bool &correct); // считывание числа типа TLong из файла. correct - true число корректно, False - некорректно
void writeTlong(ofstream &outFile, TLong num); // вывод числа типа TLong
TLong sumTLong (TLong A, TLong B); // A + B
TLong subTLong (TLong A, TLong B); // A - B
bool isLessTLong (TLong A, TLong B); // |A| < |B|
bool isEQTLong (TLong A, TLong B); // A == B

int main() {
    int i = 1;
    char buff[5];
    bool correct = true;
    TLong num1, num2, result;

    ofstream outFile("C:/Users/BedTed/CLionProjects/labaK2N1/cmake-build-debug/outFile.txt");
    ifstream inFile("C:/Users/BedTed/CLionProjects/labaK2N1/cmake-build-debug/inFile.txt");


    readTLong(inFile, num1, correct); // ввод первого числа
    if (correct && !inFile.eof()){
        while (!inFile.eof()) {
            i++;
            inFile >> buff;
            if (!inFile.eof()){
                readTLong(inFile, num2, correct);

                if (correct && buff[1] == '\0') {
                    if (buff[0] == '-') {
                        result = subTLong(num1, num2);
                    } else if (buff[0] == '+') {
                        result = sumTLong(num1, num2);
                    } else {
                        outFile << "Ошибка ввода знака операции " << i - 1 << endl;
                        return -1;
                        //break;
                    }
                } else {
                    if (!correct) outFile << "Ошибка ввода числа " << i << endl;
                    if (buff[1] != '\0') outFile << "Ошибка ввода знака " << i - 1 << endl;
                    return -1;
                    //break;
                }
            } else {
                outFile << "Нет числа после знака " << i << endl;
                return -1;
            }

            num1 = result;
        }
    } else {
        outFile << "Ошибка ввода числа 1 " << endl;
        return -1;
    }

    writeTlong(outFile, result);

    outFile.close();
    inFile.close();
    }

void makeItNormal (TLong &A){
bool flag = false;
int countInt = A.countInt;
int countReal = A.countReal;

    for (int i = A.countInt - 1; i >= 0; i--) { // складываем Int
        for (int j = 2; j >= 0; j--) {
            if (A.Integer[i][j] != '0' && A.Integer[i][j] != '\0' || i == 0 && j == 0){
                flag = true;
                break;
            } else {
                if (A.Integer[i][j] == '0') {
                    A.Integer[i][j] = '\0';
                }
            }
        }
        if (flag) {
            break;
        } else countInt--;
    }

    A.countInt = countInt;
    flag = false;

    for (int i =  A.countReal - 1; i >= 0; i--) { // складываем Real    A >= B -> A - B
        for (int j = 2; j >= 0; j--) {
            if (A.Real[i][j] != '0' && A.Real[i][j] != '\0' || i == 0 && j == 0){
                flag = true;
                break;
            } else {
                if (A.Real[i][j] == '0') {
                    A.Real[i][j] = '\0';
                }
            }
        }
        if (flag) {
            break;
        } else countReal--;
    }
    A.countReal = countReal;

//    if (A.countReal == 1 && A.countInt == 1){
//        if (A.Real[0][0] == '0' && A.Integer[0][0] == '0' && A.Real[0][1] == '\0' && A.Integer[0][1] == '\0') {
//            A.sign = true;
//        }
//    }
}

int pos(char a){
    int i = 0;
    while (num_hex[i] != a && i < 16 && (int)a != 0){
        i++;
    }
    if (i < 16) return i;
    else return -1;
}

void readTLong(ifstream &inFile, TLong &A, bool &correct){
    TLong num;
    int Inum = 0;
    int Rnum = 0;
    int dot_num = 0;
    int dot_count = 0;
    correct = true;
    int null_num = 0; // номер '/0' в буфере

    if (inFile.is_open()) {
        // буфер промежуточного хранения ['-'] + int + '.' + real + ' '
        char buff[length_buff];
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
                    } else {
                        if (dot_count == 0) Inum++;
                        if (dot_count == 1) Rnum++;
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
                    } else {
                        if (dot_count == 0) Inum++;
                        if (dot_count == 1) Rnum++;
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
        if (dot_count == 1 && correct && Inum > 0 && Rnum > 0){
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
        } else {
            correct = false;
            cout << "некорректное число" << endl;
        }
    } else cout << "файл закрыт" << endl;
    A = num;
}

void writeTlong(ofstream &outFile, TLong num){
    if (!num.sign){
        outFile << '-';
    }

    for(int i =  num.countInt - 1; i >= 0; i--){
        for(int j = 2; j >= 0 ; j--){
            if (num.Integer[i][j] != '\0') outFile << num.Integer[i][j];
        }
    }

    outFile << ".";
    for(int i = 0; i < num.countReal; i++){
        for(int j = 0; j < 3 ; j++){
            if (num.Real[i][j] != '\0') outFile << num.Real[i][j];
        }
    }
}

TLong additionTLong (TLong A, TLong B){ // A + B
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
    // случая когда + 1 countInt
    if (help / 16 == 1){
        if (C.countInt < num_I) C.countInt++;
        else if (C.countInt == num_I) cout << "ПЕРЕПОЛНЕНИЕ ЧИСЛА" << endl;
        C.Integer[C.countInt - 1][0] = '1';
        C.Integer[C.countInt - 1][1] = 0;
        C.Integer[C.countInt - 1][2] = 0;
    }
    makeItNormal(C);
    return C;
}

TLong subtractionTLong (TLong A, TLong B){ // A - B
    TLong C;
    int help = 0;
    int remainder = 0;

        C.countInt = A.countInt;
        if (A.countReal >= B.countReal) {
            C.countReal = A.countReal;
        } else {
            C.countReal = B.countReal;
        }

        for (int i = C.countReal - 1; i >= 0; i--){ // складываем Real    A >= B -> A - B
            for(int j = 2; j >= 0 ; j--){
                if (i + 1 > A.countReal || A.Real[i][j] == '\0'){
                    if (pos(A.Real[i][j]) != 0) {
                        C.Real[i][j] = num_hex[16 - pos(B.Real[i][j]) - remainder];
                        remainder = 1;
                    } else {
                        C.Real[i][j] = num_hex[0];
                    }
                } else if (i + 1 > B.countReal || B.Real[i][j] == '\0') {
                    C.Real[i][j] = A.Real[i][j];
                } else {
                    help = pos(A.Real[i][j]) - pos(B.Real[i][j]) - remainder;
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

        for (int i = 0; i < C.countInt; i++){ // складываем Int
            for (int j = 0; j <= 2; j++){
                if (i + 1 > A.countInt || A.Integer[i][j] == '\0'){
                    if (pos(A.Integer[i][j]) != 0) {
                        C.Integer[i][j] = num_hex[16 - pos(B.Integer[i][j]) - remainder];
                        remainder = 1;
                    } else {
                        C.Integer[i][j] = num_hex[0];
                    }
                } else if (i + 1 > B.countInt || B.Integer[i][j] == '\0') {
                    C.Integer[i][j] = num_hex[pos(A.Integer[i][j]) - remainder];
                } else {
                    help = pos(A.Integer[i][j]) - pos(B.Integer[i][j]) - remainder;
                    if (help < 0) {
                        C.Integer[i][j] = num_hex[16 + help];
                        remainder = 1;
                    } else {
                        C.Integer[i][j] = num_hex[help];
                        remainder = 0;
                    }
                }
            }
        }
    makeItNormal(C);
    return C;
}

TLong sumTLong (TLong A, TLong B){
    TLong C;
    if (A.sign && B.sign) {                  // A + B
        C = additionTLong (A, B);
        C.sign = true;
        // убрать лишнее если нужно
    } else if (A.sign && !B.sign) {          // A - B
        if (isLessTLong(B, A)){         // |B| < |A| ?
            C = subtractionTLong(A, B);
            C.sign = true;
        } else {
            C = subtractionTLong(B, A);
            C.sign = false;
        }
        // убрать лишнее если нужно
    } else if (!A.sign && B.sign) {         // - A + B
        if (isLessTLong(B, A)){// |B| < |A| ?
            C = subtractionTLong(A, B);
            C.sign = false;
        } else {
            C = subtractionTLong(B, A);
            C.sign = true;
        }
        // убрать лишнее если нужно
    } else {                                // - A - B
        C = additionTLong (A, B);
        C.sign = false;
        // убрать лишнее если нужно
    }

    if (C.countReal == 1 && C.countInt == 1){
        if (C.Real[0][0] == '0' && C.Integer[0][0] == '0' && C.Real[0][1] == '\0' && C.Integer[0][1] == '\0') {
            C.sign = true;
        }
    }

    return C;
}

TLong subTLong (TLong A, TLong B){
    TLong C;
    if (A.sign && B.sign) {             // A - B
        if (isLessTLong(B, A)){// |B| < |A| ?
            C = subtractionTLong(A, B);
            C.sign = true;
        } else {
            C = subtractionTLong(B, A);
            C.sign = false;
        }
        // убрать лишнее если нужно
    } else if (A.sign && !B.sign) {     // A + B
        C = additionTLong (A, B);
        C.sign = true;
        // убрать лишнее если нужно
    } else if (!A.sign && B.sign) {     // - A - B
        C = additionTLong (A, B);
        C.sign = false;
        // убрать лишнее если нужно
    } else {                            // - A + B
        if (isLessTLong(B, A)){// |B| < |A| ?
            C = subtractionTLong(A, B);
            C.sign = false;
        } else {
            C = subtractionTLong(B, A);
            C.sign = true;
        }
        // убрать лишнее если нужно
    }
    if (C.countReal == 1 && C.countInt == 1){
        if (C.Real[0][0] == '0' && C.Integer[0][0] == '0' && C.Real[0][1] == '\0' && C.Integer[0][1] == '\0') {
            C.sign = true;
        }
    }
    return C;
}

bool isLessTLong (TLong A, TLong B){
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

bool isEQTLong (TLong A, TLong B){
    if (A.countReal == B.countReal && A.countInt == B.countInt && A.sign == B.sign){
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


