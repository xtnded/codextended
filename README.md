# Call of Duty Extended [Server]

Call of Duty Extended Server is a modification of the Call of Duty 1 Linux server. It supports patches 1.1 and 1.5 (1.5 support is limited).

## Features

- q3msgboom fix
- q3dirtrav fix
- setting players contents
- cracked server, means everyone can join your server
- patched some unpure things
GSC Commands called on players:
- getip()
- getStance()
- renamebot()
- kickbot()
- setmovespeedscale()
- setmaxspeed()
- -ispure()
- isbot()
- dropclient()
 AND MORE
## Usage

Your startup commandline for your Call of Duty dedicated server would look something like this.<br>
`LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. LD_PRELOAD=/path/to/codextended.so ./cod_lnxded`

## Dependencies / libraries

`$ sudo apt-get install build-essential`<br>
`$ sudo apt-get install nasm`<br>
`$ sudo apt-get install libz-dev:i386`<br>
`$ sudo apt-get install libssl-dev:i386`<br>

* If you're on x86_64 then you would add :i386 after the library package names

## Building

`$ git clone https://github.com/riicchhaarrd/codextended`<br>
`$ cd codextended/src`<br>
`$ ./build.sh`<br>

You should now have a `codextended.so` file in your `../bin` folder.

* You can compile the debug version with `./build.sh -d`.

## MySQL / MariaDB

If you want to compile with MySQL support you have to install the `libmysqlclient` or `libmariadbclient` libraries.
Example for Ubuntu:

`$ sudo apt install libmariadbclient-dev:i386`

## Optional

If you want to use these SteamSDK or Duktape, then you have to download the dependencies and place them in their respective directories.<br>
To compile with these options you can use arguments passed to the build script.

## Questions

Forum: http://xtnded.org (php)<br>
Steam: riicchhaarrd (php)<br>
Discord: Defected#0001 (dftd)<br>

## License

CoDExtended is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.


Thanks to [kungfooman](http://killtube.org) and his [libcod](https://github.com/kungfooman/libcod) project (CoD 2 memory modification)
