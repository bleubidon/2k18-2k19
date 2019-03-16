const arduino = require('electron').remote.getGlobal('arduino');

let ctrl_graph_1 = document.querySelector("#ctrl_graph");
let ctrl_graph_2 = document.querySelector("#clear_graph");
let interval = 0, state = -1;

let graph_data;

clear_graph();
logger.add("PID", (args) => {
	console.log(args);
});

function start_plot()
{
	set_state(1);
}

function clear_graph()
{
	graph_data = {time: []};
	graph_data.dist = {
		consigne: {
			x: graph_data.time, y: [], name: "consigne",
			mode: 'lines', type: 'scatter'
		},
		val: {
			x: graph_data.time, y: [], name: "distance",
			mode: 'lines', type: 'scatter'
		}
	};
	graph_data.rot = {
		consigne: {
			x: graph_data.time, y: [], name: "consigne",
			mode: 'lines', type: 'scatter'
		},
		val: {
			x: graph_data.time, y: [], name: "orientation",
			mode: 'lines', type: 'scatter'
		}
	};

	Plotly.newPlot("graph", []);
}

function kill_plot()
{
	set_state(-1);
	clear_graph();
}

function nmod(a, b) {
	return ((a%b)+b)%b;
};

function fetch_data()
{
	let received = false;
	let payload;
	try {
		while ((payload = arduino.serial.get_log()))
		{
			let data = payload.split(",");
			if (data.length == 5)
			{
				received = true;
				graph_data.time.push(parseFloat(data[0] / 1000));
				graph_data["dist"].consigne.y.push(parseFloat(data[1]));
				graph_data["dist"].val.y.push(parseFloat(data[2]));
				graph_data["rot"].consigne.y.push(parseFloat(data[3]));
				graph_data["rot"].val.y.push(nmod(parseFloat(data[4]), 360));
			}
			else
				console.log(payload);
		}
	} catch (e) {
		console.log(e)
	}
	if (received)
	{
		Plotly.newPlot("graph", [
			graph_data[mode].consigne,
			graph_data[mode].val
		]);
	}
}

function set_state(s)
{
	if (state == -1 || (state == 0 && s == 1))
	{
		ctrl_graph.innerHTML = "Pause";
		fetch_data();
		interval = setInterval(() => {logger.update()}, 1000);
	}
	else if (s == -1)
	{
		ctrl_graph.innerHTML = "Pause";
		interval = clearInterval(interval);
	}
	else if (state == 1 && s == 0)
	{
		ctrl_graph.innerHTML = "Continue";
		interval = clearInterval(interval);
	}
	state = s;
}

function play_pause()
{
	if (state != -1)
		set_state(1 - state);
}
