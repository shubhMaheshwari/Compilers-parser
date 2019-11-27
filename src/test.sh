#/bib/zsh 

TEST_DIR="../test-programs/small-programs/"


echo "Starting testing:"
for d in $TEST_DIR* ; 
do
    echo "Testing: $d" ;
    ./parser < $d; # creates an output.o file
    g++ output.o -o compiled_file
    ./compiled_file
    break
done