/** Add the rpm function to the math.js library **/
// Randomly returns 1 or -1 with equal probability
function rpm() {
	if(math.random() < .5)
		return -1;
	else
		return 1;
}
math.import({rpm:rpm});

/** One global object to hold all of our global variables, for clarity and safety **/
var CPaintGlobals = {
    iterating: false,           // Whether or not we're currently CPaintGlobals.iterating the written equation
    coords: math.complex(0, 0), // For use when CPaintGlobals.iterating: The graph coordinates of the last-drawn point
    left: false,                // For use when CPaintGlobals.iterating: True if we should draw our next point to the left graph, false otherwise
    intervalID: window.setInterval(iterate, 1000 / 60) // Sets the window to call the iterate function every tick; stores the ID of that interval
}

/* Perform tasks which we want to do right after the document is loaded:
 *  Dynamically creates the parameter boxes for the equation, and sets them to be hidden
 *  Draws the axes (and other information on the canvases)
 */
function Init() {
	// Add the parameter boxes to the form
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) {
		var newBox = document.createElement("div");             // Create the new division to hold the box
		newBox.setAttribute("id", String.fromCharCode(iii));    // Set the division's id to be the current char, e.g. "A", "B"
		newBox.style.display = "none";                          // Default all of them to hidden
		newBox.innerHTML = String.fromCharCode(iii) +           // Add the box to the div
                           ": <input type='text' id='" +            // Box ids are the div id + "Box", e.g. "ABox", "BBox"
                           String.fromCharCode(iii) + "Box'><br>";
		document.getElementById("params").appendChild(newBox);      // Add the box to the parameter div
		document.getElementById(String.fromCharCode(iii) + "Box").value = "0"; // Default the box to contain the string "0"
	}

	DrawGrids(); // Draw the basic lines on the grids
}

/* Update which variable boxes are displayed whenever the equation is changed */
function UpdateEquation() {
	var eqStr = document.getElementById("eqBox").value; // The new equation, which determines which boxes to show

	// Hide or display relevant variable boxes
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) { // Loop through each of the uppercase letters, except Z
		if(eqStr.indexOf(String.fromCharCode(iii)) != -1) // If the character is in the string
			document.getElementById(String.fromCharCode(iii)).style.display = "inline"; // Show its box
		else
			document.getElementById(String.fromCharCode(iii)).style.display = "none"; // Otherwise, make sure the box is hidden
	}
}

/* Sets the equation to newEqStr, then adjusts variable boxes accordingly. Called by the Load Preset dropdown
 *  inputs: newEqStr - A string following the normal rules for an equation (as described in the readme).
 */
function Preset(newEqStr) {
	document.getElementById("equation").elements[0].value = newEqStr;
	UpdateEquation();
}

/* Parses the input string as an expression, and returns its numerical value.
 *  The parser will use the values in each of the variable boxes for capital letters, except Z.
 *  The value of zPt will be used as the value for "z", if it's in the string.
 *
 *  inputs: str - The expression to be evaluated as a string. Should follow the normal rules for an equation.
 *          zPt - The value substituted for "z" in the expression. Will most likely be a math.complex object.
 *
 *  returns: a number representing the numerical value of the evaluated expression.
 */
function EvalString(str, zPt) {
	var args = {z: zPt}; // The values of each parameter in the expression.
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) {
		if(str.indexOf(String.fromCharCode(iii)) != -1) // For each uppercase char in the string
			// Set that letter's argument to be the numerical value of its contents
			args[String.fromCharCode(iii)] =
					math.eval(document.getElementById(String.fromCharCode(iii) + "Box").value);
	}
	return math.eval(str, args); // Evaluate the expression given the arguments we just put together
}

/* Updates the info displayed on the graphs whenever the graph settings are changed, such as center and bounds.
 *  Currently not very useful, as we don't display any information besides the axes anyway
 */
