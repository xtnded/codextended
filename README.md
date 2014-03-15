Call of Duty Extended
====

Call of Duty Extended is a modification for Call of Duty 1, which does what it says it extends CoD with new features and it fixes known bugs.

[ Windows Standalone ]

1. Copy CoDExtended.exe into your Call of Duty folder
2. Run your Call of Duty server
3. Run CoDExtended.exe (After 5 seconds it'll automatically close)
4. Load any map e.g /map mp_carentan
5. The tool should be now working and you can use it's possibilities.

[ Linux ]

1. Compile the .so
2. Copy over the compiled library "codextended.so" to your Call of Duty folder where cod_lnxded is located.

2. Add this to your starting line (Library path may be not needed)

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. LD_PRELOAD=codextended.so ./cod_lnxded

So your starting up script/line looks like this;

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. LD_PRELOAD=codextended.so ./cod_lnxded +sets gamestartup \"`date +"%m-%d-%y %T "`\" +set ttycon 1 +set net_port 28960 +set sv_punkbuster 0

3. Run the server like you normally would

:: Features

- GSC > C/C++
- Extending the functions/methods of the default GSC script
- Various callbacks e.g ClientCommand
- Bugfixes e.g (q3dirtrav)
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
