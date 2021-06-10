input=`cat ../systemTests/scripts/$1`
echo $input | ./client > "output.txt"