function UpdateGraphs() {
	ClearGraphs();
}

/* Copies the settings of the left graph to the right if direction is truthy, or vice-versa otherwise.
 *  inputs: direction - a boolean determining which graph is copied -- left is copied if truthy, else right is copied.
 */
function CopyGraph(direction) {
	if(direction) {
		document.getElementById("RxMinBox").value = document.getElementById("LxMinBox").value;
		document.getElementById("RxMaxBox").value = document.getElementById("LxMaxBox").value;
		document.getElementById("RyMinBox").value = document.getElementById("LyMinBox").value;
		document.getElementById("RyMaxBox").value = document.getElementById("LyMaxBox").value;
		document.getElementById("RScaleBox").value = document.getElementById("LScaleBox").value;
	} else {
		document.getElementById("LxMinBox").value = document.getElementById("RxMinBox").value;
		document.getElementById("LxMaxBox").value = document.getElementById("RxMaxBox").value;
		document.getElementById("LyMinBox").value = document.getElementById("RyMinBox").value;
		document.getElementById("LyMaxBox").value = document.getElementById("RyMaxBox").value;
		document.getElementById("LScaleBox").value = document.getElementById("RScaleBox").value;
	}
}

/* Draws the basic information of the graph on to each canvas.
 *  Currently this basic information is just the two axes.
 */
function DrawGrids() {
	var canvas = document.getElementById("leftGraph");  // Get the left canvas
	var ctx = canvas.getContext("2d");                  // Left canvas' context
	// Draw the two lines
	ctx.beginPath();
	ctx.moveTo(0, canvas.height / 2);
	ctx.lineTo(canvas.width, canvas.height / 2);
	ctx.stroke();
	ctx.moveTo(canvas.width / 2, 0);
	ctx.lineTo(canvas.width / 2, canvas.height);
	ctx.stroke();
	ctx.closePath();

    // Repeat for right canvas
	canvas = document.getElementById("rightGraph");
	ctx = canvas.getContext("2d");
	ctx.beginPath();
	ctx.moveTo(0, canvas.height / 2);
	ctx.lineTo(canvas.width, canvas.height / 2);
	ctx.stroke();
	ctx.moveTo(canvas.width / 2, 0);
	ctx.lineTo(canvas.width / 2, canvas.height);
	ctx.stroke();
	ctx.closePath();
}

/* Clear stuff that we've drawn from both canvases, and stop iterating if we were */
function ClearGraphs() {
	CPaintGlobals.iterating = false;                    // Stop iterating
	var canvas = document.getElementById("leftGraph");  // Left canvas
	ctx = canvas.getContext("2d");
	ctx.clearRect(0, 0, canvas.width, canvas.height);   // Clear the canvas to white

	canvas = document.getElementById("rightGraph");     // Repeat for right canvas
	ctx = canvas.getContext("2d");
	ctx.clearRect(0, 0, canvas.width, canvas.height);

	DrawGrids(); // Re-draw the axes that we just cleared
}

/* Converts a point on the graph to its equivalent coordinates on the canvas, so it can be drawn.
 *  Note: The graph is the mathematical representation, while the canvas is the object on which the graph is drawn.
 *      The graph and canvas have different coordinate systems. For example, with default values, the point where the axes intersect
 *      would have coordinates (0,0) on the graph, but coordinates(300, 300) on the canvas.
 *
 *  inputs: cx - a math.complex object representing the coordinates on the graph of the point we're converting
 *          left - Whether or not we're using the left graph/canvas or the right set.
 *
 *  returns: a math.complex object with coordinates in the canvas system corresponding to the input point in the graph system.
 */
