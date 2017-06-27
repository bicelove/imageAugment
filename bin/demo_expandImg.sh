mpwd=`pwd`
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$mpwd/../lib

####################################################
imgList='../data/300.txt'
svPath='/home/in66/data/cloth/170227/2_expand_save/'
keyfilePath='../keyfile/'

dealDir='/home/in66/data/cloth/170227/2_expand'
ls $dealDir > $keyfilePath/dirList.txt

#valgrind  --tool=memcheck  --leak-check=full --show-reachable=yes  --num-callers=30 --track-origins=yes --log-file=val.log 
./Demo_expandImg $imgList $svPath $keyfilePath
