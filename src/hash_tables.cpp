#include <unordered_map>
#include <string>
#include <vector>


bool HAS_BEGIN_DIRECTIVE = false;


// define tipo de dado: ponteiro para função que retorna o valor de 'position_counter' para uma linha com uma diretiva
typedef unsigned short (*directive_function_ptr)(std::vector<std::string>);

unsigned short process_CONST(std::vector<std::string> tokens_array) {
    if (tokens_array.size() > 2) {
        std::cout << "ERROR: na linha \"";

        for (int i = 0; i < tokens_array.size(); i++) {
            std::cout << tokens_array[i];

            if (i == 0)
                std::cout << ' ';

            if (0 < i and i < tokens_array.size() - 1)
                std::cout << ',';
        }
        std::cout << "\"" << std::endl;
        std::cout << "       diretiva \"CONST\" deve aceitar, no máximo, um único argumento" << std::endl;
    } else {
        try {
            int constant = std::stoi(tokens_array[1]);
        } catch (const std::invalid_argument& e) {
            //std::cerr
            std::cout << "ERROR: na linha \""  << tokens_array[0] << ' ' << tokens_array[1] << "\""       << std::endl;
            std::cout << "       argumento \"" << tokens_array[1] << "\" não representa um número válido" << std::endl;
        }
    }

    return 1;
}




unsigned short process_SPACE(std::vector<std::string> tokens_array) {
    if (tokens_array.size() == 1)
        return 1;

    if (tokens_array.size() > 2) {
        std::cout << "ERROR: na linha \"";

        for (int i = 0; i < tokens_array.size(); i++) {
            std::cout << tokens_array[i];

            if (i == 0)
                std::cout << ' ';

            if (0 < i and i < tokens_array.size() - 1)
                std::cout << ',';
        }

        std::cout << "\"" << std::endl;
        std::cout << "       diretiva \"SPACE\" deve aceitar, no máximo, um único argumento" << std::endl;
        return 1;
    }

    try {
        int space_number = std::stoi(tokens_array[1]);
        return space_number; // valor que deve ser somado ao contador de posição
    } catch (const std::invalid_argument& e) {
        //std::cerr
        std::cout << "ERROR: na linha \""  << tokens_array[0] << ' ' << tokens_array[1] << "\""       << std::endl;
        std::cout << "       argumento \"" << tokens_array[1] << "\" não representa um número válido" << std::endl;
        return 1;
    }
}




inline unsigned short process_SECTION(std::vector<std::string> tokens_array) {
    return 0;
}


inline unsigned short process_BEGIN(std::vector<std::string> tokens_array) {
    if (tokens_array.size() > 1)
        std::cout << "ERROR: " << tokens_array[0] << " não deve receber nenhum argumento" << std::endl;

    return 0;
}


inline unsigned short process_END(std::vector<std::string> tokens_array) {
    if (tokens_array.size() > 1)
        std::cout << "ERROR: " << tokens_array[0] << " não deve receber nenhum argumento" << std::endl;

    if (!HAS_BEGIN_DIRECTIVE)
		std::cout << "ERROR: END usado sem que BEGIN fosse usado" << std::endl;

    return 0;
}


inline unsigned short process_EXTERN(std::vector<std::string> tokens_array) {
    return 0;
}


unsigned short process_PUBLIC(std::vector<std::string> tokens_array) {
     if (tokens_array.size() != 2)
        std::cout << "ERROR: " << tokens_array[0] << " deve receber um único argumento" << std::endl;

    return 0;
}




struct use_table {
    unsigned int position;
    bool is_extern = false;
    bool is_public = false;
    std::vector<int> position_use;
};

struct macros_infos {
	long unsigned int macro_argument_num;
	std::string macro_body; // substitui o uso da MDT (Macro Definition Table)
};

struct instruction_infos {
    std::string opcode;
    unsigned short length; // em words de 16 bits
};



std::unordered_map<std::string, struct use_table> ST;                // Symbol Table
std::unordered_map<std::string, struct macros_infos> MNT;        // Macro Name Table
std::unordered_map<std::string, directive_function_ptr> DT = {   // Directives Table
    {"CONST"  , &process_CONST},   {"const"  , &process_CONST},
    {"SPACE"  , &process_SPACE},   {"space"  , &process_SPACE},
    {"SECTION", &process_SECTION}, {"section", &process_SECTION},
    {"BEGIN"  , &process_BEGIN},   {"begin"  , &process_BEGIN},
    {"END"    , &process_END},     {"end"    , &process_END},
    {"EXTERN" , &process_EXTERN},  {"extern" , &process_EXTERN},
    {"PUBLIC" , &process_PUBLIC},  {"public" , &process_PUBLIC},
};

std::unordered_map<std::string, struct instruction_infos> IT = { // Instruction Table
    //                         opcode, length
    {"ADD"    , instruction_infos {"01", 2}},  {"add"    , instruction_infos {"01", 2}},
    {"SUB"    , instruction_infos {"02", 2}},  {"sub"    , instruction_infos {"02", 2}},
    {"MUL"    , instruction_infos {"03", 2}},  {"mul"    , instruction_infos {"03", 2}},
    {"DIV"    , instruction_infos {"04", 2}},  {"div"    , instruction_infos {"04", 2}},
    {"JMP"    , instruction_infos {"05", 2}},  {"jmp"    , instruction_infos {"05", 2}},
    {"JMPN"   , instruction_infos {"06", 2}},  {"jmpn"   , instruction_infos {"06", 2}},
    {"JMPP"   , instruction_infos {"07", 2}},  {"jmpp"   , instruction_infos {"07", 2}},
    {"JMPZ"   , instruction_infos {"08", 2}},  {"jmpz"   , instruction_infos {"08", 2}},
    {"COPY"   , instruction_infos {"09", 3}},  {"copy"   , instruction_infos {"09", 3}},
    {"LOAD"   , instruction_infos {"10", 2}},  {"load"   , instruction_infos {"10", 2}},
    {"STORE"  , instruction_infos {"11", 2}},  {"store"  , instruction_infos {"11", 2}},
    {"INPUT"  , instruction_infos {"12", 2}},  {"input"  , instruction_infos {"12", 2}},
    {"OUTPUT" , instruction_infos {"13", 2}},  {"output" , instruction_infos {"13", 2}},
    {"STOP"   , instruction_infos {"14", 1}},  {"stop"   , instruction_infos {"14", 1}},
};
