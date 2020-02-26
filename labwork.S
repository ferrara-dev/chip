  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #


.align 2
.global __mytime
	__mytime:
	.word 0x5957

.global timstr
	timstr:
	.ascii "text more text lots of text\0"


.text
tickz:	lw	$t0,0($a0)	# get time
	addiu	$t0,$t0,1	# increase
	andi	$t1,$t0,0xf	# check lowest digit
	sltiu	$t2,$t1,0xa	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x6	# adjust lowest digit
	andi	$t1,$t0,0xf0	# check next digit
	sltiu	$t2,$t1,0x60	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa0	# adjust digit
	andi	$t1,$t0,0xf00	# check minute digit
	sltiu	$t2,$t1,0xa00	# if digit < a, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0x600	# adjust digit
	andi	$t1,$t0,0xf000	# check last digit
	sltiu	$t2,$t1,0x6000	# if digit < 6, okay
	bnez	$t2,tiend
	nop
	addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
	jr	$ra		# return
	nop

.global hexasc
hexasc:
        andi    $a0,$a0,0xf     #only 4 least significant bits ignore(clear) other bits
        addi    $v0,$zero,0x30  #$v0 = 0x30 ('0' ASCII character)
        addi    $t0,$zero,0x9   #t0 = 0x9

        ble     $a0,$t0,L1      #branch to L1 if a0 <= t0
        nop
        addi    $v0,$v0,0x7     #v0 = v0 +0x7 (thier is 7 other characters from 0x39 to 0x41) thats why
        #0x7 is added to the v0 to ignore the gap and jump directly to 'A' in ASCII table

L1:
        add     $v0,$a0,$v0     #v0 = V0 +a0
        jr      $ra
        nop
.global delay
delay:
 		li    $t1,4500000 #(intitaly was t1=4711) ### 20000 iterationer genom den inre loopen <-> 1 ms (13 sekunder i programmet klockades till 13,1)
        PUSH    $s0
        PUSH    $ra
        addi    $s0, $0, 0   #i = 0
        move    $t0,$a0     #move the argument to t0
while:
        blez      $t0,done    #branch to done if ms > 0
        nop
        addi    $t0,$t0,-1      #decrement ms by 1
for:
        beq     $s0,$t1,done2
        nop
        addi    $s0,$s0,1
        j       for
        nop
done2:

        j       while
        nop
done:
        POP     $ra
        POP     $s0
        jr      $ra
        nop




.global time2string
time2string:
        PUSH     $16 # Adress pekare
        PUSH     $17 # Den befintliga tiden
        PUSH     $18 # Kontroll variabel för att bryta loopen
        PUSH	 $19 # variabel som används för att få ut önskad byte från $s1
        PUSH	 $20  # Kontroll variabel för att lägga till kolon vid rätt position i strängen
	PUSH 	 $21
        PUSH     $ra

        add     $s0,$0,$a0      # spara adressen till "timstr" från $a0 till $s0
        add     $s1,$0,$a1      # information om den befintliga tiden kopieras från $a1 till $s1
		li $s3,0xf000
		li $t7, 12
		addi $s4, $s0,2  # Om s0 pekar på index 2 i strängen --> appendColon
		addi $s2, $s0,5  # Om s0 pekar på index 5 i strängen --> endloop

		timeLoop:
		beq $s0, $s2,endloop  # Om s0 pekar på index 5 i strängen --> endloop
		nop

		and $a0,$s1,$s3			  # ta fram relevant nibble från present time
		srav $a0,$a0,$t7          # shifta relevant nibble till LSB


		jal hexasc
		nop

		sb $v0, 0($s0)
		srl $s3,$s3,4			   #
		addi $t7,$t7,-4			   #

		addi $s0,$s0,1             # Flytta fram adress pekaren till nästa byte
		beq $s4,$s0, appendColon   # Om s0 pekar på index 2 i strängen --> appendColon
		nop



		#beq  $s0,$s2,check
		#nop

		j timeLoop
		nop


		#check:
		#beq $a0,2,appendTWO
		#nop

		j timeLoop
		nop

		appendColon:
		li      $t3,0x3A        #t3 = kolon i ASCII tabel
		sb      $t3,0($s0)
		addi $s0,$s0,1
		j timeLoop
		nop


		#appendTWO:
		#li      $t3,0x54        #t3 = kolon i ASCII tabel
		#sb      $t3,-1($s0)
		#li      $t3,0x57
		#sb      $t3,0($s0)
		#li		$t3, 0x4f
		#addi $s0,$s0,1
		#sb 		$t3,0($s0)
		#addi $s0,$s0,1

		endloop:
        li      $t4,0x00        #lägg till null i slutet av strängen
   		sb      $t4, 0($s0)

        POP     $ra
	POP	$21
        POP 	$20
        POP 	$19
  	POP     $18
        POP     $17
        POP     $16

        jr       $ra
        nop
