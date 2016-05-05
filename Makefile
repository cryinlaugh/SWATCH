CXX = gcc
CFLAGS = -O3

test: test.o mnist.o cnnConvolve.o blob.o
		$(CXX) $(CFLAGS) $^ -o $@ -I. /opt/OpenBlas/lib/libopenblas.a -L/opt/OpenBLAS/lib -lopenblas -lpthread
cnnConvolve.o: cnnConvolve.c
		$(CXX) $(CFLAGS) -c $< -I. -I/opt/OpenBLAS/include/
test.o: test.c
		$(CXX) $(CFLAGS) -c $< -I. 
mnist.o: mnist.c
		$(CXX) $(CFLAGS) -c $< -I.
blob.o: blob.c
		$(CXX) $(CFLAGS) -c $< -I.
clean:
		rm log/*
		rm *.o test