function GraphToCanvas(cx, left) {
	var grid, prefString;
	if(left) {
		prefString = "L";
		grid = document.getElementById("leftGraph");
	}
	else {
		prefString = "R";
		grid = document.getElementById("rightGraph");
	}

	// The average of the values for xMin and xMax -- for locating the center
	var xAverage = (parseInt(document.getElementById(prefString + "xMaxBox").value) +
			parseInt(document.getElementById(prefString + "xMinBox").value)) / 2;
    // The average of the values for yMin and yMax
	var yAverage = (parseInt(document.getElementById(prefString + "yMaxBox").value) +
			parseInt(document.getElementById(prefString + "yMinBox").value)) / 2;

    // The difference of the values for xMax and xMin -- for figuring out the scale factor between the systems
	var xDiff = (parseInt(document.getElementById(prefString + "xMaxBox").value) -
		     parseInt(document.getElementById(prefString + "xMinBox").value)) / 2;
	// The difference of the values for yMax and yMin
	var yDiff = (parseInt(document.getElementById(prefString + "yMaxBox").value) -
		     parseInt(document.getElementById(prefString + "yMinBox").value)) / 2;

	// The amount that the two axes are scaled by between the systems
	var xScaleFac = (grid.width / 2) / xDiff;
	var yScaleFac = (grid.height / 2) / yDiff;

    // First, take the input point and multiply it by the conversion factor
	var newcx = math.complex(cx.re * xScaleFac, cx.im * yScaleFac);
    // Then translate over by the center of the grid on the canvas
	newcx.re += grid.width / 2;
	newcx.im = grid.height / 2 - newcx.im; // The y-axis is inverted, so we subtract it instead of adding it

	var center = math.complex(xAverage, yAverage); // The graph coordinates of the center point on the graph
	// Also scale the center to match the new system
	center.re *= xScaleFac;
	center.im *= yScaleFac;
	center.im *= -1; // Inver y-axis
	newcx = math.subtract(newcx, center); // Finally, translate by the (converted) center of the grid on the graph

	return newcx;
}

/* Converts a point on the canvas to its equivalent coordinates on the graph, so we can do calculations on it. Inverts GraphToCanvas
 *  Note: The graph is the mathematical representation, while the canvas is the object on which the graph is drawn.
 *      The graph and canvas have different coordinate systems. For example, with default values, the point where the axes intersect
 *      would have coordinates (0,0) on the graph, but coordinates(300, 300) on the canvas.
 *
 *  inputs: cx - a math.complex object representing the coordinates on the canvas of the point we're converting
 *          left - Whether or not we're using the left graph/canvas or the right set.
 *
 *  returns: a math.complex object with coordinates in the graph system corresponding to the input point in the canvas system.
 */
function CanvasToGraph(cx, left) {
	var grid, prefString;
	if(left) {
		prefString = "L";
		grid = document.getElementById("leftGraph");
	}
	else {
		prefString = "R";
		grid = document.getElementById("rightGraph");
	}
	var xAverage = (parseInt(document.getElementById(prefString + "xMaxBox").value) +
			parseInt(document.getElementById(prefString + "xMinBox").value)) / 2;
	var yAverage = (parseInt(document.getElementById(prefString + "yMaxBox").value) +
			parseInt(document.getElementById(prefString + "yMinBox").value)) / 2;
	var xDiff = (parseInt(document.getElementById(prefString + "xMaxBox").value) -
		     parseInt(document.getElementById(prefString + "xMinBox").value)) / 2;
	var yDiff = (parseInt(document.getElementById(prefString + "yMaxBox").value) -
		     parseInt(document.getElementById(prefString + "yMinBox").value)) / 2;
	var xScaleFac = xDiff / (grid.width / 2);
	var yScaleFac = yDiff / (grid.height / 2);

    // Do all the steps from GraphToCanvas in the opposite order
	var center = math.complex(xAverage, yAverage);
	center.re /= xScaleFac;
	center.im /= yScaleFac;
	center.im *= -1;
	newcx = math.add(cx, center);

	newcx.re -= grid.width / 2;
	newcx.im = -1*(newcx.im - grid.height / 2);
	newcx.re *= xScaleFac;
	newcx.im *= yScaleFac;
	return newcx;
}

