unction Init() {
	// Add the parameter boxes to the form	
	for(iii = "A".charCodeAt(0); iii < "Z".charCodeAt(0); iii++) {
		var newBox = document.createElement("div");
		newBox.setAttribute("id", String.fromCharCode(iii));
		newBox.style.display = "none"; // Default all of them to hidden
		newBox.innerHTML = String.fromCharCode(iii) + ": <input type='text' name='" + 
				   String.fromCharCode(iii) + "'><br>";
		document.getElementById("vars").appendChild(newBox);
	}
	
	// Draw the basic lines on the grids
	DrawGrids();
}

function UpdateEquation() {
	var eqStr = document.getElementById("equation").elements[0].value;
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

function ClearGraph() {
	DrawGrids();
}

function DrawGrids() {
	var grid = document.getElementById("leftGraph");
	var ctx = grid.getContext("2d");
	ctx.moveTo(0, grid.height / 2);
	ctx.lineTo(grid.width, grid.height / 2);
	ctx.stroke();
	ctx.moveTo(grid.width / 2, 0);
	ctx.lineTo(grid.width / 2, grid.height);
	ctx.stroke();
	
	grid = document.getElementById("rightGraph");
	ctx = grid.getContext("2d");
	ctx.moveTo(0, grid.height / 2);
	ctx.lineTo(grid.width, grid.height / 2);
	ctx.stroke();
	ctx.moveTo(grid.width / 2, 0);
	ctx.lineTo(grid.width / 2, grid.height);
	ctx.stroke();
}

function DrawPoint(point, left) {
	var grid;
	if(left)
		grid = document.getElementById("leftGraph");
	else
		grid = document.getElementById("rightGraph");
	var ctx = grid.getContext("2d");
	ctx.beginPath();
	ctx.arc(point.x, point.y, 10, 0, 2*Math.PI);
	ctx.fillStyle = "#333";
	ctx.fill();
}

// Currently only works if centered at (0,0)
function MathToGraph(point, left) {
	var grid;	
	if(left)		
		grid = document.getElementById("leftGraph");
	else
		grid = document.getElementById("rightGraph");
	var newpt = {x:0, y:0};
	newpt.x = point.x + grid.width / 2;
	newpt.y = grid.height / 2 - point.y;
	return newpt;
}

function EvalString() {
	return;
}

function foo() {
	//document.getElementById("foopar").innerHTML += 0;
	var pt = {x:25, y:25};
	DrawPoint(WindowToGraph(pt, 1), 1);
}