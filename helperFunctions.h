#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H



#include <cstring>
#define MAX_NAME 30;
#define MIN_PASS 4;
#define MAX_PASS 10;

int getInt(const char* prompt, int min, int max, const char* errorMsg);
char getChar(const char* prompt, const char* validChar, std::istream& = std::cin);
//char* getUsername(const char* prompt, std::istream & = std::cin);
//char* getPass(const char* prompt, std::istream & = std::cin);

char* getString(const char* prompt, int min, int max, std::istream& is = std::cin);
int asciiValidation(int num, const char* str);

#endif // !HELPER_FUNCTIONS_H