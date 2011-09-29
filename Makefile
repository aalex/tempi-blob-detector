all: cvfun
	@echo DONE

cvfun: main.cpp
	g++ main.cpp -o cvfun `pkg-config --libs --cflags opencv`

clean:
	rm -rf cvfun
