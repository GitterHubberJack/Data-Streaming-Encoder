#include "StEncoder.h"
#include <sstream>

class TBGenerator {
    public:
        TBGenerator(StEncoder* stEncoder, size_t dutClkPeriod, size_t rstTime);
        ~TBGenerator();
        //
        void generateHeader(std::stringstream& o);
        void generateInterfaces();
        void instantiateDUT(std::stringstream& o);
        void generateClkAndRst(std::stringstream& o);
        void configUVM(std::stringstream& o);
        void generateMakefile();
        void generateUvmDriver();
        void generateTBPkg();
        void generateSequence();
        void generate();


    private:
    std::string _dutName;
    std::string _outDir = "../tb/";
    size_t      _dutClkPeriod = 50;
    size_t      _offset = 3;
    size_t      _rstTime = 1000;
    size_t      _simTime= 20;
    StEncoder*  _stEncoder;
};
