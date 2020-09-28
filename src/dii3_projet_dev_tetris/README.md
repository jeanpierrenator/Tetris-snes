# DII3_projet_dev_tetris
## Build
Il faut au préalable installer `pvsneslib` : https://github.com/alekmaul/pvsneslib/wiki/Installation-with-Windows.

Pour générer la rom, exécuter la commande `make` dans le dossier du projet.

Il faut s'assurer de configurer `DEVKITSNES` et `DEVKIT65XX` correctement dans le `Makefile`.

## Controls
| Joypad Button | Action                                     |
|---------------|--------------------------------------------|
| Up            | Place the tetrimino directly in the column |
| Down          | Speed up the tetrimino                     |
| Left          | Move the tetrimino to left                 |
| Right         | Move the tetrimino to right                |
| Button A      | Rotate the tetrimino                       |
| Button Start  | Pause game                                 |