CXX = g++
CFLAGS = -Wall -std=c++11
# LDFLAGS= -static
PROG = anzupop_mock_fs
DEPS = fs.cc fs.h main.cc utils.cc utils.h test.h
OBJ = fs.o main.o utils.o
 
$(PROG): $(OBJ)
		$(CXX) -o $(PROG) $(OBJ) $(LDFLAGS)
		rm -rf $(OBJ)
             
%.o: %.cc $(DEPS)
		$(CXX) -c $< -o $@ $(CFLAGS)
             
clean:
		rm -rf $(OBJ) $(PROG)