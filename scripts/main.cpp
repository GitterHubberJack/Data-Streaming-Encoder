#include "TBGenerator.h"
#include <iostream>
#include <string>

int main(int argc , char *argv[]) {
    if (argc != 2 ) {
        std::cout << "Wrong arguments. Please specify the name of the full file name \n Example: ./main /dir/spec.yaml " << std::endl;
        return 1;
    } else {
        std::string fileName = argv[1];
        StructGenerator* structGen = new StructGenerator (fileName);
        structGen->generate();
        //
        StEncoder* stEncoder = new StEncoder("clk", "rst_n", structGen);
        stEncoder->generate();
        //
        size_t dutClkPeriod = 50;
        size_t rstTime = 1000;
        TBGenerator* tbGen = new TBGenerator(stEncoder, dutClkPeriod, rstTime);
        tbGen->generate();
            
        return 0;
    }
}



