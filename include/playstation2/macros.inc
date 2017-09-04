;;
;; Playstation 2 Graphics Sythesizer Macros
;;
;; A part of the naken_asm assembler 
;;
;; For more information:
;; http://www.mikekohn.net/
;;

;; Primitive types
PRIM_POINT equ 0
PRIM_LINE equ 1
PRIM_LINE_STRIP equ 2
PRIM_TRIANGLE equ 3
PRIM_TRIANGLE_STRIP equ 4
PRIM_TRIANGLE_FAN equ 5
PRIM_SPRITE equ 6

;; Macros to help with GIF data formatting
.define SETREG_RGBAQ(r,g,b,a,q) (q<<32)|(a<<24)|(b<<16)|(g<<8)|r

.define SETREG_XYZ2(x,y,z) (z<<32)|(y<<16)|x

.define SETREG_PRIM(prim,shading,texture_mapping,fogging,alpha_blend,anti_alias,texture_coords,context,fix) \
  (((fix) << 10) | \
   ((context) << 9) | \
   ((texture_coords) << 8) | \
   ((anti_alias) << 7) | \
   ((alpha_blend) << 6) | \
   ((fogging) << 5) | \
   ((texture_mapping) << 4) | \
   ((shading) << 3) | \
    (prim))

.define SETREG_MODECONT(ip,tme,fge,abe,aa1,fst,ctxt,fix) \
  (((fix) << 10) | \
   ((ctxt) << 9) | \
   ((fst) << 8) | \
   ((aa1) << 7) | \
   ((abe) << 6) | \
   ((fge) << 5) | \
   ((tme) << 4) | \
   ((iip) << 3))

.define SETREG_PRMODECONT(ac) (ac)

.define SETREG_PRMODECOUNT

.define GIF_TAG(nloop,eop,pre,prim,flg,nreg,regs) \
  (((regs) << 64) | \
   ((nreg) << 60) | \
   ((flg) << 58) | \
   ((prim) << 47) | \
   ((pre) << 46) | \
   ((eop) << 15) | \
    (nloop))

FLG_PACKED equ 0
FLG_REGIST equ 1
FLG_IMAGE equ 2

.define SETREG_TRXPOS(src_x,src_y,dst_x,dst_y,dir) \
  (((dir) << 59) | \
   ((dst_y) << 48) | \
   ((dst_x) << 32) | \
   ((src_y) << 16) | \
    (src_x))

DIR_UL_LR equ 0
DIR_LL_UR equ 1
DIR_UR_LL equ 2
DIR_LR_UL equ 3

.define SETREG_TRXREG(width,height) (((height) << 32) | (width))

.define SETREG_TRXDIR(dir) (dir)

XDIR_HOST_TO_LOCAL equ 0
XDIR_LOCAL_TO_HOST equ 1
XDIR_LOCAL_TO_LOCAL equ 2
XDIR_DEACTIVATED equ 3

.define SETREG_BITBLTBUF(src_base,src_width,src_format,dst_base,dst_width,dst_format) \
  (((dst_format) << 56) | \
   ((dst_width) << 48) | \
   ((dst_base) << 32) | \
   ((src_format) << 24) | \
   ((src_width) << 16) | \
    (src_base))

FMT_PSMCT32 equ 0x00
FMT_PSMCT24 equ 0x01
FMT_PSMCT16 equ 0x02
FMT_PCMCT16S equ 0x0a
FMT_PCGPU24 equ 0x12     ; for DISPFB
FMT_PSMT8 equ 0x13
FMT_PSMT4 equ 0x14
FMT_PSMT8H equ 0x1b
FMT_PSMT4HL equ 0x24
FMT_PSMT4HH equ 0x2c
FMT_PSMZ32 equ 0x30
FMT_PSMZ24 equ 0x31
FMT_PSMZ16 equ 0x32
FMT_PSMZ16S equ 0x3a

.define SETREG_DISPLAY(dx,dy,magh,magv,dw,dh) \
  (((dh) << 44) | \
   ((dw) << 32) | \
   ((magv) << 27) | \
   ((magh) << 23) | \
   ((dy) << 12) | \
    (dx))

.define SETREG_DISPFB(fpb,fbw,psm,dbx,dby) \
  (((dby) << 43) | \
   ((dbx) << 32) | \
   ((psm) << 15) | \
   ((fbw) << 9) | \
    (fpb))

.define SETREG_TEX0(tbp0,tbw,psm,tw,th,tcc,tfx,cbp,cpsm,csm,csa,cld) \
  (((cld) << 61) | \
   ((csa) << 56) | \
   ((csm) << 55) | \
   ((cpsm) << 51) | \
   ((cbp) << 37) | \
   ((tfx) << 35) | \
   ((tcc) << 34) | \
   ((th) << 30) | \
   ((tw) << 26) | \
   ((psm) << 20)| \
   ((tbw) << 14) | \
    (tbp0))

.define TEX_MODULATE 0
.define TEX_DECAL 1
.define TEX_HIGHLIGHT 2
.define TEX_HIGHLIGHT2 3

.define SETREG_TEX1(lcm,mxl,mmag,mmin,mtba,l,k) \
  (((k) << 32) | \
   ((l) << 19) | \
   ((mtba) << 9) | \
   ((mmin) << 6) | \
   ((mmag) << 5) | \
   ((mxl) << 2) | \
   (lcm))

.define FILTER_NEAREST 0
.define FILTER_LINEAR 1
.define FILTER_NEAREST_MIPMAP_NEAREST 2
.define FILTER_NEAREST_MIPMAP_LINEAR 3
.define FILTER_LINEAR_MIPMAP_NEAREST 4
.define FILTER_LINEAR_MIPMAP_LINEAR 5

.define SETREG_TEX2(psm,cbp,cpsm,csm,csa,cld) \
  (((cld) << 61) | \
   ((csa) << 56) | \
   ((csm) << 55) | \
   ((cpsm) << 51) | \
   ((cbp) << 37) | \
   ((psm) << 20))

.define SETREG_TEXA(ta0,aem,ta1) (((ta1) << 32) | ((aem) << 15) | (ta0))
