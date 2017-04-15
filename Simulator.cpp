#include "Simulator.h"

Simulator::Simulator() {

}

Simulator::~Simulator() {

}

void Simulator::Init(const char *config_file_path) {
    char encoder_config_file_path[50];
    char channel_config_file_path[50];
    char str[50] = "";
    char output_file_path[50];

    FILE *in_file = OpenFile(config_file_path, "r");
    while(~fscanf(in_file, "%[a-z_]=", str)) {
        if(!strcmp(str, "bit_num"))
            fscanf(in_file, "%d", &bit_num);
        else if(!strcmp(str, "encoder_config_file"))
            fscanf(in_file, "%s", encoder_config_file_path);
        else if(!strcmp(str, "channel_config_file"))
            fscanf(in_file, "%s", channel_config_file_path);
        else if(!strcmp(str, "output_file"))
            fscanf(in_file, "%s", output_file_path);
        else if(!strcmp(str, "min_snr"))
            fscanf(in_file, "%lf", &min_snr);
        else if(!strcmp(str, "max_snr"))
            fscanf(in_file, "%lf", &max_snr);
        else if(!strcmp(str, "snr_step"))
            fscanf(in_file, "%lf", &snr_step);
        str[0] = '\0';
        fscanf(in_file, "%*c");
    }

    enc1.Init(encoder_config_file_path);
    chn1.Init(channel_config_file_path);
    Debug("bit_num", bit_num);
    blk_num = enc1.CalcBlkNum(bit_num);
    Debug("blk_num", blk_num);
    source_signal = new int[bit_num];
    encoded_signal = new int[blk_num * enc1.codeword_len];
    send_signal = new int[blk_num * enc1.codeword_len];
    receive_signal = new double[blk_num * enc1.codeword_len];
    restore_codeword = new int[blk_num * enc1.codeword_len];
    restore_signal = new int[bit_num];
    
    out_file = OpenFile(output_file_path, "w");

}

void Simulator::Start() {
    SetSeed(-1);
    for(snr = min_snr; snr <= max_snr; snr += snr_step) {
        err_bit_cnt = err_blk_cnt = 0;
        chn1.SetNoisePower(snr, Modulater::CntSignalPower());

        SourceCreate::CreateBitStream(source_signal, bit_num);
        enc1.Encode(source_signal, encoded_signal, blk_num);
        Modulater::Modulate(encoded_signal, send_signal, blk_num * enc1.codeword_len);
        chn1.transmit(send_signal, receive_signal, blk_num * enc1.codeword_len);
        enc1.Decode4BPSK_AWGN(receive_signal, restore_codeword, restore_signal, blk_num);

        for(int i = 0; i < bit_num; ++i)
            if(source_signal[i] != restore_signal[i])
                ++err_bit_cnt;
        for(int i = 0; i < blk_num; ++i)
            if(HammingDistance(encoded_signal + (i * enc1.codeword_len), restore_codeword + (i * enc1.codeword_len), enc1.codeword_len))
                ++err_blk_cnt;
        ber = (double)err_bit_cnt / bit_num;
        fer = (double)err_blk_cnt / blk_num;
        DisplayResult();
    }
}

void Simulator::Clean() {
    delete[] source_signal;
    delete[] encoded_signal;
    delete[] send_signal;
    delete[] receive_signal;
    delete[] restore_codeword;
    delete[] restore_signal;
    enc1.Clean();
    chn1.Clean();
    fclose(out_file);
}

void Simulator::DisplaySourceSignal() {
    fprintf(out_file, "The source signal is:\n");
    fprintf(out_file, "%d", source_signal[0]);
    for(int i = 1; i < bit_num; ++i)
        fprintf(out_file, " %d", source_signal[i]);
    fprintf(out_file, "\n\n");
}

void Simulator::DisplayEncodedSignal() {
    int idx;
    fprintf(out_file, "The encoded signal is:\n");
    for(int i = 0; i < blk_num; ++i) {
        idx = i * enc1.codeword_len;
        fprintf(out_file, "%d", encoded_signal[idx]);
        for(int j = 1; j < enc1.codeword_len; ++j)
            fprintf(out_file, " %d", encoded_signal[idx + j]);
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n\n");
}

void Simulator::DisplaySendSignal() {
    int idx;
    fprintf(out_file, "The sent signal is:\n");
    for(int i = 0; i < blk_num; ++i) {
        idx = i * enc1.codeword_len;
        fprintf(out_file, "%d", send_signal[idx]);
        for(int j = 1; j < enc1.codeword_len; ++j)
            fprintf(out_file, " %d", send_signal[idx + j]);
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n\n");
}

void Simulator::DisplayReceiveSignal() {
    int idx;
    fprintf(out_file, "The received signal is:\n");
    for(int i = 0; i < blk_num; ++i) {
        idx = i * enc1.codeword_len;
        fprintf(out_file, "%f", receive_signal[idx]);
        for(int j = 1; j < enc1.codeword_len; ++j)
            fprintf(out_file, " %f", receive_signal[idx + j]);
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n\n");
}

void Simulator::DisplayRestoreCodeword() {
    int idx;
    fprintf(out_file, "The restored codeword is:\n");
    for(int i = 0; i < blk_num; ++i) {
        idx = i * enc1.codeword_len;
        fprintf(out_file, "%d", restore_codeword[idx]);
        for(int j = 1; j < enc1.codeword_len; ++j)
            fprintf(out_file, " %d", restore_codeword[idx + j]);
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n\n");
}

void Simulator::DisplayRestoreSignal() {
    fprintf(out_file, "The restored signal is:\n");
    fprintf(out_file, "%d", restore_signal[0]);
    for(int i = 1; i < bit_num; ++i)
        fprintf(out_file, " %d", restore_signal[i]);
    fprintf(out_file, "\n\n");
}

void Simulator::DisplayResult() {
    /*
    DisplaySourceSignal();
    DisplayEncodedSignal();
    DisplaySendSignal();
    DisplayReceiveSignal();
    DisplayRestoreCodeword();
    DisplayRestoreSignal();
    */
    fprintf(out_file, "SNR=%.f, the BER=%.10f, the FER=%.10f\n", snr, ber, fer);
}
