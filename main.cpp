#include "CommonFunc.h"
#include "Simulator.h"

int main() {
    Simulator s;
    s.Init("Simulator.ini");
    s.Start();
    //s.DisplayResult();
    s.Clean();
    return 0;
}
