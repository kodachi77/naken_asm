.epiphany

main:
  add r5,r6,r7
  ;add r5,r6,#0x44
  ;add r5,r6,#0x4411
  bne blah 
  beq main
blah:

  ldr r5,[r6,#3]
  str r5,[r6,#3]
  ldrb r5,[r7,#-3]
  ldrd r5,[r7,#8]


