#include "SourceCreate.h"

SourceCreate::SourceCreate() {

}

SourceCreate::~SourceCreate() {

}

void SourceCreate::Init(const char *config_file_path) {

}

void SourceCreate::CreateBitStream(int *bit_stream, int len) {
	//long int seed = time(NULL);
	for(int i = 0; i < len; ++i)
		bit_stream[i] = (Uniform() < 0.5 ? 0 : 1);
}

void Clean() {
	
}