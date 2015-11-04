cc='clang++'
# flags='-std=c++11 -O0 -g -Wall -Wextra -pedantic -ferror-limit=3'
flags='-std=c++11 -O0 -g -Wall -Wextra -ferror-limit=3'
flags_pp='-E -DNO_STD_LIB'
flags_obj='-c'
flags_out=''

objs=''

for file in $(ls ./*.cpp)
do
    new=$($cc $flags_pp $flags $file)

    echo $new | diff -q - ./build/$file 1> /dev/null 2> /dev/null
    if [ $? -eq 0 ]
    then
        echo 'no change: '$file
    else
        echo 'changed: '$file

        $cc $flags_obj $flags $file -o ./build/$file.o
        if [ $? -eq 0 ]
        then
            echo $new > ./build/$file
        else
            echo '' > ./build/$file
            exit
        fi
    fi

    objs=$(echo $objs ./build/$file.o)
done

$cc $flags_out $flags $objs -o ./test && ./test
