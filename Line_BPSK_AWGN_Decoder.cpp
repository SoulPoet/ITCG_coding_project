#include "Line_BPSK_AWGN_Decoder.h"

Decoder::Decoder() {

}

Decoder::~Decoder() {

}

void Decoder::Init(char *config_file_path) {
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
    all_codeword = new int*[1 << codeword_num];
    for(int i = 0; i < (1 << codeword_num); ++i) {
        all_codeword[i] = new int[codeword_len];
        for(int j = 0; j < codeword_len; ++j)
            all_codeword[i][j] = 0;
        for(int j = 0; j < codeword_num; ++j)
            if(i & (1 << j)) {
                for(int k = 0; k < codeword_len; ++k)
                    all_codeword[i][k] ^= codebook[j][k];
            }
    }
}

void Decoder::Decode(const Encoder &enc, const Channel &chn, double *in, int *imd, int *out, int blk_num) {
    for(int i = 0; i < blk_num * codeword_len; ++i) {
        imd[i] = (in[i] > 0 ? 0 : 1);
    }

    for(int i = 0; i < blk_num; ++i) {
        int m = HammingDistance(imd + (i * codeword_len), all_codeword[0], codeword_len), codeword_idx = 0;
        for(int j = 1; j < (1 << codeword_num); ++j) {
            int dis = HammingDistance(imd + (i * codeword_len), all_codeword[j], codeword_len);
            if(dis < m) {
                codeword_idx = j;
                m = dis;
            }
        }
        for(int j = 0; j < codeword_len; ++j)
            imd[i * codeword_len + j] = all_codeword[codeword_idx][j];
        for(int j = 0; j < codeword_num; ++j)
            out[i * codeword_num + j] = (codeword_idx & (1 << j)) ? 1 : 0;
    }
}

void Decoder::Clean() {
    for(int i = 0; i < codeword_num; ++i)
        delete[] codebook[i];
    delete[] codebook;
}
