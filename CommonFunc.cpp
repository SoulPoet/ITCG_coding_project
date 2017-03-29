#include "CommonFunc.h"

CodeWord::CodeWord(int codeword_len) {
    len = codeword_len;
    cw = new int[codeword_len];
}

CodeWord::~CodeWord() {
    delete[] cw;
}

CodeWord CodeWord::operator + (const CodeWord &t) const {
    CodeWord rst(len);
    for(int i = 0; i < len; ++i)
        rst.cw[i] = cw[i] ^ t.cw[i];
    return rst;
}

double Uniform(double a, double b, long int &seed) {
	seed = 2045 * seed + 1;
	seed %= 1048576;
	double rst = seed / 1048576.0;
	rst = a + (a - b) * rst;
    return rst;
}

double Gauss(double mean, double segma, long int &seed) {
	double rst = 0.0;
	for(int i = 0; i < 12; ++i)
		rst += Uniform(0.0, 1.0, seed);
	rst = mean + (rst - 6.0) * segma;
	return rst;
}

int HammingDistance(int *str1, int *str2, int len) {
    int rst = 0;
    for(int i = 0; i < len; ++i)
        if(str1[i] != str2[i])
            ++rst;
    return rst;
}

FILE* OpenFile(const char *file_path, const char *mode) {
    FILE *fp = fopen(file_path, mode);
    if(fp == NULL) {
        fprintf(stderr, "\n Cannot open the file \"%s\"!!!\n", file_path);
        exit(1);
    }
    return fp;
}

double SNR(double sigal_power, double noise_power) {
    return 10.0 * log10(sigal_power / noise_power);
}

void Debug(int val) {
    printf("\nDebug:%d\n", val);
}

void Debug(double val) {
    printf("\nDebug:%f\n", val);
}

void Debug(char *str) {
    printf("\nDebug:%s\n", str);
}
