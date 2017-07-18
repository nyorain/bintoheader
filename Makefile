CXX=g++
PREFIX=/usr

bintoheader: bintoheader.cpp
	$(CXX) -std=c++1z bintoheader.cpp -o bintoheader

#.PHONY: install
install: bintoheader
	install bintoheader $(PREFIX)/bin

#.PHONY: clean
clean:
	rm bintoheader
