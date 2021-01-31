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

.section ".rodata2" superfree

linebrr:
.incbin "line.brr"
linebrrend:

placebrr:
.incbin "place.brr"
placebrrend:

menubg:
.incbin "bmpmenudown.pic"
menubg_end:

menubgMap:
.incbin "bmpmenudown.map"
menubgMap_end:

menubgPalette:
.incbin "bmpmenudown.pal"
menubgPalette_end:

menubg2:
.incbin "bmpmenuup.pic"
menubg2_end:

menubg2Map:
.incbin "bmpmenuup.map"
menubg2Map_end:

menubg2Palette:
.incbin "bmpmenuup.pal"
menubg2Palette_end:

menubg3:
.incbin "bmpmenucenter.pic"
menubg3_end:

menubg3Map:
.incbin "bmpmenucenter.map"
menubg3Map_end:

menubg3Palette:
.incbin "bmpmenucenter.pal"
menubg3Palette_end:

menubg4:
.incbin "bmpmenutitle.pic"
menubg4_end:

menubg4Map:
.incbin "bmpmenutitle.map"
menubg4Map_end:

menubg4Palette:
.incbin "bmpmenutitle.pal"
menubg4Palette_end:

.ends