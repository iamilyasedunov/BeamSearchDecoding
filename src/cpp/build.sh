g++ -Wall -O2 -Wextra -pedantic -fPIC -DBUILD_MYLIB --std=c++11 -o BeamSearch.o main.cpp MatrixCSV.cpp Beam.cpp
# g++ -shared -o BeamSearch.dll BeamSearch.o