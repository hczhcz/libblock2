mkdir -p ./build

cc='clang++'
flags='-std=c++14 -stdlib=libc++ -O0 -g -Wall -Wextra -ferror-limit=3'
flags_pp='-E -DSKIP_LIB'
flags_pch=''
flags_obj='-c'
flags_out='-lgc'

pg='python ./lib/myparser/myparser.py'
pg_flags='c++ -p ./lib/myparser/'

lint='cppcheck'
lint_flags='-q --std=c++11 --enable=all' # --std=c++14 --inconclusive

test_cc='clang'
test_flags='-O0 -g -Wall -Wextra -ferror-limit=3'
test_flags_out='-lgc'

echo '======== build precompiled files ========'
echo

new=$(cat ./include.hpp)

echo $new | diff -q - ./build/include.hpp 1> /dev/null 2> /dev/null
if [ $? -eq 0 ]
then
    inc_changed=0
    echo 'not changed'
    echo
else
    inc_changed=1
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

echo '======== generate the parser ========'
echo

echo $pg $pg_flags ./syntax.md -o ./parser_syntax.hpp
$pg $pg_flags ./syntax.md -o ./parser_syntax.hpp
if [ $? -ne 0 ]
then
    exit
fi
echo

echo '======== build updated files ========'
echo

objs=''
upds=''

for file in $(ls ./*.cpp)
do
    new=$($cc $flags_pp $flags $file)

    echo $new | diff -q - ./build/$file 1> /dev/null 2> /dev/null
    if [ $? -eq 0 -a $inc_changed -eq 0 ]
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

        upds=$(echo $upds $file)
    fi

    objs=$(echo $objs ./build/$file.o)
done

echo '======== lint ========'
echo

echo $lint $lint_flags $upds
$lint $lint_flags $upds
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
