#Compiling CoDExtended
compiler="g++"
params="-I. -m32 -fPIC -Wno-write-strings"
#params="-m32 -fPIC -Wno-write-strings"

mkdir bin
mkdir obj
echo "Compiling..."
echo "[ROOT]"
$compiler $params -c init.cpp -o obj/init.opp
$compiler $params -c codextended.cpp -o obj/codextended.opp
$compiler $params -c entity.cpp -o obj/entity.opp
echo "[COMMON]"
$compiler $params -c common/cracking.cpp -o obj/cracking.opp
$compiler $params -c common/cvar.cpp -o obj/cvar.opp
$compiler $params -c common/files.cpp -o obj/files.opp
echo "[GAME]"
$compiler $params -c game/shared.cpp -o obj/shared.opp
$compiler $params -c game/script.cpp -o obj/script.opp
echo "[SERVER]"
$compiler $params -c server/sv_commands.cpp -o obj/sv_commands.opp
$compiler $params -c server/sv_client.cpp -o obj/sv_client.opp
$compiler $params -c server/sv_world.cpp -o obj/sv_world.opp
$compiler $params -c server/sv_game.cpp -o obj/sv_game.opp
$compiler $params -c server/sv_init.cpp -o obj/sv_init.opp
echo "[SCRIPT]"
$compiler $params -c game/script/scr_method_player.cpp -o obj/scr_method_player.opp
$compiler $params -c game/script/scr_method_entity.cpp -o obj/scr_method_entity.opp
$compiler $params -c game/script/scr_mysql.cpp -o obj/scr_mysql.opp
obj="$(ls obj/*.opp)"
$compiler -m32 -shared -L/lib32 -lmysqlclient -L/usr/lib/mysql -I/usr/include/mysql -L./lib -o ./bin/codextended.so $obj -Os -s -ldl -lm -Wall
rm -rf ./obj
echo "Done."
