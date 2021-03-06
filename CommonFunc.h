#include <cstdio>
#include <cmath>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <complex>
#include <vector>
#include <queue>
#include <map>
#include <stack>
#include <set>
#include <cctype>
using namespace std;

#ifndef _ITCG_COMMON_FUNCTION_

	#define _ITCG_COMMON_FUNCTION_

	const double PI = acos(-1.0);


	class CodeWord {
    public:
        int *cw;
        int len;

        CodeWord(int l);
        ~CodeWord();

        CodeWord operator + (const CodeWord &t) const;

	};


	void SetSeed(int flag);
	void PrintState(FILE *fp);
	double Uniform(double a, double b, long int &seed);
	double Uniform();
	double Gauss(double mean, double segma, long int &seed);
	double Normal(double mean, double segma);
	int HammingDistance(int *str1, int *str2, int len);
	FILE* OpenFile(const char *file_path, const char *mode);
	double SNR(double sigal_power, double noise_power);
	void Debug(char *var_name, int val);
	void Debug(char *var_name, double val);
	void Debug(char *var_name, char *str);

#endif
