#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "hash_tables.cpp"
#include "debugging/print_table_functions.cpp"


using namespace std;


void replaceAll(std::string& A, const std::string& x, const std::string& y) {
    size_t pos = 0;
    while ((pos = A.find(x, pos)) != std::string::npos) {
        A.replace(pos, x.length(), y);
        pos += y.length(); // Avançar posição para continuar a busca
    }
}

void macro_substitution(std::string& asm_stream, const std::string& label, int label_begin_index) {
    size_t parameters_begin_index = label.size() + 1;
    std::string token_name;
    std::vector<std::string> token_vector;

    //Procura parâmetros na primeira linha
    size_t i = parameters_begin_index + label_begin_index;
    while (true) {
        if (asm_stream[i] == ',') {
            token_vector.push_back(token_name);
            token_name.clear();
        } else {
            token_name += asm_stream[i];
        }

        i++;
        if (asm_stream[i] == '\n') {
            token_vector.push_back(token_name);
            break;
        } 
    }
    i++;
    std::string char_count = "#1";
    std::string macro_body_with_new_parameters = MNT[label].macro_body; 
    for (int j = 0; j < MNT[label].macro_argument_num; j++) {
        replaceAll(macro_body_with_new_parameters, char_count, token_vector[j]);
        char_count[1]++;
    }

    asm_stream.replace(label_begin_index, i - label_begin_index, macro_body_with_new_parameters);
    
}

std::string implement_macro(const std::string& asm_stream) {
    std::string asm_stream_without_macro = asm_stream;

    //Passa pelo loop duas vezes caso haja macros que se chamem
    for (int i = 0; i < 2; i++){
        for (const auto& pair : MNT) {
            std::string label = pair.first;
            size_t macro_call_index = asm_stream_without_macro.find(label, 0);

            while (macro_call_index != std::string::npos)
            {
                macro_substitution(asm_stream_without_macro, label, macro_call_index);
                macro_call_index = asm_stream_without_macro.find(label, 0);
            }
            
        }
    }

    //Codigo para checar se ainda há labels. Se sim, dar erro
    for (const auto& pair : MNT) {
        std::string label = pair.first;
        size_t macro_call_index = asm_stream_without_macro.find(label, 0);

        if (macro_call_index != std::string::npos) {
            std::cout << "Erro: loop infinito de macros se chamando" << std::endl;
            return asm_stream_without_macro;
        }
    }

    return asm_stream_without_macro;
}

void analyze_macro(std::string& macro_body, const std::string& macro_name) {
    int i = 6;
    std::string token_name;
    std::vector<std::string> token_vector;
    

    while (true) {
        if (macro_body[i] == ',') {
            token_vector.push_back(token_name);
            token_name.clear();
        } else {
            token_name += macro_body[i];
        }

        i++;
        if (macro_body[i] == '\n') {
            token_vector.push_back(token_name);
            break;
        } 
    }
    //i = tamanho da linha
    i++;
    
    std::string char_count = "#1";

    for (std::string s : token_vector) {
        replaceAll(macro_body, s, char_count);
        char_count[1]++;
    }

    //Remoção da primeira linha
    size_t pos = macro_body.find('\n');        

    MNT[macro_name] = macros_infos {token_vector.size(), macro_body.substr(pos + 1)};
    prints_MNT();
}

std::string resolve_macro(const std::string& asm_stream) {
    std::string asm_stream_without_macro = asm_stream;
    size_t line_begin = 0;

    size_t macro_begin = asm_stream.find("MACRO ", line_begin);

    while (macro_begin != std::string::npos) {
        char label_char = asm_stream_without_macro[macro_begin - 2];
        string label_name;
        if (label_char != ':'){
            std::cout << "Caracter: \"" << label_char << "\"" << std::endl;
            std::cout << "Não foi encontrado rótulo antes da diretiva MACRO" << std::endl;
            return asm_stream_without_macro;
        }

        int i = macro_begin - 3;
        label_char = asm_stream_without_macro[i];
        while (label_char != '\n') {
            label_name = label_char + label_name;
            label_char = asm_stream_without_macro[--i];
        }

        size_t end_macro = asm_stream_without_macro.find("ENDMACRO", line_begin);
        
        if (end_macro == std::string::npos) {
            std::cout << "Não foi encontrado a diretiva ENDMACRO após um MACRO" << std::endl;
            return asm_stream_without_macro;
        }

        std::string substring_to_save = asm_stream_without_macro.substr(macro_begin, end_macro - macro_begin);
        end_macro += 8;

        analyze_macro(substring_to_save, label_name);

        asm_stream_without_macro.erase(i, end_macro - i);
        macro_begin = asm_stream_without_macro.find("MACRO ", line_begin);

    }

    return asm_stream_without_macro;
}

/*
int main() {
    ifstream arquivoEntrada("/home/matheus/Dropbox/PC/Documents/C_C++/codigo.txt");
    if (!arquivoEntrada.is_open()) {
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    string codigo, linha;
    while (getline(arquivoEntrada, linha)) {
        codigo += linha + '\n';
    }
    arquivoEntrada.close();

    string codigoSemComentarios = resolve_macro(codigo);

    ofstream arquivoSaida("/home/matheus/Dropbox/PC/Documents/C_C++/codigo_sem_comentarios.txt");
    if (!arquivoSaida.is_open()) {
        cerr << "Erro ao criar o arquivo de saída!" << endl;
        return 1;
    }

    arquivoSaida << codigoSemComentarios;
    arquivoSaida.close();

    cout << "Comentários removidos com sucesso! Arquivo gerado: codigo_sem_comentarios.txt" << endl;
    return 0;
}
*/