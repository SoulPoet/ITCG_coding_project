#include <cstdio>
#include <cmath>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <complex>
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



	double Uniform(double a, double b, long int &seed);
	double Gauss(double mean, double segma, long int &seed);
	int HammingDistance(int *str1, int *str2, int len);
	FILE* OpenFile(const char *file_path, const char *mode);
	double SNR(double sigal_power, double noise_power);
	void Debug(int val);
	void Debug(double val);
	void Debug(char *str);

#endif
