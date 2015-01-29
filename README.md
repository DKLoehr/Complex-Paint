Complex-Paint
=============

This is a remake of a very old program known as Complex Paint. Its purpose is to allow visualization of functions over the complex numbers by displaying input sets and output side by side.

The source code was written in C++ and uses the SFML Library. The source code is available at https://github.com/DKLoehr/Complex-Paint

==========================
Installation Instructions:
Extract the .zip file to whatever directory you wish to keep it in. 
For Windows, ensure that the file VeraMono.ttf is in the same directory as CPaintWin.exe
To run, double-click the .exe file (Windows) or the app (Macs)

===================
Usage Instructions:


I: Entering Equations: The box in the top left corner is the equation input box. Equations should be typed in according to the following rules: 

* 'z' or 'Z' (both are equivalent) is the input (the number corresponding to where you clicked on the graph).

* Parameters should be uppercase: e.g. 'A' 'B', but not 'a' or 'c'. You may use up to 8 parameters due to space constraints.

* You can currently perform the four basic operations (+,-,*,/) as you'd expect, but you must use an operator: If you type in "AB", the program will crash; type "A * B" instead. You may also use ^ for exponentiation (A^2) = A * A = "A squared".

* Functions you can use: sin(x), cos(x), tan(x), asin(x), acos(x), atan(x), sqrt(x), abs(x), ln(x), rpm(). All of these should be fairly self-explanatory, except rpm(), which just generates randomly either 1 or -1. Used for Julia sets.

* Constants you may use: 'pi', 'phi', 'e': These should be all lowercase to distinguish them from the all-uppercase parameters. Note that phi is the golden ratio.

* Whitespace is irrelevant; "A + B" is the same as "A+B". Also, "sqrt(A)" is the same as "sq rt(A)", but the latter one is silly; don't use it.

* Once you have entered your equation, press the nearby "Save Changes" button. This will adjust the parameters below the equation box to show the ones you've used. You may then edit the values of those parameters. Once you have done this, hit the "Save Changes" button again. Note that the border of the "Save Changes" button will be red if you have unsaved changes for either the equation or the parameters.

* You may click on any of the four presets to get pre-written equations that are often used. This automatically saves changes.
	* Linear: 	Az + B
	* Polar Linear: (R*cos(2*pi*T)+R*sin(2*pi*T)*i)*z+B
	* Quadratic: 	z^2 + C
	* Inverse Quad: rpm() * sqrt(z - C)

============================
II: Changing the Graph Settings: 

The set of buttons in the top-right corner of the screen control the graph settings: size, location, and format. The column on the left controls the graph on the left; the right column is similar. The buttons have the following meanings:
* X Range is how far out from the center the graph goes horizontally; e.g. if center is (1, 5), a graph with X Range of 10 would go from -9 to 11.

* Y Range is equivalent to X Range, except in the vertical direction

* X Scale is how far apart the tick marks (or lines) appear on the X axis

* Y Scale is the same as X Scale, except for the Y axis.

* Center is the (real) coordinates of the point where the axes meet, in the form "(X,Y)"; parentheses are optional.

* "Numbers," if checked, will display the X or Y coordinate (as appropriate) next to each tick mark.

* "Lines", if checked, extends the tick marks into full lines to the edge of the graph, forming a grid.

* The "<" and ">" buttons on the top mirror settings from one column to another. For example, if you changed a bunch of settings on the left graph, and wanted to do the same to the right, you would hit the ">" button, rather than manually typing in each change again.

* Once you have made your changes, hit the "Save Changes" button below the columns to see them go into effect. Note that the border of this button will be red if you have unsaved changes.

====================
III: Drawing things: The fun part!

Once you have entered your equation and parameter values, moving your mouse in one of the two graphs on the bottom of the screen will cause the other graph to display a point that moves with your mouse, showing where you would end up if you applied that equation to your current location. 
If you click on the screen, various things happen depend on which mode you are in. Change your mode by clicking on one of the buttons in the middle of the screen; the active mode's button will have a green border.

The possible modes have been added are Point, Iterate, Line, Rectangle, Grid, Circle1, Circle2, Circle3, and Freedraw. Each is activated when you click, and terminates (if applicable) when you click again. With the exception of Iterate and Point, once terminated, the figure you specified will be drawn on the side you created it on; the equation will then be applied to several points on the figure, which is then drawn to the other side.

* Point mode draws the result of applying the current equation to the location where you clicked.

* Iterate mode does the same thing as Point mode, but then applies the equation to the location of the newly drawn point, and then to the result of that, and so on. Each of the results is drawn to both graphs (This is unique to Iterate). Also uniquely, the first point drawn of the most recent iteration will be colored green.

* Line mode simply draws a straight line between the two locations where you click.

* Rectangle mode draws a rectangle with corners at the locations where you click.

* Grid mode does the same as Rectangle mode, except it also draws several lines inside the rectangle parallel to the sides, forming a grid.

* Circle1 draws a circle centered at the origin, passing through the second location you click.

* Circle2 draws a circle centered at the first location you click, passing through the second.

* Circle3 draws several concentric circles, centered at the first location you click; the number is however many will fit between the first and second click locations.

* Freedraw traces your mouse from the first point where you click to the last, and draws the path. Once you click a second time, the function in the equation box will be applied to many points on the shape you drew, and the result of that will be drawn on the opposite graph. The shape you drew will be drawn to the graph on which you clicked (You cannot click on two graphs for one shape).

In all cases, the most recently-drawn point will be colored red, while all other points will be colored black. This is to assist in tracking that point, mostly for use in "Follow-the-mouse."

Note that the code also tracks if you are drawing a Julia set: this is assumed to be the case if the Inverse Quad preset was the last one selected. Note that if you change the equation without using a preset, the program will not recognize that you are no longer drawing a Julia set. Julia sets are drawn to a separate layer, which appears on top of the regular graph.

Hitting the "Clear Graph" button will clear anything drawn on the regular layer (leaving the Julia set layer untouched); Hitting the "Clear Julia" button will clear anything drawn on the Julia Set layer (leaving the regular layer untouched.)

=============
Known Issues and Workarounds:

* Negative numbers are not parsed properly; e.g. "5*-5" will cause the program to crash. Workaround: Put parentheses around the negative number; e.g. "5*(-5)" is parsed correctly
* "-i" is replaced with "-1i" when entered into an equation or variable; this should not affect usage, and is in fact a workaround for a problem related to parsing "-i"