function Init() {
	// Add the parameter boxes to the form	
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) {
		var newBox = document.createElement("div");
		newBox.setAttribute("id", String.fromCharCode(iii));
		newBox.style.display = "none"; // Default all of them to hidden
		newBox.innerHTML = String.fromCharCode(iii) + ": <input type='text' id='" + 
				   String.fromCharCode(iii) + "Box'><br>";
		document.getElementById("vars").appendChild(newBox);
	}
	
	// Draw the basic lines on the grids
	DrawGrids();
}

function UpdateEquation() {
	var eqStr = document.getElementById("eqBox").value;
	eqStr = eqStr.replace(/z/g, "Z"); // Replace all lower case z's with upper-case Z's.
	var varsStart = document.getElementById("vars");
	// Hide or display relevant variable boxes
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) {
		if(eqStr.indexOf(String.fromCharCode(iii)) != -1) // If this character is in the string
			document.getElementById(String.fromCharCode(iii)).style.display = "inline"; // Show its box
		else 
			document.getElementById(String.fromCharCode(iii)).style.display = "none"; // Don't
	}
}

function UpdateGraphs() {
	return;
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

function DrawPoint(cx, left) {
	var grid;
	if(left)
		grid = document.getElementById("leftGraph");
	else
		grid = document.getElementById("rightGraph");
	var ctx = grid.getContext("2d");
	ctx.beginPath();
	ctx.arc(cx.re, cx.im, 10, 0, 2*Math.PI);
	ctx.fillStyle = "#333";
	ctx.fill();
}

// Currently only works if centered at (0,0), no scaling yet
function MathToGraph(cx, left) {
	var grid;	
	if(left)		
		grid = document.getElementById("leftGraph");
	else
		grid = document.getElementById("rightGraph");
	var newcx = math.complex(cx.re + grid.width / 2, grid.height / 2 - cx.im);
	return newcx;
}

// Currently only works if centered at (0,0), no scaling yet
function GraphToMath(cx, left) {
	var grid;	
	if(left)		
		grid = document.getElementById("leftGraph");
	else
		grid = document.getElementById("rightGraph");
	var newcx = math.complex(cx.re - grid.width / 2, -1 * (cx.im - grid.height / 2));
	return newcx;
}

/* Can cause an recursive loop at the moment, but only if you're silly 
 * enough to assign a parameter to be a function of itself, or form
 * a similar loop in parameter assignments (e.g. A = B and B = A).
 */
function EvalString(str) {
	var args = {};
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) {
		if(str.indexOf(String.fromCharCode(iii)) != -1)
			args[String.fromCharCode(iii)] = 
					EvalString(document.getElementById(String.fromCharCode(iii) + "Box").value);
	}
	return math.eval(str, args);
}

function graphClick(event) {
	var coords = getClickLoc(event);
	DrawPoint(coords, event.target === document.getElementById("leftGraph"));
	coords = GraphToMath(coords);
}

function getClickLoc(event){
	var canvas = event.target;	
	var x = event.clientX + document.body.scrollLeft + document.documentElement.scrollLeft;
	var y = event.clientY + document.body.scrollTop + document.documentElement.scrollTop;
	
	var click = math.complex(x - canvas.offsetLeft, y - canvas.offsetTop);

	return click;
}

function foo() {
	var temp = EvalString(document.getElementById("eqBox").value);
	debug(temp);
	//var pt = {x:25, y:25};
	//DrawPoint(MathToGraph(pt, 1), 1);
}

function debug(str) {
	document.getElementById("foopar").innerHTML += str + "//";
}