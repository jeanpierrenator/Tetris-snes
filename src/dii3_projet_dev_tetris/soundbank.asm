;************************************************
; snesmod soundbank data                        *
; total size:        388 bytes                  *
;************************************************

.include "hdr.asm"

.bank 5
.section "SOUNDBANK" ; need dedicated bank(s)

__SOUNDBANK__:
.incbin "soundbank.bnk"
.ends
