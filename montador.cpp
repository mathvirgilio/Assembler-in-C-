#include "src/headers.hpp"

#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>


#define  MAX_FILE_LINE_LEN 99
#define  ERROR_MSG__INVALID_FILE_EXT  "arquivo inválido: espera-se arquivos de extensão \".asm\" ou \".pre\""


int main(int argc, char *argv[]) {

	if (argc != 2) { // confere se o comando foi executado de maneira correta:
		std::cout << "espera-se: \"" << argv[0] << " <filename>.asm\"" << std::endl;
		return -1;
	}

	FILE *file_ptr = fopen(argv[1], "rt");

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


	file_stream = preprocess_asm_stream(file_stream);

	if (file_extension == ".asm") {
		std::string output_file_path = std::string(argv[1], len - 4);
		output_file_path += ".pre";

		FILE *pre_file_ptr = fopen(output_file_path.c_str(), "wt");

		if (pre_file_ptr == NULL) {
			std::cout << "erro durante a criação de \"" << output_file_path << "\"" << std::endl;
			return -1;
		}

		fprintf(pre_file_ptr, "%s", file_stream.c_str());
		fclose(pre_file_ptr);

		std::cout << "arquivo pré-processado criado em: \"" << output_file_path << "\"" << std::endl;

	} else if (file_extension == ".pre") {
		file_stream = assemble(file_stream);

		if (file_stream[0] == '\0') {
			file_stream = file_stream.substr(1, file_stream.length() - 1);
			//std::cout << "o primeiro caractere do arquivo .obj é nulo" << std::endl;
		}
		
		//std::cout << "file_stream :" << std::endl;
		//std::cout <<  file_stream << std::endl;

		std::string output_file_path = std::string(argv[1], len - 4);
		output_file_path += ".obj";
		std::ofstream file(output_file_path);

		if (!file) {
			//std::cerr
			std::cout << "Erro ao abrir o arquivo: " << output_file_path << std::endl;
			return -1;
		}

		file << file_stream;
		std::cout << "arquivo objeto criado em: \"" << output_file_path << "\"" << std::endl;

	} else {
		std::cout << ERROR_MSG__INVALID_FILE_EXT << std::endl;
		return -1;
	}


	return 0;
}
