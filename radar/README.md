This is OpenGC adapted for X-Plane by use of a new TCP/IP data plugin between X-Plane
and OpenGC called XPSERVER. The plugin allows to transfer X-Plane internal and plane
specific data. The code has been tested on OSX 10.6 and on Ubuntu 18.04

I have removed the cmake dependency and implemented a standard gnu configure/autoconf/automake toolset.
Also, several of the codes were cleaned from warnings and the xplane datasource was completely remodeled,
plus all other data sources were turned off. They can be turned on with some modifications to their code,
so that they are in line again with the current state of the OpenGC framework. Windows compilation support
was removed (who cares about windows anyway ...).

## Installation Dependencies

* gcc/g++ compiler
* gnu Autotools / Autoconf
* FTGL library (=2.1.2 stable release, 2.1.3 and above does not work yet)
* FLTK library (>=1.1.7)
* Freetype library (>=2.1.9)
* X11R6 Framework
* OpenGL library (GL and GLU)

## Installation How-to

resolve above depencies, for debian/ubuntu:

```
sudo apt install libfltk1.3-dev -y
sudo apt install libftgl-dev -y
sudo apt install libfreetype6-dev -y
sudo apt install build-essential -y
sudo apt install autoconf -y
```

In case you cloned from git, please first run these in order to complete automake/autoconf file structure:

```
autoreconf
automake --add-missing
autoreconf
```

Then type:

```
./configure 
make
```

This should build the application xpopengc in src/main.

Since either of the FTGL, FLTK, Plib, Freetype libraries might be located in a custom location, type ./configure --help
to see the options to customize the paths to those libraries during configuration. The freetype library somehow requires
to specify its prefix on Ubuntu/Debian: so try:

```
./configure --with-freetype=/usr
```

## Run How-to:

edit the file inidata/default.ini and change the following items according to your OpenGC/X-Plane configuration:

```
cd src/main
./xpopengc default
```

It should start a X window with a default gauge and tell you on the command line that it expects a X-Plane connection

Start X-Plane with the xpserver plugin. You should see OpenGC display content changing according
To your flight data.

## Configuration using opengc.ini

To get running, edit opengc.ini so that it matches your system
configuration and desired gauge layout.

## Paths
The first portion of opengc.ini establishes the location of your font and
nav data directories. Both declarations are formatted in the same way, as
follows:

```
NAV DATABASE PATH
/Users/beowulf/opengc/xplanenav/
FONT PATH
/Users/beowulf/opengc/opengc/Fonts/
```

## Data source selection
You should verify that the correct data source is selected for the type of
sim that you have. There are restrictions as to which data source will work
with which operating systems, some of which arise from the sims themselves
4and some of which are limitations that may be removed in the
future. Please see Data source / sim configuration for more details.

* X-Plane version 6.30+

```
DATA SOURCE
X-Plane
```

## Render window
The parameters of the render window are xPosition yPosition xSize
ySize frameTest, with the first four being expressed in integer pixels and
the last as 0=false, 1=true. Note that the size and position represent initial
window placement and size; you can change this quite easily once
is running. The frame rate test runs through a brief demo once
loads if set to true (i.e. 1) - this unfortunately does not work on Windows
at the moment because of FLTK issues.

```
RENDER WINDOW
0 0 1024 700 1
```

## Gauges
Gauges are defined by:

```
NEWGAUGE
GaugeName
xPosition yPosition xScale yScale
ENDGAUGE
```

Positions are measured in approximate millimeters (more full-featured
calibration tools will be included in version 1.0). Scales are
measured in % / 100. Therefore 1.0 is full scale, 0.5 is half, and 2.0 is
double. As provided, it should work well on a 1024x768 17 inch
monitor without modifying the scale and position of the default gauges.

## Data source / sim configuration
Although it’s possible to run on the same machine as the sim, this
is not the way to get good performance. I strongly suggest creating a small
LAN and running separate sim and glass cockpit machines.

## X-Plane configuration
can read X-Plane packets sent via UDP over port 49000. For these
instructions, it assumed that you are running both X-Plane on
a private network, 192.168.1.x, as is common with Cable/DSL routers. More
complicated network configuration is beyond the scope of this document.

1. In opengc.ini, make sure X-Plane is selected as your data source.
The remainder of these instructions refer to X-Plane itself.
2. Click the circle next to IP address of data receiver in the Set
Internet Connections dialog. Fill in 192.168.1.255 as the address,
and 49000 as the port number. Note that 192.168.1.255 is the broad-
cast address for the network, meaning that all machines with IP’s of
192.168.1.x will receive data from X-Plane. This allows you to eas-
ily add extra machines to display different gauges without
reconfiguring X-Plane.
3. Close the internet connections page. Open Set Data Output and
check the first of four circles next to the following variables: 2, 7, 24,
725, 37, 38, 39, and 40. If you fail to highlight a variable, it will not be
sent. Selecting incorrect variables will not crash,
but it may result in a particular gauge not working as expected
4. Start flying. It does not matter whether X-Plane is started
first.

## Developer’s guide

Style guide - read me before coding!
Please adhere to the following style conventions when writing code. These conventions
help to ensure easy interpretation of code and provide for consistent formatting between
developers.

## Class and variable names

* Variable and class names should be as verbose as necessary to describe the intent of the
variable/object without needing to wade through a morass of acronyms. The exception to
this is acronyms which are accepted as common aviation language, for instance PFD, EICAS,
etc. For example, m PFDSkyColor is an acceptable variable name, m Hyk8 is not.
* In general, all class and variable names should follow the “camel hump” style of capitalization - thisIsCamelHumpCapitalization
* Variables which are class members should be named using the Microsoft convention of pref-
acing the variable name with m . Variables which are local to a function do not have
the m preface and do not have their first character capitalized. As an example, indi-
cated airspeed in knots would be named m IndicatedAirspeedKnots if a class member and
indicatedAirspeedKnots if a local function variable. Certain functions and classes may
break this convention; for instance, the FSUIPC data source names raw values with a
character preface.

## Tabs and indentation

* Configure your editor to insert 2 spaces rather than hard tabs. This ensures that indentation
appears constant regardless of the tab setting of the editor being used.
* Indent function/class/loop/etc. braces flush with the line immediate above, and then indent
the body of the function one additional level. For instance:

```
for(int i=0; i<10; i++)
{
cout << i << endl;
// a bunch of
// code here
}
```

## Modules

Is designed in a modular, object-oriented manner in order to facilitate reuse of code.
There are several main components and it’s important to understand what each does
when designing a new gauge.

* main.cpp - Although this file contains the main() function, very little actually occurs here.
Several global objects are created, notably the primary executable object AppObject.
After instantiating these objects, a rendering callback is established in FLTK and
enters a rendering loop.
