MAIN: add r3   , LIST
LOOP  : prn #48


    ;testdelete

;testdelete

macr noammax
 cmp r3, #-6
 bne END
endmacr 
 lea STR, r6
 inc r6
 mov *r6,K
 sub r1, r4
 noammax

macr testmacro2
inc r7
inc r1
dec K
endmacr
 dec K
 jmp LOOP
 testmacro2
END: stop
.entry END
     
         

STR: .string "1234fgfssff-03i"
LIST: .data 6, -9
 .data -100 , 55
K: .data 31

