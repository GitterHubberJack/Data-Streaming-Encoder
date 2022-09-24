//
//@brief: class used to generated a verilog stucture based on input "spec.yaml" file
//
#include <string>
//#include <unordered_map>
#include <map>
#include <vector>
#include <set>

class StructGenerator {
    public:
        StructGenerator(std::string& fileName);
        ~StructGenerator();
        void parseSpecFile();
        void generateStruct();
        void generate();
        std::map<std::string, std::vector<std::string> > getFieldsMap() {
            return _fieldsMap;
        }
        std::map<std::string, std::pair<size_t, size_t> > getValuesMap() {
            return _valuesMap;
        }
        size_t getMaxSize() { return _maxSize;}
        size_t getMaxNumFields() { return _maxNumFields;}
        size_t getMaxMsgType() { return _maxMsgType;}
        typedef std::map <std::string, std::map <size_t, std::map <std::string, size_t> > > groups;
        groups getGroups() { return _groups;}

    private:
        std::string                                                             _fileName;
        std::string                                                             _outFileName;
        std::string                                                             _outDir = "../src/";
        size_t                                                                  _maxSize;
        size_t                                                                  _maxNumFields;
        size_t                                                                  _maxMsgType = 0;
        std::vector< std::pair <std::string, size_t> >                          _supportedMsgTypes;
        std::map<std::string, std::vector<std::string> >                        _fieldsMap;
        std::map<std::string, std::pair<size_t, size_t> >                       _valuesMap;
        //
        std::map <std::string, std::set <std::pair <std::string, size_t> > >    _sizeMap;
        std::map <std::string, std::set <std::pair <std::string, size_t> > >    _cycleMap;
        groups                                                                  _groups;


};
