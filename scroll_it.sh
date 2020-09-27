#!/bin/sh

if test -z "$1" ; then
  echo usage:  scroll_it.sh filename.jpg '[nn]'
  echo where 'nn' is # lines scrolled per frame
  exit
fi

input_file="$1"

if test -z "$2" ; then
  lines_per_frame=1
else
  lines_per_frame="$2"
fi

rm -rf jpegs
mkdir jpegs

# how big is the image?

height=`identify "$1" | awk '{ A=$3; I=index(A,"x"); print substr(A, I+1); }'`

lines=`expr ${height} - 720`

curline=0

## temporary
#lines=500

while test ${curline} -lt ${lines} ; do

  frameno=`echo $curline | awk '{ AA="00000000"$1; LL=length(AA); print substr(AA,LL-5,6); }'`

  outfile=jpegs/frame${frameno}.jpg

  convert -crop 1280x720+0+${curline} ${input_file} ${outfile}

  curline=`expr ${curline} + ${lines_per_frame}`

done

# make 1 seconds' worth of panels for the ending, and fade it out

lines=`expr ${lines} + 25`

while test ${curline} -lt ${lines} ; do

  frameno=`echo $curline | awk '{ AA="00000000"$1; LL=length(AA); print substr(AA,LL-5,6); }'`

  cp ${outfile} jpegs/frame${frameno}.jpg

  curline=`expr ${curline} + 1`

done

contrast=100
while test ${contrast} > 0 ; do

  frameno=`echo $curline | awk '{ AA="00000000"$1; LL=length(AA); print substr(AA,LL-5,6); }'`

  contrast=`expr ${contrast} - 4`

  convert -modulate ${contrast} ${outfile} jpegs/frame${frameno}.jpg

  curline=`expr ${curline} + 1`

done

echo  use a command similar to this to create the video
echo  mencoder '"'mf://jpegs/*jpg'"' -idx -ovc x264 -mf type=jpeg:fps=25 -o out.avi -audiofile /path/to/my/audio.mp3 -oac lavc
echo ""
echo follow this with:"  "ffmpeg -i out.avi -c:a mp2 out.mp4
echo ""



