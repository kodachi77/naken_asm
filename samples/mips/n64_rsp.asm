.n64_rsp

li $v1, 234
add $v1, $v0, $t0
lsv $v1[14], 16($1)
lsv $v1[12], 16($1)
lhv $v1[0], 256($1)
slv $v1[0], 128($1)
mtc2 $t5, $v4[4]
mfc2 $t5, $v31[4]
vnop
vmov $v3[15], $v5[2]
;vmrg $v1, $v5, $v1[1q]
;vadd $v1, $v5, $v0

