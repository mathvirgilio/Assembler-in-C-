#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <iomanip> // incluso apenas para debbuging

//#include "resolve_macro.cpp"
#include "hash_tables.cpp"
#include "debugging/print_table_functions.cpp"


bool HAS_END_DIRECTIVE = false;


std::vector<std::vector<std::string>> first_pass(std::vector<std::vector<std::string>> tokens_lines_array) {
	
	std::string token;
	std::string last_label;

	unsigned int position_counter = 0;
	unsigned int position_addition;
	unsigned int tokens_line_length;
	std::vector<std::string> new_line;


	for (int i = 0; i < tokens_lines_array.size(); i++) {
		//std::cout << "linha atual: " << std::setfill('0') << std::setw(2) << i << " | \"" << tokens_array[0] << "\"" << std::endl;
		tokens_line_length = tokens_lines_array[i][0].length();

		if (tokens_lines_array[i][0][tokens_line_length - 1] == ':') {
			// linha somente com a definição de um símbolo
			token = tokens_lines_array[i][0].substr(0, tokens_line_length - 1);
			last_label = token;

			if (ST.count(token) == 1) {
				if (ST[token].is_public == false) {
					std::cout << "ERROR: redefinição do símbolo \"" << token << "\"" << std::endl;
				} else {
					ST[token].position = position_counter;
					tokens_lines_array.erase(tokens_lines_array.begin() + i);
					i--;
				}

			} else {
				struct use_table aux;
				aux.position = position_counter;
				ST[token] = aux;
				//ST[token] = position_counter;
				tokens_lines_array.erase(tokens_lines_array.begin() + i);
				i--;
				//std::cout << "└──────── última linha inserida na ST: " << std::endl;
				//std::cout << "                                       " << token << " | " << position_counter << std::endl;
			}

		} else {
			token = tokens_lines_array[i][0];

			if (IT.count(token) == 1) {
				position_counter += IT[token].length;
				//std::cout << "└──────────── tamanho da instrução: " << IT[token].length << std::endl;
				//std::cout << "└──────────── contador de posição : " << position_counter << std::endl;
			} else if (DT.count(token) == 1) {
				position_addition = DT[token](tokens_lines_array[i]);
				position_counter += position_addition;
				//std::cout << "└──────────── tamanho da diretiva : " << DT[token] << std::endl;
				//std::cout << "└──────────── contador de posição : " << position_counter << std::endl;

				if (tokens_lines_array[i][0] == "CONST" or tokens_lines_array[i][0] == "const") {
					new_line.push_back(tokens_lines_array[i][1]); // assume-se q CONST foi usado corretamente
					tokens_lines_array.insert(tokens_lines_array.begin() + i, new_line);
					tokens_lines_array.erase( tokens_lines_array.begin() + i + 1);
					new_line.clear();

				} else if (tokens_lines_array[i][0] == "SPACE" or tokens_lines_array[i][0] == "space") {
					for (int space = 0; space < position_addition; space++) {
						new_line.push_back("0");
					}
					tokens_lines_array.insert(tokens_lines_array.begin() + i, new_line);
					tokens_lines_array.erase( tokens_lines_array.begin() + i + 1);
					new_line.clear();

				} else if (tokens_lines_array[i][0] == "SECTION" or tokens_lines_array[i][0] == "section") {
					tokens_lines_array.erase(tokens_lines_array.begin() + i);
					i--;

				} else if (tokens_lines_array[i][0] == "BEGIN" or tokens_lines_array[i][0] == "begin") {
					HAS_BEGIN_DIRECTIVE = true;
					tokens_lines_array.erase(tokens_lines_array.begin() + i);
					i--;

				} else if (tokens_lines_array[i][0] == "END" or tokens_lines_array[i][0] == "end") {
					HAS_END_DIRECTIVE = true;

					tokens_lines_array.erase(tokens_lines_array.begin() + i);
					i--;

				} else if (tokens_lines_array[i][0] == "EXTERN" or tokens_lines_array[i][0] == "extern") {
					ST[last_label].is_extern = true;

					tokens_lines_array.erase(tokens_lines_array.begin() + i);
					i--;

				} else if (tokens_lines_array[i][0] == "PUBLIC" or tokens_lines_array[i][0] == "public") {
					struct use_table aux;
					ST[tokens_lines_array[i][1]] = aux;
					ST[tokens_lines_array[i][1]].is_public = true;

					tokens_lines_array.erase(tokens_lines_array.begin() + i);
					i--;
				}

			} else {
				std::cout << "ERROR: operação \"" << token << "\" não identificada" << std::endl;
				position_counter += 1;
			}
		}
	}


	if (HAS_BEGIN_DIRECTIVE and !HAS_END_DIRECTIVE)
		std::cout << "ERROR: BEGIN usado sem que END fosse usado" << std::endl;

	return tokens_lines_array;
}




