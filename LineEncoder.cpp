#include "LineEncoder.h"

Encoder::Encoder() {

}

Encoder::~Encoder() {

}

void Encoder::Init(const char *config_file_path) {
    char str[50] = "";
    FILE *in_file = OpenFile(config_file_path, "r");
    while(~fscanf(in_file, "%[a-z_]=", str)) {
        if(!strcmp(str, "codeword_len")) {
            fscanf(in_file, "%d", &codeword_len);
        }
        else if(!strcmp(str, "codeword_num")) {
            fscanf(in_file, "%d", &codeword_num);
            int cw = codeword_num;
            message_blk_len = 0;
            cw >>= 1;
            while(cw) {
                ++message_blk_len;
                cw >>= 1;
            }
        }
        else if(!strcmp(str, "message_blk_len")) {
            fscanf(in_file, "%d", &message_blk_len);
            codeword_num = 1 << message_blk_len;
        }
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
    int i, j, in_idx = 0, target_codeword_idx, out_idx = 0;
    for(i = 0; i < blk_num; ++i) {
        target_codeword_idx = 0;
        for(j = 0; j < message_blk_len; ++j, ++in_idx)
            target_codeword_idx = (target_codeword_idx << 1) | in[in_idx];
        for(j = 0; j < codeword_len; ++j, ++out_idx)
            out[out_idx] = codebook[target_codeword_idx][j];
    }
}

void Encoder::Decode4BPSK_AWGN(double *in, int *imd, int *out, int blk_num) {
    int i, j, imd_idx = 0, m, dis, target_codeword_idx, out_idx = 0;
    for(i = 0; i < blk_num * codeword_len; ++i) {
        imd[i] = (in[i] > 0 ? 0 : 1);
    }

    for(i = 0; i < blk_num; ++i) {
        m = HammingDistance(imd + imd_idx, codebook[0], codeword_len);
        target_codeword_idx = 0;
        for(j = 1; j < codeword_num; ++j) {
            dis = HammingDistance(imd + imd_idx, codebook[j], codeword_len);
            if(dis < m) {
                target_codeword_idx = j;
                m = dis;
            }
        }

        for(j = 0; j < codeword_len; ++j, ++imd_idx)
            imd[imd_idx] = codebook[target_codeword_idx][j];

        for(j = 0; j < message_blk_len; ++j, ++out_idx) {
            out[out_idx] = (target_codeword_idx >> (message_blk_len - 1 - j)) & 1;
        }
    }
}

void Encoder::Clean() {
    for(int i = 0; i < codeword_num; ++i)
        delete[] codebook[i];
    delete[] codebook;
}

int Encoder::CalcBlkNum(int bit_num) {
    return bit_num / message_blk_len;
}
