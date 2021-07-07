# VideoHelpers
various 'video helper' utilities and documentation for creating videos and effects


## make_lcd_saver_jpegs.c

This utility works with mencoder to create an 'lcd screen saver' video.  Build
and run instructions are in the source as comments.

In short, it outputs a bunch of video frames as JPEG files to './jpeg_output/'.
From here, you can use an application like 'mencoder' to create an actual
video '.avi' file.

The video itself is similar to the Linux screen saver that helps correct those
irritating "burn" problems that LCD displays sometimes get, by exercising the
pixels.  Looping the video for several hours might even completely remove the
problem except on those must stubborn of monitors.  So Your Mileage May Vary
as to how well it works.  The utility itself allows you to create a video with
a resolution that matches your monitor.  You might want to make sure that the
aspect ratio is also correctly assigned so that playing it 'full screen' will
give you the correct behavior, exercising individual pixels.


## copy_it.sh

This script copies a single image as multiple files with ascending sequence
numbers, intended to be compatible with the other scripts.  You can specify
the starting sequence and number of images (frames).


## scroll_it.sh fade_it.sh

These shell scripts require 'Image Magick' to have been installed.  They use
the 'convert' utility to take a "tall" jpeg image and turn it into a set of
frames with unique sequence numbers.

The 'fade_it.sh' utility will fade in or fade out a single image, creating
a sequence of image files, similar to copy_it.sh, but with varying contrast
via the 'convert' utility.  This effectively "fades in" or "fades out" an
image.  This can be useful for title screens.

The 'scroll_it.sh' utility specifically creates a set of 1280x720 frames,
scrolling from the top to the bottom.  If you were to use a text editor
(let's say Libre Office) to output a "tall" JPEG image containing the
text, with a width that's appx 1280 pixels, you could then use this
script to create a "scrolling credits" ending for a video.

With Libre Office, setting the page length to something large enough
(let's say 36 inches), with a width of 8 inches, would require you to use a
bit resolution of 160 pixels per inch to get a width of ~1280, when you use
Libre Office's "File Export" menu AND export to a JPEG file.  Then, you
put 720 pixels' worth of space (a little over 4.5 inches) at the very top
of the document so that the scrolling starts out black.

If you use default colors (black text, white background) with your document,
do not worry.  Simply load it into 'gimp' and invert the colors by using the
"Colors Invert" menu, and re-export it back into the same file (or a different
one if you prefer).  THEN, run 'scroll_it.sh' to create the output files in
the 'jpeg' directory.

The file 'ScrollingCredits_Sample.odt' is an empty Libre Office document file
that has an extremely tall page length, as an example.


