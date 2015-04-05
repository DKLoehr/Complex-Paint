// Convert sqrt randomly 
function rpm() {
	if(math.random() < .5)
		return -1;
	else
		return 1;
}

math.import({rpm:rpm});

var iterating = false;
var coords = math.complex(0, 0); 
var left = false;
var intervalID = window.setInterval(iterate, 1000 / 60);

function Init() {
	// Add the parameter boxes to the form	
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) {
		var newBox = document.createElement("div");
		newBox.setAttribute("id", String.fromCharCode(iii));
		newBox.setAttribute("onchange", "UpdateEquation()");
		newBox.style.display = "none"; // Default all of them to hidden
		newBox.innerHTML = String.fromCharCode(iii) + ": <input type='text' id='" + 
				   String.fromCharCode(iii) + "Box'><br>";
		document.getElementById("vars").appendChild(newBox);
	}
	document.getElementById("eqBox").value = "0";
	
	// Draw the basic lines on the grids
	DrawGrids();
}

function UpdateEquation() {
	var eqStr = document.getElementById("eqBox").value;
	// Hide or display relevant variable boxes
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) {
		if(eqStr.indexOf(String.fromCharCode(iii)) != -1) // If this character is in the string
			document.getElementById(String.fromCharCode(iii)).style.display = "inline"; // Show its box
		else 
			document.getElementById(String.fromCharCode(iii)).style.display = "none"; // Don't
	}
}

function UpdateGraphs() {
	ClearGraphs();
}

function Preset(newEqStr) {
	document.getElementById("equation").elements[0].value = newEqStr;
	UpdateEquation();
}

function UpdateMode(newMode) {
	document.getElementById("currentMode").innerHTML = newMode;
}

function CopyGraph(direction) {
	if(direction == 0) {
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

function ClearGraphs() {
	iterating = false;
	var grid = document.getElementById("leftGraph");
	ctx = grid.getContext("2d");
	ctx.clearRect(0, 0, grid.width, grid.height);
	
	grid = document.getElementById("rightGraph");
	ctx = grid.getContext("2d");
	ctx.clearRect(0, 0, grid.width, grid.height);

	DrawGrids();
}

function DrawGrids() {
	var grid = document.getElementById("leftGraph");
	var ctx = grid.getContext("2d");
	ctx.beginPath();
	ctx.moveTo(0, grid.height / 2);
	ctx.lineTo(grid.width, grid.height / 2);
	ctx.stroke();
	ctx.moveTo(grid.width / 2, 0);
	ctx.lineTo(grid.width / 2, grid.height);
	ctx.stroke();
	ctx.closePath();
	
	grid = document.getElementById("rightGraph");
	ctx = grid.getContext("2d");
	ctx.beginPath();
	ctx.moveTo(0, grid.height / 2);
	ctx.lineTo(grid.width, grid.height / 2);
	ctx.stroke();
	ctx.moveTo(grid.width / 2, 0);
	ctx.lineTo(grid.width / 2, grid.height);
	ctx.stroke();
	ctx.closePath();
}

// Fairly certain this works
function MathToGraph(cx, left) {
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
	
	var newcx = math.complex(cx.re / xScaleFac, cx.im / yScaleFac);
	newcx.re += grid.width / 2;
	newcx.im = grid.height / 2 - newcx.im;
	
	var center = math.complex(xAverage, yAverage);
	center.re /= xScaleFac;
	center.im /= yScaleFac;
	center.im *= -1;
	newcx = math.subtract(newcx, center);
	return newcx;
}

// Inverse of MathToGraph
function GraphToMath(cx, left) {
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

function DrawPoint(cx, left, color) {
	if(color === undefined) color = "#000000"; // Black?
	var grid;
	if(left)
		grid = document.getElementById("leftGraph");
	else
		grid = document.getElementById("rightGraph");
	var ctx = grid.getContext("2d");
	ctx.beginPath();
	ctx.arc(cx.re, cx.im, 1, 0, 2*Math.PI);
	ctx.fillStyle = color;
	ctx.fill();
	ctx.closePath();
}

// Note that parameters cannot be defined in terms of each other
function EvalString(str, zPt) {
	var args = {};
	args.z = zPt;
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) {	
		if(str.indexOf(String.fromCharCode(iii)) != -1)
			args[String.fromCharCode(iii)] = 
					math.eval(document.getElementById(String.fromCharCode(iii) + "Box").value);
	}
	return math.eval(str, args);
}

function graphClick(event) {
	iterating = !iterating;
	if(!iterating) return;
	coords = getClickLoc(event);
	left = (event.target === document.getElementById("leftGraph"));
	DrawPoint(coords, left, "#00FF00");
	coords = GraphToMath(coords, left);
}

function iterate() {
	if(!iterating) return;
	var numIters = 1;
	var mode = document.getElementById("currentMode").innerHTML;
	if(mode == "Iterate")
		numIters = 30;
	for(iters = 0; iters < numIters; iters++) {
		coords = EvalString(document.getElementById("eqBox").value, coords);
		DrawPoint(MathToGraph(coords, left), left);
		if(mode == "Iterate")
			DrawPoint(MathToGraph(coords, !left), !left)
		left = !left;
	}
	if(mode != "Iterate"	)
		iterating = false;
}

function getClickLoc(event){
	var canvas = event.target;	
	var x = event.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
	var y = event.clientY + document.body.scrollTop + document.documentElement.scrollTop;
	
	var click = math.complex(x - canvas.offsetLeft, y - canvas.offsetTop);

	return click;
}

function foo() {
	debug(math.eval("sqrt(0.464 + 0.448i - 1)"));
}

function debug(str) {
	document.getElementById("foopar").innerHTML += str + "//";
}