/* Draws a point to the specified graph of the specified color
 *
 * inputs:  cx - a math.complex object representing the coordinates on the canvas of the point to draw
 *          left - draws to the left graph if truthy, or to the right graph if falsy
 *          color - optional. Specified the color of the point. Default is black.
 */
function DrawPoint(cx, left, color) {
	if(color === undefined) color = "#000000"; // Default color to black
	var canvas; // The canvas to which we draw
	if(left) // Get the appropriate canvas
		canvas = document.getElementById("leftGraph");
	else
		canvas = document.getElementById("rightGraph");
	var ctx = canvas.getContext("2d");
	ctx.beginPath();
	ctx.arc(cx.re, cx.im, 1, 0, 2*Math.PI); // Full circle at (cx.re, cx.im). Radius 1 makes it a 4-pixel square.
	ctx.fillStyle = color;
	ctx.fill();
	ctx.closePath();
}

/* Applies the expression in the equation box to the last set of coordinates, and draws the resulting point.
 *  Runs once per tick.
 */
function iterate() {
	if(!CPaintGlobals.iterating) return; // We're not currently drawing, so don't do anything
	var numIters = 1; // For all modes but iterate, we just draw one point
	var mode = document.getElementById("modeSelect").value; // The current mode, as a capitalized string
	if(mode == "Iterate") // If we're iterating, draw 30 points at once so it goes faster
		numIters = 30;
	for(iters = 0; iters < numIters; iters++) {
		CPaintGlobals.coords = EvalString(document.getElementById("eqBox").value, CPaintGlobals.coords); // Update the point we're tracking
		DrawPoint(GraphToCanvas(CPaintGlobals.coords, CPaintGlobals.left), CPaintGlobals.left); // Draw the point to the correct canvas
		if(mode == "Iterate") // If we're iterating, we draw to both canvases
			DrawPoint(GraphToCanvas(CPaintGlobals.coords, !CPaintGlobals.left), !CPaintGlobals.left) // Draw to the other canvas
		// CPaintGlobals.left = !CPaintGlobals.left; // Toggle which canvas we draw to.
	}
	if(mode != "Iterate") // If we're not iterating, we just wanted one point, so we're done.
		CPaintGlobals.iterating = false; // Stop iterating
}

/* Event handler for when one of the graphs is clicked. Toggles iteration and sets a new start point.
 *
 *  inputs: event - the click event that triggered this function.
 */
function graphClick(event) {
	CPaintGlobals.iterating = !CPaintGlobals.iterating; // Toggle iteration
	if(!CPaintGlobals.iterating) return;                // If we're not iterating, we're done
	clickLoc = getClickLoc(event);                      // The canvas coordinates of the click
	CPaintGlobals.left = !(event.target === document.getElementById("leftGraph"));  // We draw to the opposite canvas of the one that was clicked
	DrawPoint(clickLoc, !CPaintGlobals.left, "#33CC33");                            // Draw the location of the click in green on the canvas we clicked
	CPaintGlobals.coords = CanvasToGraph(clickLoc, CPaintGlobals.left); // Store the graph coordinates of the click for iteration
}

/* Returns the canvas coordinates of the click event as a math.complex object*/
function getClickLoc(event){
	var canvas = event.target;
	var x = event.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
	var y = event.clientY + document.body.scrollTop + document.documentElement.scrollTop;

    // The location of the canvas on the screen. Would be nice to find a way to get them automatically.
    var xLoc = (event.target === document.getElementById("leftGraph")) ? 8 : 615;
    var yLoc = 330;

	var click = math.complex(x - xLoc, y - yLoc);

	return click;
}

/* For debugging (obviously). Prints the given string to the "Debug: " section of the window */
function debug(str) {
	document.getElementById("debug").innerHTML += str + "//";
}
