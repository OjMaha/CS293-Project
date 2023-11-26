#------------------------
# Do not modify this file
#------------------------

all: trader iml

trader: combine.cpp receiver.h
	g++ -std=c++20 combine.cpp -o trader

ojas_trader: traderpart2.cpp receiver.h
	g++ -std=c++20 traderpart2.cpp -o ojas_trader

tk_trader: traderpart3tk.cpp receiver.h
	g++ -std=c++20 traderpart3tk.cpp -o tk_trader

iml: iml.cpp
	g++ -std=c++20 iml.cpp -o iml

runpart1: iml trader
	./trader 1 > output.txt &
	sleep 1
	./iml inputs/part1/data1.txt
	diff outputs/out1.txt output.txt

runpart2: iml trader
	./trader 2 > output.txt &
	sleep 1
	./iml inputs/part2/data2.txt
	diff outputs/out2.txt output.txt

runpart3: iml trader
	./trader 3 > output.txt &
	sleep 1
	./iml inputs/part3/data3.txt
	diff outputs/out3.txt output.txt

ojas: iml ojas_trader
	./ojas_trader 2 > output.txt &
	sleep 1
	./iml inputs/part2/data2.txt

tk_trader1: traderpart1.cpp receiver.h
	g++ -std=c++20 traderpart1.cpp -o tk_trader1


tk: iml tk_trader
	./tk_trader 3 > output1.txt &
	sleep 1
	./iml inputs/data3.txt
#	diff outputs/out3.txt output.txt

tk1: iml tk_trader1
	./tk_trader1 1 > output.txt &
	sleep 1
	./iml inputs/part1/data1.txt
#	diff outputs/out1.txt output.txt

socket_trader: socket.cpp receiver.h
	g++ -std=c++20 socket.cpp -o socket_trader

socket: iml socket_trader
	./socket_trader 3 > output.txt &
	sleep 1
	./iml inputs/mydata.txt

clean:
	rm -rf output.txt trader iml
	rm -rf *~