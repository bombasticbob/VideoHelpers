#!/bin/sh

if test -z "$1""$2" ; then
  echo usage:  copy_it.sh filename.jpg "'"nn"'" "'"[mm]"'" "'"d"'"
  echo where "'"nn"'" is # of jpeg files to fade in/out
  echo ""
  echo File names generated is jpegs/frame'####'.jpg with seq num starting with "'"mm"'"
  echo '('default 0000')' using "'"d"'" digits '('default 6 digits')'
  exit
fi

input_file="$1"
num_frames="$2"

if test -z "$3" ; then
  start_seq=0
  digits=6
else
  start_seq="$3"

  if test -z "$4" ; then
    digits=6
  else
    digits=$4
  fi
fi

rm -rf jpegs
mkdir jpegs

echo $input_file $num_frames $digits $start_seq

# how big is the image?

height=`identify "$input_file" | awk '{ A=$3; I=index(A,"x"); print substr(A, I+1); }'`
width=`identify "$input_file" | awk '{ A=$3; I=index(A,"x"); print substr(A, 1, I-1); }'`

height=`expr ${height}`
width=`expr ${width}`

ctr=0

while test ${ctr} -lt ${num_frames} ; do

  echo pass $ctr

  dig2=`expr ${digits} - 1`
  frm=`expr ${start_seq} + ${ctr}`
  frameno=`echo ${frm} | awk '{ AA="0000000000"$1; LL=length(AA); print substr(AA,LL-'${dig2}','${digits}'); }'`

  # consider hard link
  cp -p ${input_file} jpegs/frame${frameno}.jpg

  ctr=`expr ${ctr} + 1`

done

