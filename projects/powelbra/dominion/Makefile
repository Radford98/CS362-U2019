CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

cardEffect.o: cardEffect.h cardEffect.c
	gcc -c cardEffect.c -g $(CFLAGS)

testHelp.o: testHelp.h testHelp.c
	gcc -c testHelp.c -g $(CFLAGS)

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

unittest1: unittest1.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)

unittest2: unittest2.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)

unittest3: unittest3.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)

unittest4: unittest4.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)

unittest5: unittest5.c dominion.o rngs.o cardEffect.o testHelp.o
	gcc -o unittest5 -g unittest5.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)

cardtest1: cardtest1.c dominion.o rngs.o cardEffect.o interface.o
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o cardEffect.o interface.o $(CFLAGS)

cardtest2: cardtest2.c dominion.o rngs.o cardEffect.o
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o cardEffect.o $(CFLAGS)

cardtest3: cardtest3.c dominion.o rngs.o cardEffect.o
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o cardEffect.o $(CFLAGS)

cardtest4: cardtest4.c dominion.o rngs.o cardEffect.o interface.o
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o cardEffect.o interface.o $(CFLAGS)

cardtest5: cardtest5.c dominion.o rngs.o cardEffect.o
	gcc -o cardtest5 -g cardtest5.c dominion.o rngs.o cardEffect.o $(CFLAGS)
	
unittestresults: unittest1.c unittest2.c unittest3.c unittest4.c unittest5.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c cardtest5.c dominion.o rngs.o cardEffect.o interface.o testHelp.o
	echo "Running unit and card tests" > unittestresults.out
	echo "unittest1.c:" >> unittestresults.out
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
	./unittest1 >> unittestresults.out
	gcov -b -f -c unittest1.c >> unittestresults.out
	echo "unittest2.c:" >> unittestresults.out
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
	./unittest2 >> unittestresults.out
	gcov -b -f -c unittest2.c >> unittestresults.out
	echo "unittest3.c:" >> unittestresults.out
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
	./unittest3 >> unittestresults.out
	gcov -b -f -c unittest3.c >> unittestresults.out
	echo "unittest4.c:" >> unittestresults.out
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
	./unittest4 >> unittestresults.out
	gcov -b -f -c unittest4.c >> unittestresults.out
	echo "unittest5.c:" >> unittestresults.out
	gcc -o unittest5 -g unittest5.c dominion.o rngs.o cardEffect.o testHelp.o $(CFLAGS)
	./unittest5 >> unittestresults.out
	gcov -b -f -c unittest5.c >> unittestresults.out
	echo "cardtest1.c:" >> unittestresults.out
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o cardEffect.o interface.o $(CFLAGS)
	./cardtest1 >> unittestresults.out
	gcov -b -f -c cardtest1.c >> unittestresults.out
	echo "cardtest2.c:" >> unittestresults.out
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o cardEffect.o $(CFLAGS)
	./cardtest2 >> unittestresults.out
	gcov -b -f -c cardtest2.c >> unittestresults.out
	echo "cardtest3.c:" >> unittestresults.out
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o cardEffect.o $(CFLAGS)
	./cardtest3 >> unittestresults.out
	gcov -b -f -c cardtest3.c >> unittestresults.out
	echo "cardtest4.c:" >> unittestresults.out
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o cardEffect.o interface.o $(CFLAGS)
	./cardtest4 >> unittestresults.out
	gcov -b -f -c cardtest4.c >> unittestresults.out
	echo "cardtest5.c:" >> unittestresults.out
	gcc -o cardtest5 -g cardtest5.c dominion.o rngs.o cardEffect.o $(CFLAGS)
	./cardtest5 >> unittestresults.out
	gcov -b -f -c cardtest5.c >> unittestresults.out
	echo "Coverage of card functions:" >> unittestresults.out
	gcov -b -f -c cardEffect.c >> unittestresults.out
	echo "Coverage of dominion.c code:" >> unittestresults.out
	gcov -b -f -c dominion.c >> unittestresults.out


all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
