SECTION TEXT
SWAP: MACRO &A, &B, &T
      COPY  &A, &T
      COPY  &B, &A
      COPY  &T, &B
      ENDMACRO

; assume-se que é possível passar rótulos para uma macro
BEQ:  MACRO &op1, &op2, &LABEL ; branch to &LABEL if &op1 is equal to &op2
      LOAD &op1                ; ACC =  &op1
      SUB  &op2                ; ACC -= &op2
      JMPZ &LABEL              ; se &op1 = &op2 => ACC = 0 => jump
      ENDMACRO

BGEZ: MACRO &op1, &LABEL       ; branch to &LABEL if &op1 is greater than or equal to 0
      LOAD 0                   ; ACC =  0
      SUB  &op1                ; ACC -= &op1
      JMPZ &LABEL              ; se &op1 = 0 => ACC = 0 => jump
      JMPN &LABEL              ; se &op1 > 0 => ACC < 0 => jump
      ENDMACRO

      INPUT N1
      INPUT N2
      BEQ   N1, N2, END
      BGEZ  N1, END
      BGEZ  N2, END
        
      ; se N1 for diferente de N2 e ambos forem menores que 0, o valor das variáveis é trocado
      SWAP   N1, N2, TEMP
      OUTPUT N1
      OUTPUT N2

END:  STOP

SECTION DATA
        N1:   SPACE
        N2:   SPACE
        TEMP: SPACE

