# OpenGC Programmer’s Guide

## Developer’s guide

Style guide - read me before coding!
Please adhere to the following style conventions when writing code for OpenGC. These conventions
help to ensure easy interpretation of code in OpenGC and provide for consistent formatting between
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
* Classes which are part of OpenGC should be prefaced with ogc, for instance ogcMyClass.

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

## Comments

Please comment liberally, and explain anything that is not immediately obvious. When commenting
class header files, please use doxygen style comments, e.g. /** This is a comment */ to permit
automatic generation of documentation web pages. When commenting code elsewhere, please use
// rather than /* */ - this makes it easier to comment out large sections of code when debugging.

## Headers and copyrights

All classes which are part of OpenGC should include the following copyright/licensing header at
the top of the file. Material between the “$” marks is filled in by CVS to indicated revision history.
You should modify the original author and contributors as appropriate.
Classes and/or other files which are distributed with OpenGC but were not created by the OpenGC
team (e.g. SimGear) should preserve whatever formatting and licensing was distributed with the
original source code. If appropriate, include in CVS a copy of any licenses or notices that accompany
the code.

## OpenGC modules

OpenGC is designed in a modular, object-oriented manner in order to facilitate reuse of code.
There are several main components to OpenGC, and it’s important to understand what each does
when designing a new gauge.

* main.cpp - Although this file contains the main() function, very little actually occurs here.
Several global objects are created, notably the primary executable object ogcAppObject.
After instantiating these objects, a rendering callback is established in FLTK and OpenGC
enters a rendering loop.
