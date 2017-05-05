#include "CommonFunc.h"

const int A = 48271;
const long M = 2147483647;
const int Q = M / A;
const int R = M % A;
long int state;
bool normal_flag;

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

void SetSeed(int flag) {
    if (flag < 0)
        state = 17;
    else if (flag == 0) {
        state = 0;
        while (state == 0) {
            srand((unsigned)time(NULL));
            state = rand();
        }
    } else {
        fprintf(stdout, "\nEnter the initial state: ");
        fscanf(stdin, "%ld", &state);
    }
    normal_flag = false;
}

void PrintState(FILE *fp) {
    fprintf(fp, "\n***init_state = %ld***\n", state);
}

double Uniform(double a, double b, long int &seed) {
    seed = 2045 * seed + 1;
    seed %= 1048576;
    double rst = seed / 1048576.0;
    rst = a + (a - b) * rst;
    return rst;
}

double Uniform() {

    double u;

    int tmpState = A * ( state % Q ) - R * ( state / Q );

    if ( tmpState >= 0)
    {
        state = tmpState;
    }
    else
    {
        state = tmpState + M;
    }

    u = state / (double)M;

    return u;
}

double Gauss(double mean, double segma, long int &seed) {
	double rst = 0.0;
	for(int i = 0; i < 12; ++i)
		rst += Uniform(0.0, 1.0, seed);
	rst = mean + (rst - 6.0) * segma;
	return rst;
}

double Normal(double mean, double segma) {
    static double x1, x2, w;

    if(normal_flag) {
        normal_flag = false;
        return mean + segma * x2 * w;
    }

    w = 2.0;
    while (w > 1.0){
        x1 = 2.0 * Uniform() - 1.0;
        x2 = 2.0 * Uniform() - 1.0;

        w = x1 * x1 + x2 * x2;
    }

    w = sqrt(-2.0 * log(w) / w);

    normal_flag = true;

    return mean + segma * x1 * w;

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

void Debug(char *var_name, int val) {
    printf("\nDebug:%s=%d\n", var_name, val);
}

void Debug(char *var_name, double val) {
    printf("\nDebug:%s=%f\n", var_name, val);
}

void Debug(char *var_name, char *str) {
    printf("\nDebug:%s=%s\n", var_name, str);
}
