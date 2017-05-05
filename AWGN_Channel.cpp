#include "AWGN_Channel.h"

Channel::Channel() {

}

Channel::~Channel() {

}

void Channel::Init(const char *config_file_path) {
    FILE *in_file = OpenFile(config_file_path, "r");
    char str[50] = "";
    while(~fscanf(in_file, "%[a-z]=", str)) {
        if(!strcmp(str, "segma"))
            fscanf(in_file, "%lf", &segma);
        str[0] = '\0';
        fscanf(in_file, "%*c");
    }
    fclose(in_file);
}

void Channel::transmit(int *in, double *out, int len) {
    //long int seed = time(NULL);
    for(int i = 0; i < len; ++i)
        out[i] = in[i] + Normal(0.0, segma);
}

void Channel::Clean() {

}

void Channel::SetNoisePower(double noise_power) {
    segma = sqrt(noise_power);
}

double Channel::CntNoisePower() {
    return segma * segma;
}
