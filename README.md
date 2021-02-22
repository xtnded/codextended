# Call of Duty Extended

CoDExtended is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

====

Call of Duty Extended is a modification for Call of Duty 1, which does what it says it extends CoD with new features, increasing limits and fixing bugs.

# Usage

Your startup commandline for your Call of Duty dedicated server would look something like this.
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:. LD_PRELOAD=/path/to/codextended.so ./cod_lnxded

# Building

Install GCC and libs if you haven't yet with
apt-get install build-essential

[ If you're on x86_64 then you would add :i386 after the library package names ]

Then download and place the dependencies listed under here in the respective directories.

SteamSDK, Duktape

# Questions

If you have any question post a new thread or PM me on http://xtnded.org
or add me on steam.

Steam: riicchhaarrd

====

Thanks to kung foo man (http://killtube.org) and his libcod project (CoD 2 memory modification) (https://github.com/kungfooman/libcod)