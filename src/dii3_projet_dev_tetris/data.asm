.include "hdr.asm"

.section ".rodata1" superfree

snesfont:
.incbin "pvsneslibfont.pic"


mapsprite:
.incbin "sprite8.map"
mapsprite_end:

gfxpsrite:
.incbin "sprite8.pic"
gfxpsrite_end:

palsprite:
.incbin "sprite8.pal"
palsprite_end:

m0: .incbin "bg0.map"
m0_end:

t0: .incbin "bg0.pic"
t0_end:

p0: .incbin "bg0.pal"
p0_end:

linebrr:
.incbin "line.brr"
linebrrend:

placebrr:
.incbin "place.brr"
placebrrend:


.ends

