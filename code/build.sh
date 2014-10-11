#!/bin/bash

cc="gcc"

info()
{
	cat << EOF
	CoDExtended
		 by
	riicchhaarrd

	http://github.com/riicchhaarrd/CoDExtended
	http://cod1.eu/
	
	Thanks to kung foo man and his project libcod
	https://github.com/kungfooman/libcod
EOF
}

uANY=false
uMYSQL=false
uECMASCRIPT=false
DEBUG=false
DEFINES=""

while getopts “hdmj” qo
do
	case $qo in
	h)
		info
		exit 1
		;;
	m)
		uMYSQL=true
		uANY=true
		;;
	j)
		uECMASCRIPT=true
		uANY=true
		;;
	d)
		DEBUG=true
		;;
	esac
done

if [ $DEBUG = true ]; then
echo "{ CODEXTENDED DEVELOPMENT BUILD }"
else
echo "{ CODEXTENDED RELEASE BUILD }"
fi

if [ $uANY = true ]; then
echo "Using: "
fi

if [ $uMYSQL = true ]; then
	echo -n "MYSQL, "
fi

if [ $uECMASCRIPT = true ]; then
	echo -n "ECMASCRIPT, "
fi

if [ $uANY = true ]; then
echo ""
fi

#Compiling CoDExtended

compiler="$cc -Os -O1 -O3 -s -fvisibility=hidden -w"

if [ $DEBUG = true ]; then
#compiler="$cc -DxDEBUG -g -w"
compiler="$cc -DxDEBUG -g -Os -O1 -O3 -s -fvisibility=hidden -w"
fi

if [ $uMYSQL = true ]; then
DEFINES+="-DuMYSQL "
fi

if [ $uECMASCRIPT = true ]; then
DEFINES+="-DBUILD_ECMASCRIPT "
fi

params="$DEFINES -std=c99 -I. -m32 -fPIC -Wno-write-strings"
#params="-m32 -fPIC -Wno-write-strings"

mkdir -p bin
mkdir -p obj

info

echo "Compiling..."
echo "[ROOT]"
$compiler $params -c init.c -o obj/init.o
$compiler $params -c librarymodule.c -o obj/librarymodule.o
$compiler $params -c codextended.c -o obj/codextended.o
$compiler $params -c entity.c -o obj/entity.o
echo "[COMMON]"
$compiler $params -c cracking.c -o obj/cracking.o
$compiler $params -c cvar.c -o obj/cvar.o
$compiler $params -c files.c -o obj/files.o
$compiler $params -c cmd.c -o obj/cmd.o
$compiler $params -c msg.c -o obj/msg.o
echo "[GAME]"
$compiler $params -c shared.c -o obj/shared.o
$compiler $params -c script.c -o obj/script.o
$compiler $params -c bg_pmove.c -o obj/bg_pmove.o
echo "[SERVER]"
$compiler $params -c sv_commands.c -o obj/sv_commands.o
$compiler $params -c sv_client.c -o obj/sv_client.o
$compiler $params -c sv_world.c -o obj/sv_world.o
$compiler $params -c sv_game.c -o obj/sv_game.o
$compiler $params -c sv_init.c -o obj/sv_init.o
$compiler $params -c sv_main.c -o obj/sv_main.o
$compiler $params -c x_clientcmds.c -o obj/x_clientcmds.o
echo "[SCRIPT]"
$compiler $params -c scr_method_player.c -o obj/scr_method_player.o
$compiler $params -c scr_string.c -o obj/scr_string.o
$compiler $params -c scr_fields.c -o obj/scr_fields.o
$compiler $params -c scr_method_entity.c -o obj/scr_method_entity.o

if [ $uMYSQL = true ]; then
$compiler $params -c scr_mysql.c -o obj/scr_mysql.o
fi

if [ $uECMASCRIPT = true ]; then
echo "[ECMACRIPT]"
$compiler $params -c js.c -o obj/js.o

if [ ! -f obj/duktape.o ]; then
$compiler $params -c duktape/duktape.c -o obj/duktape.o
fi

fi

obj="$(ls obj/*.o)"
if [ $uMYSQL = true ]; then
$compiler -m32 -shared -L/lib32 -L/home/lib -lmysqlclient -L/usr/lib/mysql -I/usr/include/mysql -o ./bin/codextended.so $obj -Os -s -ldl -lm -Wall
else
$compiler -m32 -shared -L/lib32 -L./lib -L/home/rawcod -o ./bin/codextended.so $obj -Os -s -ldl -lm -Wall
fi
#rm -rf ./obj
find /home/ext/obj -name '*.o' ! -name 'duktape.o' -delete
echo "Done."
