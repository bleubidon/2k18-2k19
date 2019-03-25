const arduino = require('electron').remote.getGlobal('arduino');

let graph = {
	running: false,
	clear: function()
	{
		graph.data = {time: []};
		graph.data.dist = {
			consigne: {
				x: graph.data.time, y: [], name: "consigne",
				mode: 'lines', type: 'scatter'
			},
			val: {
				x: graph.data.time, y: [], name: "distance",
				mode: 'lines', type: 'scatter'
			}
		};
		graph.data.rot = {
			consigne: {
				x: graph.data.time, y: [], name: "consigne",
				mode: 'lines', type: 'scatter'
			},
			val: {
				x: graph.data.time, y: [], name: "orientation",
				mode: 'lines', type: 'scatter'
			}
		};

		Plotly.newPlot("graph", []);
	},

	draw: function()
	{
		if (!graph.running)
			return;

		Plotly.newPlot("graph", [
			graph.data[mode].consigne,
			graph.data[mode].val
		]);
	}
};


// Events
document.querySelector(".ctrl #toggle").addEventListener("click", () => {
	graph.running = !graph.running;
	graph.draw();
});
document.querySelector(".ctrl #clear").addEventListener("click", () => {
	graph.clear();
});
document.querySelector(".ctrl #stop").addEventListener("click", () => {
	arduino.exec("stop");
});


// Start plot
graph.clear();
graph.running = true;


// Start logger
logger.add("PID", (args) => {
	data = args.split(',');
	if (data.length == 5)
	{
		graph.data.time.push(parseFloat(data[0] / 1000));
		graph.data["dist"].consigne.y.push(parseFloat(data[1]));
		graph.data["dist"].val.y.push(parseFloat(data[2]));
		graph.data["rot"].consigne.y.push(parseFloat(data[3]));
		graph.data["rot"].val.y.push(nmod(parseFloat(data[4]), 360));

		graph.draw();
	}
});
setInterval(logger.update, 1000);

// Utils
function nmod(a, b) {
	return ((a%b)+b)%b;
};
