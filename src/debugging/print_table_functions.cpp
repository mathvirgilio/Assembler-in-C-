/*
void prints_MNT() {
    
    char last = {0};
    int  macro_name_length = 0;

    std::cout << "Macro Name Table (Two-pass)"      << std::endl;
    std::cout << "──────┬─────────────┬───────────" << std::endl;
    std::cout << "name  | num de args | corpo"      << std::endl;

    for (auto infos : MNT) {
        std::cout << infos.first << "\t" << infos.second.macro_argument_num << "\t" << "\"";

        macro_name_length = infos.first.length();

        for (int i = 0; i < infos.second.macro_body.length(); i++) {
            if (last == '\n') {
                for (int space = 0; space < macro_name_length; space++)
                    std::cout << ' ';

                std::cout << "\t \t";
            }

            std::cout << infos.second.macro_body[i];
            last = infos.second.macro_body[i];
        }        
    }
}
*/
void prints_tokens_lines_array(std::vector<std::vector<std::string>> tokens_lines_array,  std::string context_msg) {
    std::cout << "\"tokens_lines_array\" (" << context_msg << "): [" << std::endl;
    
	for (int i = 0; i < tokens_lines_array.size(); i++) {
		std::cout << "    [";
	
		for (int j = 0; j < tokens_lines_array[i].size(); j++) {
			(j == tokens_lines_array[i].size() - 1)  ?  std::cout << "\"" << tokens_lines_array[i][j] << "\"]" << std::endl
													 :  std::cout << "\"" << tokens_lines_array[i][j] << "\", ";
		}
	}
	std::cout << "]" << std::endl << std::endl;
}




void prints_ST() { // ordenadas segundo algum critério desconhecido

	std::cout << "Symbol Table (Two-pass)"          << std::endl;
    std::cout << "────────┬─────┬───────┬─────────" << std::endl;
    std::cout << "symbol  │ pos │ extrn │ pos use " << std::endl;

	for (auto infos : ST) {
		
		std::string spaces = {0};
		int spaces_num = 8 - infos.first.length();

		if (spaces_num >= 0) {
			for (int i = 0; i < spaces_num; i++) {spaces += ' ';}
		}

	    std::cout << infos.first << spaces << "│ " << std::setfill('0') << std::setw(2) << infos.second.position << "  │ " ;
        infos.second.is_extern  ?  std::cout << "true  │ "
                                :  std::cout << "false │ ";
        
        size_t len = infos.second.position_use.size();
        if (len > 0) {
            for (int i = 0; i < len; i++) {
                i == len - 1  ?  std::cout << std::setfill('0') << std::setw(2) << infos.second.position_use[i]
                              :  std::cout << std::setfill('0') << std::setw(2) << infos.second.position_use[i] << ' ';
            }
        }
        std::cout << std::endl;
	}

    std::cout << std::endl;
}
