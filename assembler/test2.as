MAIN: add r3, LIST
macr noam_macr   
 cmp r3, #-6
 bne END
endmacr 
 lea STR, r6
 inc r6
 mov *r6,K
macr my_macro
 dec K
 jmp LOOP
endmacr 
 sub r1, r4
 my_macro
 my_macro
 noam_macr
 dec K
 jmp LOOP
END: stop
STR: .string "abcd"
LIST: .data 6, -9
 .data -100
K: .data 31

