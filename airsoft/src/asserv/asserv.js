let mode = "dist";
let PID = {
	html : {
		Kp: document.querySelector("#Kp"),
		Ki: document.querySelector("#Ki"),
		Kd: document.querySelector("#Kd")
	},
	dist : {
		Kp: 1,
		Ki: 0,
		Kd: 0
	},
	rot : {
		Kp: 1,
		Ki: 0,
		Kd: 0
	}
};

$("#rot").roundSlider({
	sliderType: "default",
	value: 0,
	max: 360,
	radius: 60,
	width: 20,
	change: send_consigne
});

function send_pid()
{
	for (let coef of ["Kp", "Ki", "Kd"])
		PID[mode][coef] = PID.html[coef].value;

	let cmd = `pid ${mode == "dist" ? 0 : 1} ${PID[mode].Kp} ${PID[mode].Ki} ${PID[mode].Kd}`;
	try {
		arduino.exec(cmd);
	}
	catch (e) {
		console.error(e);
	}
}

function send_consigne(e) {
	try {
		if (e.target && e.target.id == "dist")
			arduino.exec("dist " + e.target.value);
		else if (e.id == "rot")
			arduino.exec("rot " + e.value);
	}
	catch (err) {
		console.error(err);
	}
}

let controllers = {
	tabs: {
		dist: document.querySelector(".tab_dist"),
		rot: document.querySelector(".tab_rot")
	},
	dist: document.querySelector("#dist"),
	rot: document.querySelector("#rot")
}

controllers.dist.addEventListener("change", send_consigne);
controllers.dist.addEventListener("keydown", (e) => { if (e.keyCode == 13) send_consigne(e); });

function set_mode(m)
{
	if (mode == m)
		return ;

	// Update display
	controllers.tabs[m].className = "nav-link tab_dist active";
	controllers.tabs[mode].className = "nav-link tab_rot";
	controllers[m].style.display = "flex";
	controllers[mode].style.display = "none";
	mode = m;
	graph.draw();

	// Load PID coefficients
	for (let coef of ["Kp", "Ki", "Kd"])
		PID.html[coef].value = PID[mode][coef];
}

controllers.tabs["dist"].addEventListener("click", () => set_mode("dist"));
controllers.tabs["rot"].addEventListener("click", () => set_mode("rot"));
