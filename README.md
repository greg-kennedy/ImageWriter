# ImageWriter
Greg Kennedy, 2026

## Overview
This is a tool to handle Apple ImageWriter printer control language.  Given one or more input files, it will render them to .bmp images, pure .txt files, or PostScript documents.

In fact, almost none of the code was written by me - the ImageWriter interpreter and graphics plotting was taken wholesale from the [GSport IIGS emulator](https://github.com/david-schmidt/gsport/blob/main/src/imagewriter.cpp), in turn borrowed from KEGS and DOSBox.  My contribution was simply a wrapper program to make it work from the CLI, and Makefile that "works on my machine" :)

The idea for this came from [a post by Colin Leroy-Mira](https://www.colino.net/wordpress/en/print-from-an-apple-ii-to-any-modern-printer/) but since I lack physical hardware, it's an all-software solution.

Because the original code requires SDL 1.2 for plotting graphics, so does this version.  Sorry!

## Usage
`./imagewriter [-d dpi] [-p pageSize] [-b bannerSize] [-o outputType] [-m] input.txt [input2.txt ...]`

* DPI is arbitrary, but note that the print head in the ImageWriter could only achieve something like 144 DPI, so multiples of this are probably good choices.
* Paper Sizes, 0 through 6, which corresponds to one of these:  (length units are Postscript Points, 72 per inch)
```
	0 - 612 x 792, //US Letter 8.5 x 11in
	1 - 612 x 1008, //US Legal 8.5 x 14in
	2 - 595 x 842, //ISO A4 210 x 297mm
	3 - 499 x 709, //ISO B5 176 x 250mm
	4 - 1071 x 792, //Wide Fanfold 14 x 11in
	5 - 792 x 1224, //Ledger/Tabloid 11 x 17in
	6 - 842 x 1191, // ISO A3 297 x 420mm
```
* bannerSize asks how many pages you want to use for your banner.  Default 0 (no banner).
* outputType asks for one of these
```
	"text" - write text components to a text file
	"png" - would write to PNG if I could get libpng working
	"colorps" - color postscript file?
	"ps" - grayscale postscript file
	anything else - BMP (default) - this is all I tested sorry
```
* multiPageOutput - for PS / ColorPS, output each page in a one multi-page doc, instead of as separate per-page .ps.  (Default off, specify `-m` to enable)

You may specify multiple input.txt files.  To create input files, you can use something like [AppleWin](https://github.com/AppleWin/AppleWin) with the "printer dump filename" to log printer commands to a file, then feed them here.

To read from STDIN instead of a file, use `-` as the only input filename instead.

## Output
The output writer code seems to put out files of the format `page####.bmp` with an increasing number, or for multi-page ps files, `doc####.ps`.

There is also a font path specified but I don't know what that does nor do I care.  The Print Shop can make perfectly good cards without it, and that's all I wrote this for.
