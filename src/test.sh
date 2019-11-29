#/bib/zsh 

TEST_DIR="../test-programs/"


echo "Starting testing:"
for d in $TEST_DIR* ; 
do
    echo "Testing: $d" ;
    ./parser < $d; # creates an output.o file
    g++ output.o -o test
    ./test
    rm test output.o
done