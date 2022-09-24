#include "StructGenerator.h"

#include <string>
#include <vector>
#include <sstream>  

struct Triplet {
    std::string msgType;
    std::string field;
    std::string value;
};

struct CycleInfo {
    size_t avail_cycle;
    size_t delay_cycle;
    size_t delay_cycle_width;
    size_t overlap;
};


class StEncoder {
    public:
        StEncoder(std::string clk, std::string rst, StructGenerator* structGen);
        ~StEncoder();
        void generateHeader(std::stringstream& o);
        void generateInterface(std::stringstream& o);
        void generateCore(std::stringstream& o);
        void createLocalVars(std::stringstream& o);
        void generateFSM(std::stringstream& o);
        void generateFieldFSM(std::stringstream& o, size_t state, std::string& serial_data, size_t numOfStates, size_t counter_width,
                std::string& msgType, std::string field, size_t len, size_t offset, size_t readyIdx);

        void addPipeline(std::stringstream& o, size_t syncStages, size_t width, std::string inSig,
                std::string outSig0, std::string outSig1, std::string instName);

        void addPiSoInstance(std::stringstream& o, size_t P_WIDTH, size_t SYNC_STAGES, std::string load,
                std::string paralle_in, std::string serial_out, std::string instName);

        void generatePipelines(std::stringstream& o);
        void generate(); 
        //
        StructGenerator* getStructGen() {return _structGen;}
        size_t getMaxCounterSize() {return _maxCounterSize;}


    private:
        size_t                          _frameSize = 32;
        std::string                     _moduleName;
        std::string                     _outDir = "../src/";
        std::vector <std::string>       _includes;
        //std::vector <Triplet>           _localParams;
        //std::vector <Triplet>           _localBusses;
        std::map<size_t, CycleInfo>      _delayCycles;
        std::map<std::string, size_t>   _readyDelay;
        std::map<std::string, size_t>   _expandedBus;
        size_t                          _maxNumOfStates = 0;
        size_t                          _maxCounterSize = 0;
        std::string                     _clk;
        std::string                     _rst;
        StructGenerator*                _structGen;
};

