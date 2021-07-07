#!/bin/sh

if test -z "$1""$2""$3" ; then
  echo usage:  fade_it.sh [in/out] filename.jpg "'"nn"'" "'"[mm]"'" "'"d"'"
  echo where "'"nn"'" is # of jpeg files to fade in/out
  echo and "'"in"'" or "'"out"'" indicates the direction
  echo ""
  echo File names generated is shit/frame'####'.jpg with seq num starting with "'"mm"'"
  echo '('default 0000')' using "'"d"'" digits '('default 6 digits')'
  exit
fi

if test "$1" == "in" -o "$1" == "out" ; then
  # it's ok
else
  echo Must specify "'"in"'" or "'"out"'"
  exit
fi

inout=$1
input_file="$2"
num_frames="$3"

if test -z "$4" ; then
  start_seq=0
  digits=6
else
  start_seq="$4"

  if test -z "$5" ; then
    digits=6
  else
    digits=$5
  fi
fi

rm -rf shit
mkdir shit

echo $inout $input_file $num_frames $digits $start_seq


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

  if test ${inout} == "in" ; then
    ctr2=${ctr}
  else
    ctr2=`expr $num_frames - $ctr - 1`
  fi

  ctr3=`expr ${num_frames} - 1`
  contrast=`expr 100 \* ${ctr2} / ${ctr3}`

  convert -modulate ${contrast} ${input_file} shit/frame${frameno}.jpg

  ctr=`expr ${ctr} + 1`

done

