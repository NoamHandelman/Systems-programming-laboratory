macr macroOne
    add r1, *r2
    cmp r3, #-15
    bne funcCall
endmacr

macr macroTwo
    sub *r4, r5
    jsr funcTwo
    inc r1
endmacr

macr macroThree
    mov r2, *r3
    prn r4
    clr *r5
endmacr

macr bigMacro
    mov r1, *r3
    add r2, r3
    sub r4, *r5
    cmp r6, r7
    bne funcOne
    jsr funcTwo
    prn r7
endmacr

.extern funcOne

startLabel: mov r1, *r2
            prn stringOne
            lea labelOne, r3

.entry loopLabel

            sub r3, dataOne
            jsr funcOne

.extern externalLabel1

            macroOne
            prn r2
            clr r6
            mov r7, *r4
            sub stringTwo, labelTwo
            add r1, *r3
            mov dataOne, r5
            jsr funcThree
            stop

stringOne:  .string "Initial string test."
dataOne:    .data 1, -1, 3767, -3767

.extern funcTwo

funcCall:   lea stringTwo, *r6
            inc *r6

.entry funcCall

            mov *r6, dataOne
            sub r1, *r4
            cmp r3, #-6
            bne loopLabel
            add r7, *r6

.extern externalLabel2

            clr kVar
            sub labelThree, *r2
            jmp finalStop

stringTwo:  .string "Another test string mixed with instr."
kVar:       .data 10, 20, -30, 40, 50

loopLabel:  prn #48
            lea stringThree, *r5
            macroTwo
            inc r1
            cmp r2, #-20
            bne funcOne
            jsr funcTwo
            dec r3
            jmp loopLabel

.extern funcThree

funcOneLocal: clr r2
              lea stringOne, r3
              add r4, r3
              jsr funcThree
              prn *r1
              macroOne

.entry funcOneLocal

labelFive:   .data 100, 200, 300

              mov r6, labelFive
              stop

.extern externalLabel3

funcTwoLocal: clr r5
              mov r1, r6
              add r2, *r3
              sub r4, r5
              jsr funcOneLocal
              macroTwo

.extern externalLabel4

              mov r7, r6

.entry funcTwoLocal

              prn labelFour
              stop

labelOne:     .data 256, -512, 1024, -2048
              prn stringThree

stringThree:  .string "String inside the data section."

              mov r2, *r3
              prn r4

funcThreeLocal: jsr funcOneLocal
                prn *r6
                clr r2
                mov r3, labelSix
                sub r1, r2
                cmp r7, #-30
                bne funcTwoLocal
                macroThree
                stop

labelSix:     .data 400, 500, 600  

bigMacro

.extern externalLabel5

              mov r2, labelTwo
              cmp r3, r4
              add r5, r6
              sub r7, r1
              jsr funcOneLocal
              dec r2
              prn *r3
              mov labelOne, r5
              jsr funcThreeLocal
              jmp loopLabel
              prn r6

labelTwo:     .data 5500, -5600, 5700, -5800, 5900
              macroThree

.extern externalLabel6

mov r2, stringFour

stringFour:   .string "Another interleaved string test."

              prn labelTwo
              add r3, *r4

labelFour:    .data 800, -900, 1000, -1100, 1200, -1300
              mov r2, *r5
              sub r7, r6
              prn r6

.extern externalLabel7

              macroOne
              mov r5, labelFour
              add r2, r7
              prn stringFive
              sub r6, *r3
              jmp funcOneLocal

labelThree:   .data 1400, 1500, 1600
              prn *r1
              mov r2, r3
              clr r5

.extern funcFour

              mov labelFive, r4
              prn stringThree
              sub r6, r7
              stop

stringFive:   .string "Final string at the end."

.entry finalStop

.extern finalLabel

finalStop:    stop
              prn r7