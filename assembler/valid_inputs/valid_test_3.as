START: add LabelA, r6
inc r7
sub #50 ,r7
macr NewMacro
mov *r2 , START
cmp #-25 , #25
clr K
endmacr
K: .data 3,4,5,6
lea LABELB , r5
dec LABELB
NewMacro
LABELB: .string "hellolabel"
red EXlabel
not r0
jmp *r5
LabelA: jsr *r4 
rts
stop
.entry LABELB
.extern EXlabel
;Ignore from duplicate declaration of array or entry
.extern EXlabel