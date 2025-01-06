#!/bin/fish

set FileName $argv

set parts (string split "." $FileName)

# 文件名与后缀
set basename $parts[1]
set extension (string lower $parts[2])

# echo $basename
# echo $extension

if test "$extension" = "cpp"
    clang++ -std=c++26 -O2 -pipe -Wno-vla -g -I/home/Gopher/Code/Headers/ -DLOCAL src/{$FileName} -o build/{$basename}
    cd build && ./{$basename}

else if test "$extension" = "py"
    pypy3 src/{$FileName}

else
    echo "This file type is not supported !!!"

end
