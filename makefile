# Meus Codigos

loadDataCpp:
	clear && g++ src/MeuCpp/loadData.cpp -o src/out/loadData -Wall && clear && src/out/loadData

mainCpp:
	clear && g++ src/MeuCpp/main.cpp -std=c++2a -Wshadow -Wall -o src/out/mainCpp -Wno-unused-result -lpthread -O3 -Wall && clear && src/out/mainCpp

mainPy:
	clear src/MeuPy/main.py

# Codigos KTNS

KTNS:
	clear && g++ ./src/KTNS/main.cpp ./src/KTNS/src/*.cpp -std=c++2a -Wshadow -Wall -o ./src/out/KTNS -Wno-unused-result -lpthread -O3 -Wall

KTNSTest1:
	clear && ./src/out/KTNS ./dataRaw/KTNS/Instances/Mecler/Tabela1/F1001.txt 6 ./dataRaw/KTNS/SolList/Files/sollist50.txt

KTNSTest2:
	clear && ./src/out/KTNS ./dataRaw/KTNS/Instances/Catanzaro/Tabela1/datA1 6 ./dataRaw/KTNS/SolList/Files/sollist10.txt

# Update Git

git:
	clear && git add . && git commit -m "update" && git push origin master