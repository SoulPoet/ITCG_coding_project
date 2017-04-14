#include "LineEncoder.h"

Encoder::Encoder() {

}

Encoder::~Encoder() {

}

void Encoder::Init(const char *config_file_path) {
    char str[50] = "";
    FILE *in_file = OpenFile(config_file_path, "r");
    while(~fscanf(in_file, "%[a-z_]=", str)) {
        if(!strcmp(str, "codeword_len"))
            fscanf(in_file, "%d", &codeword_len);
        else if(!strcmp(str, "codeword_num"))
            fscanf(in_file, "%d", &codeword_num);
        else if(!strcmp(str, "codebook")) {
            codebook = new int*[codeword_num];
            for(int i = 0; i < codeword_num; ++i) {
                codebook[i] = new int[codeword_len];
                for(int j = 0; j < codeword_len; ++j)
                    fscanf(in_file, "%d", &codebook[i][j]);
            }
        }
        str[0] = '\0';
        fscanf(in_file, "%*c");
    }
    fclose(in_file);
}

void Encoder::Encode(int *in, int *out, int blk_num) {
    int i, j, k, idx_out;
    for(int i = 0; i < blk_num * codeword_len; ++i)
        out[i] = 0;
    for(i = 0; i < blk_num; ++i) {
        for(j = 0; j < codeword_len; ++j) {
            idx_out = i * codeword_len + j;
            for(k = 0; k < codeword_num; ++k) {
                out[idx_out] ^= in[i * codeword_num + k] * codebook[k][j];
            }
        }
    }
}

void Encoder::Clean() {
    for(int i = 0; i < codeword_num; ++i)
        delete[] codebook[i];
    delete[] codebook;
}

int Encoder::CalcBlkNum(int bit_num) {
    return bit_num / codeword_num;
}
