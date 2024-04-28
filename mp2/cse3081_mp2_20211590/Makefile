cc = g++
target = mp2_20211590
objects = mp2_20211590.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

%.o: %.cpp
	$(cc) -c -o $@ $<

.PHONY : clean
clean :
	rm -f *.o $(target)
