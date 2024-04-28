cc = g++
target = mp3_20211590
objects = mp3_20211590.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

%.o: %.cpp
	$(cc) -c -o $@ $<

.PHONY : clean
clean :
	rm -f *.o $(target)
