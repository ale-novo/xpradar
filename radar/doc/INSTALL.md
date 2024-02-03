# Installation and Getting Started

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