std::vector<std::vector<std::string>> second_pass(std::vector<std::vector<std::string>> tokens_lines_array) {

	std::string token;
	std::string metadata_token;
	std::string linker_metadata;
	std::vector<std::string> metadata_tokens_array;
	std::vector<std::string> R_bit_map;
	R_bit_map.push_back("R,");

	std::vector<std::vector<std::string>> linker_metadata_array;

	unsigned int position_counter = 0;
	unsigned short length;


	for (int i = 0; i < tokens_lines_array.size(); i++) {
		token = tokens_lines_array[i][0];

		if (IT.count(token) == 1) {
			tokens_lines_array[i][0] = IT[token].opcode;
			length = IT[token].length;
			position_counter++;
			R_bit_map.push_back("0");

			if (tokens_lines_array[i].size() != length)
				std::cout << "ERROR: número de argumentos inválidos. O número de argumentos aceitos pela operação \"" << token << "\", é " << length - 1 << std::endl;

			for (int j = 1; j < length; j++) {
				token = tokens_lines_array[i][j];
				R_bit_map.push_back("1");

				if (ST.count(token) == 1) {
					if (ST[token].is_public) {
						metadata_token  = "D,";
						metadata_token += token;
						metadata_tokens_array.push_back(metadata_token);
						//std::cout << "metadata_tokens_array[0]" << metadata_tokens_array[0] << std::endl;

						metadata_token = std::to_string(ST[token].position);
						metadata_tokens_array.push_back(metadata_token);
						//std::cout << "metadata_tokens_array[1]" << metadata_tokens_array[1] << std::endl;

						linker_metadata_array.push_back(metadata_tokens_array);
						metadata_tokens_array.clear();

						tokens_lines_array[i][j] = std::to_string(ST[token].position);
					} else if (ST[token].is_extern) {
						ST[token].position_use.push_back(position_counter);
						//std::cout << "para DEPURAR - valor de 'tokens_lines_array[i][j]': " << tokens_lines_array[i][j] << std::endl;
						tokens_lines_array[i][j] = "0";
					} else {
						tokens_lines_array[i][j] = std::to_string(ST[token].position);
					}

					if (ST[token].is_public and ST[token].is_extern)
						std::cout << "ERROR: símbolo \"" << token << "\" definido tanto como PUBLIC quanto como EXTERN" << std::endl;
				}
				position_counter++;
			}

		} else {
			for (int pos = 0; pos < tokens_lines_array[i].size(); pos++) {
				R_bit_map.push_back("0");
			}
			position_counter += tokens_lines_array[i].size();
		}
	}

	//std::cout << "R_bit_map: [" << std::endl;
	//for (int i = 0; i < R_bit_map.size(); i++) {
	//	std::cout << "[\"" << R_bit_map[i] << "\"] ";
	//}
	//std::cout << std::endl << "]" << std::endl;

	std::vector<std::string> obj_code_array;

	for (int i = 0; i < tokens_lines_array.size(); i++) {
		for (int j = 0; j < tokens_lines_array[i].size(); j++) {
			obj_code_array.push_back(tokens_lines_array[i][j]);
		}
	}

	//for (int i = 0; i < obj_code_array.size(); i++) {
	//	std::cout << "linha:    \"" << obj_code_array[i] << "\"" << std::endl;
	//}


	for (auto infos : ST) {
		if (infos.second.is_extern) {
			metadata_token  = "U,";
			metadata_token += infos.first;
			metadata_tokens_array.push_back(metadata_token);
			//std::cout << "metadata_tokens_array[0]" << metadata_tokens_array[0] << std::endl;

			for (int pos : infos.second.position_use) {
				metadata_tokens_array.push_back(std::to_string(pos));
				//std::cout << "metadata_tokens_array[sla]" << metadata_tokens_array[0] << std::endl;
			}

			linker_metadata_array.push_back(metadata_tokens_array);
			metadata_tokens_array.clear();

			//metadata_token = std::to_string(ST[token].position);
		}
	}

	linker_metadata_array.insert(linker_metadata_array.begin(), R_bit_map);
	tokens_lines_array.clear();
	tokens_lines_array.push_back(obj_code_array);

	for (std::vector<std::string> array: linker_metadata_array) {
		tokens_lines_array.insert(tokens_lines_array.begin(),  array);
	}
	
	return tokens_lines_array;
}




std::string assemble(std::string asm_stream) {

	//asm_stream = resolve_macro(asm_stream);
	//asm_stream = implement_macro(asm_stream);

	std::vector<std::vector<std::string>> tokens_lines_array;
	std::vector<std::string> tokens_array;
	std::string token;
	std::string asm_line;
	std::string return_string = {0};

	for (int i = 0; i < asm_stream.length(); i++) {
		asm_line += asm_stream[i];

		if (asm_stream[i] == '\n') {
		
			for (int c = 0; c < asm_line.length(); c++) {
				if (asm_line[c] == ' '
				||  asm_line[c] == ','
				||  asm_line[c] == '\n') {
					tokens_array.push_back(token);
					token.clear();
				} else {
					//token += asm_line[c];
					token += std::toupper(static_cast<unsigned char>(asm_line[c]));
				}
			}
			tokens_lines_array.push_back(tokens_array);
			tokens_array.clear();
			asm_line.clear();
		}
	}

	
	//prints_tokens_lines_array(tokens_lines_array, "apenas pré-formatado");

	tokens_lines_array = first_pass(tokens_lines_array);
	//prints_tokens_lines_array(tokens_lines_array, "depois da PRIMEIRA passagem");
	//prints_ST();

	tokens_lines_array = second_pass(tokens_lines_array);
	prints_tokens_lines_array(tokens_lines_array, "depois da SEGUNDA passagem");
	//prints_ST();

	for (int i = 0; i < tokens_lines_array.size(); i++) {
		for (int j = 0; j < tokens_lines_array[i].size(); j++) {
			return_string += tokens_lines_array[i][j];
			
			(j == tokens_lines_array[i].size() - 1)  ?  return_string += '\n'
													 :  return_string += ' ';
		}
	}

	return return_string;
}
