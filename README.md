MDLL
====

Call of Duty 1.5 Memory Modification

[ Standalone ]

1. Copy Tools/bin into Call of Duty folder (Or just copy MDLL.exe into Tools/bin, which is a directory if you installed the CoD MODTools)
2. Run your Call of Duty server
3. Run MDLL.exe (After 5 seconds it'll automatically close)
4. Load any map e.g /map mp_carentan
5. The tool should be now working and you can use it's possibilities.

[ DLL Project ]

- Run the standalone, which creates the DLL which you can inject. (The steps are basically the same)


Or you can create a DLL Project
DLL Project; Compiled with GCC, IDE: Code::Blocks 12.11


[ Linux ] (Unstable) (Less features than Windows version)

1. Compile the .so

gcc -I. -m32 -fPIC -c mdll.cpp -o mdll.opp

gcc -m32 -shared -L/lib32 -o mdll.so  mdll.opp -Os -s -ldl -Wall

2. Add this to your starting line (Library path may be not needed)

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. LD_PRELOAD=mdll.so ./cod_lnxded

3. Run the server like you normally would


:: Features
- GSC Scripting to C++
- ClientCommand receiving
- Player Collision (Use setContents(0) on a player-entity)
  If Player Collision is on and the "bounce" off function isn't patched then you can stand ontop of other people!
- Velocity
- Entity collision and damage feedback
- Player angles, extended button usage and more
- Much more unlisted features


For any questions feel free to ask me on xfire: phpcod

Thanks to kung foo man (http://killtube.org) and his libcod project (CoD 2/4 linux memory modification) (https://github.com/kungfooman/libcod)
