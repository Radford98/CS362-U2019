CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

cardEffect.o: cardEffect.h cardEffect.c
	gcc -c cardEffect.c -g $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o cardEffect.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o cardEffect.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o cardEffect.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o cardEffect.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o cardEffect.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o cardEffect.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

randomtestcard1: randomtestcard1.c dominion.o rngs.o cardEffect.o
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o cardEffect.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o rngs.o cardEffect.o
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o cardEffect.o $(CFLAGS)

randomtestcard3: randomtestcard3.c dominion.o rngs.o cardEffect.o interface.o
	gcc -o randomtestcard3 -g randomtestcard3.c dominion.o rngs.o cardEffect.o interface.o $(CFLAGS)

randomtestresults: randomtestcard1.c randomtestcard2.c randomtestcard3.c dominion.o rngs.o cardEffect.o interface.o
	echo "Running random card tests" > randomtestresults.out
	echo "randomtestcard1.c:" >> randomtestresults.out
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o cardEffect.o $(CFLAGS)
	./randomtestcard1 >> randomtestresults.out
	gcov -b -f -c randomtestcard1.c >> randomtestresults.out
	echo "randomtestcard2.c:" >> randomtestresults.out
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o cardEffect.o $(CFLAGS)
	./randomtestcard2 >> randomtestresults.out
	gcov -b -f -c randomtestcard2.c >> randomtestresults.out
	echo "randomtestcard3.c:" >> randomtestresults.out
	gcc -o randomtestcard3 -g randomtestcard3.c dominion.o rngs.o cardEffect.o interface.o $(CFLAGS)
	./randomtestcard3 >> randomtestresults.out
	gcov -b -f -c randomtestcard3.c >> randomtestresults.out
	echo "Coverage of card functions:" >> randomtestresults.out
	gcov -b -f -c cardEffect.c >> randomtestresults.out

player: player.c interface.o dominion.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o cardEffect.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
