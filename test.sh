mkdir -p ./build

cc='clang++'
flags='-std=c++14 -O0 -g -Wall -Wextra -ferror-limit=3'
flags_pp='-E -DSKIP_STD_LIB'
flags_pch=''
flags_obj='-c'
flags_out=''

lint='cppcheck'
lint_flags='-q --std=c++11 --enable=all --inconclusive' # c++14?

test_cc='clang'
test_flags='-O0 -g -Wall -Wextra -ferror-limit=3'
test_flags_out='-lgc'

echo '======== build precompiled files ========'
echo

new=$(cat ./include.hpp)

echo $new | diff -q - ./build/include.hpp 1> /dev/null 2> /dev/null
if [ $? -eq 0 ]
then
    echo 'not changed'
    echo
else
    echo 'changed'

    echo $cc $flags_pch $flags ./include.hpp -o ./build/include.pch
    $cc $flags_pch $flags ./include.hpp -o ./build/include.pch
    if [ $? -eq 0 ]
    then
        echo $new > ./build/include.hpp
    else
        echo '' > ./build/include.hpp
        exit
    fi
    echo
fi

echo '======== build updated files ========'
echo

objs=''

for file in $(ls ./*.cpp)
do
    new=$($cc $flags_pp $flags $file)

    echo $new | diff -q - ./build/$file 1> /dev/null 2> /dev/null
    if [ $? -eq 0 ]
    then
        echo 'not changed: '$file
        echo
    else
        echo 'changed: '$file

        echo $cc $flags_obj $flags -include-pch ./build/include.pch $file -o ./build/$file.o
        $cc $flags_obj $flags -include-pch ./build/include.pch $file -o ./build/$file.o
        if [ $? -eq 0 ]
        then
            echo $new > ./build/$file
        else
            echo '' > ./build/$file
            exit
        fi
        echo
    fi

    objs=$(echo $objs ./build/$file.o)
done

echo '======== lint ========'
echo

$lint $lint_flags ./*.cpp
echo

echo '======== build the test ========'
echo

echo $cc $flags_out $flags $objs -o ./build/test.out
$cc $flags_out $flags $objs -o ./build/test.out
if [ $? -ne 0 ]
then
    exit
fi
echo

./build/test.out
if [ $? -ne 0 ]
then
    exit
fi
echo

echo '======== build the generated file ========'
echo

echo $test_cc $test_flags_out $test_flags ./build/test.gen.c -o ./build/test.gen.out
$test_cc $test_flags_out $test_flags ./build/test.gen.c -o ./build/test.gen.out
if [ $? -ne 0 ]
then
    exit
fi
echo

./build/test.gen.out
if [ $? -ne 0 ]
then
    exit
fi
echo
