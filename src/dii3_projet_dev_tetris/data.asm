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

patterns:
.incbin "pvsneslib.pic"
patterns_end:

patternslogo:
.incbin "logo_polytech_1.pic"
patternslogo_end:

map:
.incbin "pvsneslib.map"
map_end:

palette:
.incbin "pvsneslib.pal"
palette_end:


maplogo:
.incbin "logo_polytech_1.map"
maplogo_end:

palettelogo:
.incbin "logo_polytech_1.pal"
palettelogo_end:

.ends

