# Call of Duty Extended

CoDExtended is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

====

Call of Duty Extended is a modification for Call of Duty 1, which does what it says it extends CoD with new features and it fixes known bugs.

[ *nix setup ]

Binary release:
	http://cod1.eu/codextended.so

{Building the binary}

Install these packages if you don't have them yet.

apt-get install build-essential
apt-get install libssl-dev
apt-get install libcurl4-openssl-dev

[If you're on x86_64 then you would add :i386 after the library packages ]

Run build.sh	

Your startup commandline for your Call of Duty dedicated server would look something like this.
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. LD_PRELOAD=/path/to/codextended.so ./cod_lnxded

Features:

- GSC > C/C++
- Extending the functions/methods of the default GSC script
- New memberfields additions
- Various callbacks e.g ClientCommand
- Bugfixes e.g (q3dirtrav)
- Much more unlisted features
- Collision (If the collision is turned off then you can stand ontop of other people. ;])
- Velocity altering
- Instead of the self.angles > self getplayerangles() which returns better angles
- Play player-animation(s) with a script
- Entity collision and damage feedback

For any questions feel free to ask me on xfire: phpcod or steam: riicchhaarrd

Thanks to kung foo man (http://killtube.org) and his libcod project (CoD 2 memory modification) (https://github.com/kungfooman/libcod)
