#include "BPSK_Modem.h"

Modem::Modem() {

}

Modem::~Modem() {

}

void Modem::Init(const char *config_file_path) {

}

void Modem::Modulate(int *in, int *out, int len) {
    for(int i = 0; i < len; ++i)
        out[i] = (in[i] ? -1 : 1);
}

double Modem::CntSignalPower() {
    return 1.0;
}

void Modem::Demodulate_Hard_Decision(double *in, int *out, int len) {
	for(int i = 0; i < len; ++i)
		out[i] = (in[i] > 0 ? 0 : 1);
}

void Modem::Clean() {
	
}