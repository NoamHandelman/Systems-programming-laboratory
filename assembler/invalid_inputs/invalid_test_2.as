macr goodmacro
.data 5 , 6, 7, 8,9
prn #100
endmacr

.entry UndefinedLabel1
.extern fn1

.string abcd"

jmp fakeoperand
goodmacro
LABEL: mov #123456, r7
lea LABEL ,
cmp *r3, *r4
clr BADLABEL
inc #90
sub r6, #-90000
add r1 , r2 ,
red
dec NOTEXSITS
.string .data
jsr *r0
not *r10
stop r4
rts

