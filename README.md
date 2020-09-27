# VideoHelpers
various 'video helper' utilities and documentation for creating videos and effects


* make_lcd_saver_jpegs.c

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



*
