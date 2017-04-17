#include <cstdio>
#include <cmath>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <complex>
#include <vector>
using namespace std;

#ifndef _ITCG_COMMON_FUNCTION_ZLM_

	#define _ITCG_COMMON_FUNCTION_ZLM_

	const double PI = acos(-1.0);
	const complex< double > J = complex< double >(0.0, -1.0);

	class CodeWord {
    public:
        int *cw;
        int len;
        int base;

        CodeWord(int l, int b);
        ~CodeWord();

        CodeWord operator + (const CodeWord &t) const {
        	CodeWord rst(len, base);
		    for(int i = 0; i < len; ++i)
		        rst.cw[i] = (cw[i] + t.cw[i]) % base;
		    return rst;
        }

        const CodeWord& operator += (const CodeWord &t) {
        	for(int i = 0; i < len; ++i)
        		cw[i] = (cw[i] + t.cw[i]) % base;
        	return *this;
        }

        CodeWord& operator [] (int idx) {
        	return cw[idx];
        }

        bool operator == (const CodeWord &t) {
        	if(len != t.len) return false;
        	for(int i = 0; i < len; ++i)
        		if(cw[i] != t.cw[i])
        			return false;
        	return true;
        }
	};


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

	int HammingDistance(const CodeWord &a, const CodeWord &b) {
		int rst = 0;
		for(int i = 0; i < a.len; ++i)
			rst += (a[i] != b[i]);
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

	double CntSNR(double sigal_power, double noise_power) {return 10.0 * log10(sigal_power / noise_power);}

	void Debug(int val) {printf("\nDebug:%d\n", val);}
	void Debug(double val) {printf("\nDebug:%f\n", val);}
	void Debug(char *str) {printf("\nDebug:%s\n", str);}
	void Debug(char *var_name, int val) {printf("\nDebug:%s=%d\n", var_name, val);}
	void Debug(char *var_name, double val) {printf("\nDebug:%s=%f\n", var_name, val);}
	void Debug(char *var_name, char *str) {printf("\nDebug:%s=%s\n", var_name, str);}

#endif
