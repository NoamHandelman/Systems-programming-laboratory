MAIN: add r3   , LIST

LOOP  : prn #48


    ;testdelete

;testdelete

macr END
 cmp r3, #-6
 bne END
endmacr 
 lea STR, r6
 inc r6
 mov *r6,K
 sub r1, r4
 END

inc r7
inc r1
dec K

 dec K
 jmp LOOP
END: stop
.entry END
     
         

STR: .string "1234fgfssff-03i"
LIST: .data 6, -9
 .data -100 , 55
K: .data 31

