# 2k19
Code robots 2k18-2k19

## Installation

Après git clone, faire :

```
git submodule init
git submodule update
```

pour télécharger les modules spécifiés dans .gitmodules

## Arduino IDE

Specifier le chemin vers le dossier libraries du projet dans les parametres du logiciel.

## Raspberry

Pour compiler le projet:

```
make
```

Pour programmer la carte:

```
avrdude -p PARTNO -b 115200 -c arduino -P /dev/ttyACM* -v -D -U flash:w:BINARY.hex:i
```

Adapter PARTNO en fonction de la carte:
	* Arduino UNO: PARTNO=m328p

Remplacer BINARY.hex par le nom du binaire a uploader


Pour communiquer sur le port série

```
tty=/dev/ttyACM*
stty -F $tty cs8 9600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
```
