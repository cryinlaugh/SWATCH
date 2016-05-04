CXX = gcc

test: test.o mnist.o cnnConvolve.o blob.o
		$(CXX) $^ -o $@ -I.
cnnConvolve.o: cnnConvolve.c
		$(CXX) -c $< -I.
test.o: test.c
		$(CXX) -c $< -I. 
mnist.o: mnist.c
		$(CXX) -c $< -I.
blob.o: blob.c
		$(CXX) -c $< -I.
clean:
		rm *.o test
