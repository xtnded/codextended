#!/bin/bash

cc="gcc"
LINK_CURL="$(/usr/bin/curl-config --static-libs --cflags)";

info()
{
	cat << EOF
	CoDExtended
		Linux
		 by
	riicchhaarrd

	http://github.com/riicchhaarrd/CoDExtended
	http://cod1.eu/
EOF
}

PATCHVERSION=1
uANY=false
uMYSQL=false
uECMASCRIPT=false
DEBUG=false
uPOWERED=false
DEFINES=""

while getopts “hdmjp15” qo
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
	p)
		uPOWERED=true
		uANY=true
		;;
	5)
		PATCHVERSION=5
		;;
	1)
		PATCHVERSION=1
		;;
	esac
done

if [ $PATCHVERSION = 5 ]; then
echo "Call of Duty Patch 1.5"
else
echo "Call of Duty Patch 1.1"
fi

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

if [ $uPOWERED = true ]; then
	echo -n "BRANDED VERSION, "
else
	echo -n "UNBRANDED VERSION, "
fi

if [ $uANY = true ]; then
echo ""
fi

#Compiling CoDExtended

compiler="$cc -Os -O1 -O3 -s -fvisibility=hidden -w -Wl,--exclude-libs,ALL"

if [ $DEBUG = true ]; then
compiler="$cc -g -w -Wl,--exclude-libs,ALL"
#compiler="$cc -DxDEBUG -DDEBUG -Os -O1 -O3 -s -fvisibility=hidden -w -Wl,--exclude-libs,ALL"
fi

if [ $uMYSQL = true ]; then
DEFINES+="-DuMYSQL "
fi

if [ $uPOWERED = true ]; then
DEFINES+="-DxPOWERED "
fi

if [ $uECMASCRIPT = true ]; then
DEFINES+="-DBUILD_ECMASCRIPT "
fi

if [ $PATCHVERSION = 5 ]; then
DEFINES+="-DCODPATCH=5 "
else
DEFINES+="-DCODPATCH=1 "
fi

params="$DEFINES -std=c99 -I. -m32 -fPIC -Wno-write-strings"
#params="-m32 -fPIC -Wno-write-strings"

mkdir -p bin
mkdir -p obj

echo -e "\nCOMPILING"

#echo "[CPPP]"
#for f in cppp/*.c; do
#	filename=$(basename "$f")
#	extension="${filename##*.}"
#	filename="${filename%.*}"
#	$compiler $params -c cppp/$filename.c -o obj/$filename.o
#done


echo "[ROOT]"
$compiler $params -c init.c -o obj/init.o
$compiler $params -c librarymodule.c -o obj/librarymodule.o
$compiler $params -c codextended.c -o obj/codextended.o
echo "[COMMON]"
$compiler $params -c cvar.c -o obj/cvar.o
$compiler $params -c files.c -o obj/files.o
$compiler $params -c cmd.c -o obj/cmd.o
$compiler $params -c msg.c -o obj/msg.o
$compiler $params -c curl.c -o obj/curl.o
$compiler $params -c util.c -o obj/util.o
echo "[GAME]"
$compiler $params -c shared.c -o obj/shared.o
$compiler $params -c script.c -o obj/script.o
$compiler $params -c bg_pmove.c -o obj/bg_pmove.o
$compiler $params -c bg_misc.c -o obj/bg_misc.o
$compiler $params -c g_utils.c -o obj/g_utils.o
$compiler $params -c g_spawn.c -o obj/g_spawn.o
$compiler $params -c q_math.c -o obj/q_math.o
echo "[SERVER]"
$compiler $params -c sv_commands.c -o obj/sv_commands.o
$compiler $params -c sv_client.c -o obj/sv_client.o
$compiler $params -c sv_world.c -o obj/sv_world.o
$compiler $params -c sv_game.c -o obj/sv_game.o
$compiler $params -c sv_init.c -o obj/sv_init.o
$compiler $params -c net_chan.c -o obj/net_chan.o
$compiler $params -c sv_main.c -o obj/sv_main.o
$compiler $params -c x_clientcmds.c -o obj/x_clientcmds.o
$compiler $params -c unix_net.c -o obj/unix_net.o
echo "[SCRIPT]"
$compiler $params -c pre.c -o obj/pre.o
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

if [ $DEBUG = true ]; then
for f in obj/*.o; do
if [ "$f" != "duktape.o" ]; then
#objcopy --keep-symbols=symbols.txt --prefix-symbols=xtn_ $f
objcopy --redefine-syms=syms.txt $f
fi
done
fi

if [ $uMYSQL = true ]; then
if [ $DEBUG = true ]; then
$compiler -m32 -shared -L/lib32 -L/home/lib -lmysqlclient -L/usr/lib/mysql -I/usr/include/mysql -o ./bin/codextended.so $obj -Os -lz $LINK_CURL -ldl -lm -Wall
else
$compiler -m32 -shared -L/lib32 -L/home/lib -lmysqlclient -L/usr/lib/mysql -I/usr/include/mysql -o ./bin/codextended.so $obj -Os -s -lz $LINK_CURL -ldl -lm -Wall
fi
#$compiler -m32 -shared -L/lib32 `mysql_config --libs` -I/usr/include/mysql -o ./bin/codextended.so $obj -Os -s -lz $LINK_CURL -ldl -lm -Wall
else
if [ $DEBUG = true ]; then
$compiler -m32 -shared -L/lib32 -L./lib -o ./bin/codextended.so $obj -lz $LINK_CURL -ldl -lm -Wall
else
$compiler -m32 -shared -L/lib32 -L./lib -o ./bin/codextended.so $obj -Os -s -lz $LINK_CURL -ldl -lm -Wall
fi
fi
#rm -rf ./obj
find ./obj -name '*.o' ! -name 'duktape.o' -delete
echo "Done."
