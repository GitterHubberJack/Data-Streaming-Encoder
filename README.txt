/************************************************************************************
    OVERVIEW
*************************************************************************************/
A "data streaming Encoder" generator dedicated to parse the messages spec and generate a SystemVerilog encoder which
outputs a serialized packet stream from a subset of the input fields.
Optimize the encoder to minimize the latency in the packet serialization.
Validate your encoder for correctness.
Validate your encoder generator for correctness.
Use a validation framework of your choosing (cocotb, uvm, scala, etc).


A "data streaming Encoder" generator dedicated to parse the messages spec and generate a SystemVerilog encoder which outputs a serialized packet stream from a subset of the input fields.
It is capable of encoding any type of message that adhere to the spec mentioned under example/*.yaml file
With the example included (example_spec.yaml), which contains 2 type of messsages, about 5 fields each, design is timing clean in standalone mode at 500 MHz
EDA P&R tool: Quartus
Version: 22.1
Reports are included under the directory <quartus>


/************************************************************************************
    HOW TO RUN
*************************************************************************************/

1) Generate the project
   - cd scripts
   - make
<st_encoder.sv> file should be generated under src directory
tesbench environment should be generated under tb

2) Test the st_encoder
    - cd tb
    - make
    - ./main <file_name>                //  Example: ./main example_spec.yaml

 Once the task is done, <wave.vcd> should be generated
 If you wish to convert it to .wlf, run: make view_vcd



