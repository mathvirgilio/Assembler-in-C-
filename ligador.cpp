#include "src/headers.hpp"

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <sstream>
#include <unordered_map>

#define  MAX_FILE_LINE_LEN 99
#define  ERROR_MSG__INVALID_FILE_EXT  "arquivo inválido: espera-se arquivos de extensão \".asm\" ou \".pre\""

int convert_string_to_int(std::string str) {
    std::stringstream ss(str);
    int number;
    ss >> number;
    return number;
}

void writeToFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);

    if (!file) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    file << content;

}

std::vector<std::vector<std::string>> transform_to_tokens_lines_array(std::string asm_stream) {
    std::vector<std::vector<std::string>> tokens_lines_array;
	std::vector<std::string> tokens_array;
	std::string token;
	std::string asm_line;

	for (int i = 0; i < asm_stream.length(); i++) {

		asm_line += asm_stream[i];

		if (asm_stream[i] == '\n' or i == asm_stream.length() - 1) {
		
			for (int c = 0; c < asm_line.length(); c++) {
				if (asm_line[c] == ' '
				||  asm_line[c] == ','
				||  asm_line[c] == '\n') {
					tokens_array.push_back(token);
					token.clear();
				} else {
					token += asm_line[c];
				}
			}
			tokens_lines_array.push_back(tokens_array);
			tokens_array.clear();
			asm_line.clear();
		}
	}

    tokens_array.push_back(token);
    
    std::cout << "\"Arquivo objeto em array (size = " << tokens_lines_array.size() << ")\": [" << std::endl;
	for (int i = 0; i < tokens_lines_array.size(); i++) {
		std::cout << "    [";
	
		for (int j = 0; j < tokens_lines_array[i].size(); j++) {
			(j == tokens_lines_array[i].size() - 1)  ?  std::cout << "\"" << tokens_lines_array[i][j] << "\"]" << std::endl
													 :  std::cout << "\"" << tokens_lines_array[i][j] << "\", ";
		}
	}
	std::cout << "]" << std::endl;
    return tokens_lines_array;
}

std::vector<std::string> linking(std::vector<std::vector<std::vector<std::string>>> all_objects) {
    std::vector<std::string> final_object;
    std::unordered_map<std::string, int> locals_symbol_table;
    std::unordered_map<std::string, std::vector<int>> pendencies;
    std::vector<int> tokens_to_ajust;
    std::vector<int> PC_at_time;
    long unsigned int PC = 0;
    int aaa;

    for (std::vector<std::vector<std::string>> object : all_objects) {
        for (std::vector<std::string> line : object) {
            if (line[0] == "D") {
                locals_symbol_table[line[1]] = convert_string_to_int(line[2]) + PC;
            } else if (line[0] == "U") {
                std::vector<int> all_uses;
                for (int i = 2; i < line.size(); i++) {
                    aaa = convert_string_to_int(line[i]); // + PC;
                    //std::cout << "aqui " << aaa << std::endl;
                    all_uses.push_back(aaa);
                }
                pendencies[line[1]] = all_uses;
            } else if (line[0] == "R") {
                for (int j = 1; j < line.size(); j++) {
                    if (line[j] == "1") {
                        tokens_to_ajust.push_back(j);
                        PC_at_time.push_back(PC);
                    }
                }
            } else {
                final_object.insert(final_object.end(), line.begin(), line.end());
                PC += line.size();
            }
        }
    }

    for (const auto& local_label : locals_symbol_table) {
        std::cout << "Label: " << local_label.first << ", Endereço: " << local_label.second << std::endl;       
    }

    for (const auto& extern_label : pendencies) {
        int adress = locals_symbol_table[extern_label.first];
        //std::cout << extern_label.first << " endereço" << adress << " posicoes: ";
        for (int i : extern_label.second) {
            final_object[i] = std::to_string(adress);
            //std::cout << i << std::endl;
        }        
    }

    for (int i = 0; i < tokens_to_ajust.size(); i++) {
        int aux = convert_string_to_int(final_object[tokens_to_ajust[i]]) + PC_at_time[i];
        final_object[i] = std::to_string(aux);
    }

    return final_object;
}

int main(int argc, char *argv[]) {
	if (argc < 2) { // confere se o comando foi executado de maneira correta:
		std::cout << "espera-se mais arquivos <filename>.obj\"" << std::endl;
		return -1;
	}

    std::vector<std::vector<std::vector<std::string>>> all_objects;
    for (int i = 1; i <= argc - 1; i++) {
        FILE *file_ptr = fopen(argv[i], "rt");

        if (file_ptr == NULL) {
            std::cout << "erro durante a abertura de \"" << argv[1] << "\"" << std::endl;
            return -1;
        }

        char file_line[MAX_FILE_LINE_LEN + 1];
        size_t len = strlen(argv[1]);
        std::string file_stream;
        std::string file_extension = std::string(argv[1] + len - 4, 4);

        if (len <= 4) {
            std::cout << ERROR_MSG__INVALID_FILE_EXT << std::endl;
            return -1;
        }	

        // extrai os dados do arquivo e armazena em objeto 'std::string'
        while (fgets(file_line, 100, file_ptr) != NULL) {
            file_stream += file_line;
        }

        fclose(file_ptr);
        //std::cout << file_stream << std::endl;

        if (file_extension == ".obj") {
            std::vector<std::vector<std::string>> tokens_lines_array = transform_to_tokens_lines_array(file_stream);
            all_objects.push_back(tokens_lines_array);
        } else {
            std::cout << ERROR_MSG__INVALID_FILE_EXT << std::endl;
            return -1;
        }
    }
    std::vector<std::string> final_string = linking(all_objects);
    std::string output;
    for (std::string element : final_string) {
        output += element;
        output += " ";
    }
    output += "\n";

    writeToFile("asm/exs/objeto_final.e", output);

	return 0;
}
