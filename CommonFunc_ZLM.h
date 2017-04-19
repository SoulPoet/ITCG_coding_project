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

	class BaseVector {
	public:
		vector< int > m_v;
		int m_base;

		BaseVector() {m_base = 2;}
		BaseVector(int len, int b) : m_v(len, 0), m_base(b) {}
		~BaseVector() {}

		void resize(int len, int val = 0) {
			m_v.resize(len, val);
		}

		int size() {
			return m_v.size();
		}

		void clear() {
			m_v.clear();
		}

		BaseVector operator + (const BaseVector &t) const {
        	BaseVector rst(m_v.size(), base);
		    for(int i = 0; i < m_v.size(); ++i)
		        rst.m_v[i] = (m_v[i] + t.m_v[i]) % base;
		    return rst;
        }

        const BaseVector& operator += (const BaseVector &t) {
        	for(int i = 0; i < m_v.size(); ++i)
        		m_v[i] = (m_v[i] + t.m_v[i]) % base;
        	return *this;
        }

        BaseVector& operator [] (int idx) const {
        	return m_v[idx];
        }

        bool operator == (const BaseVector &t) const {
        	if(m_v.size() != t.m_v.size()) return false;
        	for(int i = 0; i < m_v.size(); ++i)
        		if(m_v[i] != t.m_v[i])
        			return false;
        	return true;
        }

        int operator - (const BaseVector &t) const {
        	int rst = 0, e = min(m_v.size(), t.m_v.size());
        	for(int i = 0; i < e; ++i)
        		rst += (m_v[i] != t.m_v[i]);
        }
	};

	typedef BaseVector CodeWord;

	class SignalVector {
	public:
		vector< double > m_v;

		SignalVector() {}
		SignalVector(int len) : m_v(len, 0.0) {}
		~SignalVector() {}

		void resize(int len, double val = 0.0) {
			m_v.resize(len, val);
		}

		int size() {
			return m_v.size();
		}

		void clear() {
			m_v.clear();
		}

		SignalVector operator + (const SignalVector &t) const {
        	SignalVector rst(m_v.size());
		    for(int i = 0; i < m_v.size(); ++i)
		        rst.m_v[i] = m_v[i] + t.m_v[i];
		    return rst;
        }

        const SignalVector& operator += (const SignalVector &t) {
        	for(int i = 0; i < m_v.size(); ++i)
        		m_v[i] += t.m_v[i];
        	return *this;
        }

        SignalVector& operator [] (int idx) const {
        	return m_v[idx];
        }

        bool operator == (const SignalVector &t) const {
        	if(m_v.size() != t.m_v.size()) return false;
        	for(int i = 0; i < m_v.size(); ++i)
        		if(m_v[i] != t.m_v[i])
        			return false;
        	return true;
        }
	};

	/*
	class CodeWord {
    public:
        vector< int > cw;
        int base;

        CodeWord(int l, int b) : {}
        ~CodeWord() {}

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
	*/

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

	/*
	int HammingDistance(const CodeWord &a, const CodeWord &b) {
		int rst = 0;
		for(int i = 0; i < a.len; ++i)
			rst += (a[i] != b[i]);
		return rst;
	}
	*/

	FILE* OpenFile(const char *file_path, const char *mode) {
		FILE *fp = fopen(file_path, mode);
	    if(fp == NULL) {
	        fprintf(stderr, "\nCannot open the file \"%s\"!!!\n", file_path);
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
