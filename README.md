## Resolve above depencies, for debian/ubuntu:

```
sudo apt install libfltk1.3-dev -y
sudo apt install libftgl-dev -y
sudo apt install libfreetype6-dev -y
sudo apt install build-essential -y
sudo apt install autoconf -y
```

## Compile the plugin

Set up all automake/autoconf files:

```
autoreconf
automake --add-missing
autoreconf
```

Build:

```
./configure
make
make install
```

This will generate a directory plugins/xpserver.
Copy the xpserver subdirectory to your X-Plane directory under Resources/plugins/

## Compile the radar

Set up autoconf:

```
autoreconf
automake --add-missing
autoreconf
```

Build:

```
./configure 
make
```

This should build the application in src/main.

### Run:

Start X-Plane first.

Edit the file inidata/default.ini and change the following items according to your X-Plane configuration:

```
cd src/main
./xpradar default
```

It should start a X window with a default gauge and tell you on the command line that it expects a X-Plane connection

Start X-Plane with the xpserver plugin. You should see the display content changing according
To your flight data. The current code is in development, any updates welcome.
