#include <fstream>
#include <iostream>


std::string positions_SECTION_DATA_last(const std::string& asm_stream) {
	size_t section_text_idx = asm_stream.find("SECTION TEXT");
	size_t section_data_idx = asm_stream.find("SECTION DATA");

	// Checar se ambas as seções existem
	/*
	if (section_text_idx == std::string::npos || section_data_idx == std::string::npos)
		return asm_stream; */

	// Determinar qual seção vem primeiro
	size_t first_section_idx  = std::min(section_text_idx, section_data_idx);
	size_t second_section_idx = std::max(section_text_idx, section_data_idx);

	if (first_section_idx == section_text_idx)
		return asm_stream;

	// Separar cada seção
	std::string first_section  = asm_stream.substr(first_section_idx, second_section_idx - first_section_idx);
	std::string second_section = asm_stream.substr(second_section_idx);
	std::string prefix = asm_stream.substr(0, first_section_idx);

	return prefix + second_section + first_section;
}




std::string remove_comments(const std::string& asm_stream) {
	
	std::string asm_stream_without_comments;
	int line_begin_idx = 0;

	while (line_begin_idx < asm_stream.size()) {
		int line_end_idx = asm_stream.find('\n', line_begin_idx);

		if (line_end_idx == std::string::npos)
			line_end_idx = asm_stream.size();

		int comment_idx = asm_stream.find(';', line_begin_idx);
		
		if (comment_idx != std::string::npos && comment_idx < line_end_idx) {
			// concatena apenas a parte pré-comentário
			asm_stream_without_comments += asm_stream.substr(line_begin_idx, comment_idx - line_begin_idx);
			
		} else {
			// concatena toda a linha de instrução
			asm_stream_without_comments += asm_stream.substr(line_begin_idx, line_end_idx - line_begin_idx);
		}

		asm_stream_without_comments += '\n';
		line_begin_idx = line_end_idx + 1;
	}


	/*
	std::string path = "./asm_stream_without_comments.asm";
	std::ofstream file(path);
	if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
    }

	file << asm_stream_without_comments;
	file.close();
	*/

	return asm_stream_without_comments;
}




std::string remove_unnecessary_chars(std::string asm_stream) {

	int i = 0;
	char previous = '\n';


	while (asm_stream[i] != NULL) {

		if (asm_stream[i] == ' ') {
			if (previous == '\n'
			||  previous == ' '
			||  previous == ',')
			{
				asm_stream.erase(asm_stream.begin() + i);
				i--;
			} else if (previous == ':')
				asm_stream[i] = '\n';

		} else if (asm_stream[i] == '\n') {
			if (previous == '\n') {
				asm_stream.erase(asm_stream.begin() + i);
				i--;
			} else if (previous == ' ') {
				// ||  previous == '\t'
				asm_stream.erase(asm_stream.begin() + i - 1);
				i--;
			}

		} else if (asm_stream[i] == '\t') {
			if (previous == '\n'
			||  previous == ' '
			||  previous == ',')
			{
				asm_stream.erase(asm_stream.begin() + i);
				i--;
			} else if (previous == ':') {
				// condição inserida para a formatação de rótulos posicionados no começo das linhas de instrução
				asm_stream[i] = '\n';
			} else {
				asm_stream[i] = ' ';
			}

		} else if (asm_stream[i] == ',' && previous == ' ') {
			// condição inserida por último por ser a menos provável de ocorrer
			asm_stream.erase(asm_stream.begin() + i - 1);
			i--;
		}

		i++;

		i == 0 ? previous = '\n'
			   : previous = asm_stream[i - 1];
	}

	return asm_stream;
}




std::string preprocess_asm_stream(std::string asm_stream) {
	
	asm_stream = remove_comments(asm_stream);
	asm_stream = remove_unnecessary_chars(asm_stream);
	//asm_stream = positions_SECTION_DATA_last(asm_stream);

	//std::cout << "****** ****** ****** ****** ****** ****** ****** ******" << std::endl;
	//std::cout << asm_stream;

	return asm_stream;
}
