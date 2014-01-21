Call of Duty Extended
====

Call of Duty Extended is a modification for Call of Duty 1, which does what it says it extends CoD with new features and it fixes known bugs.

[ Standalone ]

1. Copy CoDExtended.exe into your Call of Duty folder
2. Run your Call of Duty server
3. Run CoDExtended.exe (After 5 seconds it'll automatically close)
4. Load any map e.g /map mp_carentan
5. The tool should be now working and you can use it's possibilities.

[ Linux ] (Unstable) (Less features than Windows version) (Only Patch 1.5 so far)

1. Compile the .so

gcc -I. -m32 -fPIC -c CoDExtended.cpp -o CoDExtended.opp

gcc -m32 -shared -L/lib32 -o CoDExtended.so  CoDExtended.opp -Os -s -ldl -Wall

2. Add this to your starting line (Library path may be not needed)

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. LD_PRELOAD=CoDExtended.so ./cod_lnxded

3. Run the server like you normally would

:: Features

- GSC > C/C++
- Extending the functions/methods of the default GSC script
- Various callbacks e.g ClientCommand
- Much more unlisted features

:: Player Features
- Collision (If the collision is turned off then you can stand ontop of other people. ;])
- Velocity altering
- Instead of the self.angles > self getplayerangles() which returns better angles
- Play player-animation(s) with a script

:: Entity features
- Entity collision and damage feedback


For any questions feel free to ask me on xfire: phpcod

Thanks to kung foo man (http://killtube.org) and his libcod project (CoD 2/4 linux memory modification) (https://github.com/kungfooman/libcod)
