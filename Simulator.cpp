#include "Simulator.h"

Simulator::Simulator() {

}

Simulator::~Simulator() {

}

void Simulator::Init(const char *config_file_path) {
    char codec_config_file_path[50];
    char channel_config_file_path[50];
    char str[50] = "";
    char output_file_path[50];

    m_save_temp_info = false;
    blk_num = 100;

    FILE *in_file = OpenFile(config_file_path, "r");
    while(~fscanf(in_file, "%[a-z_]=", str)) {
        if(!strcmp(str, "bit_num"))
            fscanf(in_file, "%d", &bit_num);
        else if(!strcmp(str, "codec_config_file"))
            fscanf(in_file, "%s", codec_config_file_path);
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
        else if(!strcmp(str, "target_err_blk"))
            fscanf(in_file, "%d", &m_target_err_blk);
        else if(!strcmp(str, "temp_file")) {
            m_save_temp_info = true;
            fscanf(in_file, "%s", m_temp_file);
        }
        str[0] = '\0';
        fscanf(in_file, "%*c");
    }

    objCodec.Init(codec_config_file_path);
    objChannel.Init(channel_config_file_path);
    Debug("bit_num", bit_num);
    blk_num = objCodec.CalcBlkNum(bit_num);
    Debug("blk_num", blk_num);
    m_source_signal = new int[bit_num];
    m_encoded_signal = new int[blk_num * objCodec.codeword_len];
    m_send_signal = new int[blk_num * objCodec.codeword_len];
    m_receive_signal = new double[blk_num * objCodec.codeword_len];
    m_restore_codeword = new int[blk_num * objCodec.codeword_len];
    m_restore_signal = new int[bit_num];
    err_bit_cnt = err_blk_cnt = 0;

    out_file = OpenFile(output_file_path, "w");

}

void Simulator::Start() {
    for(double snr = m_min_snr; snr <= m_max_snr; snr += m_snr_step) {
        SaveTempInfo("snr", snr);
        SetSeed(-1);
        objChannel.SetNoisePower(Modem::CntSignalPower() / pow(10.0, snr / 10.0));
        while(m_err_blk_cnt < m_target_err_blk) {
            SourceCreate::CreateBitStream(m_source_signal, bit_num);
            objCodec.Encode(m_source_signal, m_encoded_signal, blk_num);
            Modem::Modulate(m_encoded_signal, m_send_signal, blk_num * objCodec.codeword_len);
            objChannel.Transmit(m_send_signal, m_receive_signal, blk_num * objCodec.codeword_len);
            objCodec.Decode4BPSK_AWGN(m_receive_signal, m_restore_codeword, m_restore_signal, blk_num);
            for(int i = 0; i < bit_num; ++i)
                if(m_source_signal[i] != m_restore_signal[i])
                    ++m_err_bit_cnt;
            for(int i = 0; i < blk_num; ++i)
                if(HammingDistance(m_encoded_signal + (i * objCodec.codeword_len), m_restore_codeword + (i * objCodec.codeword_len), enc1.codeword_len))
                    ++err_blk_cnt;
            SaveTempInfo();
        }
        ber = (double)m_err_bit_cnt / m_bit_num;
        fer = (double)m_err_blk_cnt / m_blk_num;
        DisplayResult();
    }
}

void Simulator::Clean() {
    delete[] m_source_signal;
    delete[] m_encoded_signal;
    delete[] m_send_signal;
    delete[] m_receive_signal;
    delete[] m_restore_codeword;
    delete[] m_restore_signal;
    objCodec.Clean();
    objChannel.Clean();
    fclose(out_file);
}

void Simulator::SaveTempInfo() {
    if(m_save_temp_info) {
        FILE *tf = OpenFile(m_temp_file, "a+");
        fprintf(tf, "\ntot_blk_num=%d,err_blk_cnt=%d\n", m_blk_num, m_err_blk_cnt);
        fprintf(tf, "tot_bit_num=%d,err_bit_cnt=%d\n\n", m_bit_num, m_err_bit_cnt);
        fclose(tf);
    }
}

void Simulator::SaveTempInfo(char *info, double val) {
    if(m_save_temp_info) {
        FILE *tf = OpenFile(m_temp_file, "a+");
        fprintf(tf, "\n%s=%d\n\n", info, val);
        fclose(tf);
    }
}

void Simulator::DisplaySourceSignal() {
    fprintf(out_file, "The source signal is:\n");
    fprintf(out_file, "%d", m_source_signal[0]);
    for(int i = 1; i < bit_num; ++i)
        fprintf(out_file, " %d", m_source_signal[i]);
    fprintf(out_file, "\n\n");
}

void Simulator::DisplayEncodedSignal() {
    int idx;
    fprintf(out_file, "The encoded signal is:\n");
    for(int i = 0; i < blk_num; ++i) {
        idx = i * enc1.codeword_len;
        fprintf(out_file, "%d", m_encoded_signal[idx]);
        for(int j = 1; j < enc1.codeword_len; ++j)
            fprintf(out_file, " %d", m_encoded_signal[idx + j]);
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n\n");
}

void Simulator::DisplaySendSignal() {
    int idx;
    fprintf(out_file, "The sent signal is:\n");
    for(int i = 0; i < blk_num; ++i) {
        idx = i * enc1.codeword_len;
        fprintf(out_file, "%d", m_send_signal[idx]);
        for(int j = 1; j < enc1.codeword_len; ++j)
            fprintf(out_file, " %d", m_send_signal[idx + j]);
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n\n");
}

void Simulator::DisplayReceiveSignal() {
    int idx;
    fprintf(out_file, "The received signal is:\n");
    for(int i = 0; i < blk_num; ++i) {
        idx = i * enc1.codeword_len;
        fprintf(out_file, "%f", m_receive_signal[idx]);
        for(int j = 1; j < enc1.codeword_len; ++j)
            fprintf(out_file, " %f", m_receive_signal[idx + j]);
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n\n");
}

void Simulator::DisplayRestoreCodeword() {
    int idx;
    fprintf(out_file, "The restored codeword is:\n");
    for(int i = 0; i < blk_num; ++i) {
        idx = i * enc1.codeword_len;
        fprintf(out_file, "%d", m_restore_codeword[idx]);
        for(int j = 1; j < enc1.codeword_len; ++j)
            fprintf(out_file, " %d", m_restore_codeword[idx + j]);
        fprintf(out_file, "\n");
    }
    fprintf(out_file, "\n\n");
}

void Simulator::DisplayRestoreSignal() {
    fprintf(out_file, "The restored signal is:\n");
    fprintf(out_file, "%d", m_restore_signal[0]);
    for(int i = 1; i < bit_num; ++i)
        fprintf(out_file, " %d", m_restore_signal[i]);
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
