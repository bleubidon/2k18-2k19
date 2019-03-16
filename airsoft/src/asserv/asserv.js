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

function stop_arduino()
{
	arduino.exec("stop");
}

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
	tab_dist: document.querySelector(".tab_dist"),
	tab_rot: document.querySelector(".tab_rot"),
	dist: document.querySelector("#dist"),
	rot: document.querySelector("#rot")
}

controllers.dist.addEventListener("change", send_consigne);

controllers.tab_dist.addEventListener("click", (e) => {
	if (mode == "dist")
		return ;

	mode = "dist";
	controllers.tab_dist.className = "nav-link tab_dist active";
	controllers.tab_rot.className = "nav-link tab_rot";
	controllers.dist.style.display = "flex";
	controllers.rot.style.display = "none";

	for (let coef of ["Kp", "Ki", "Kd"])
		PID.html[coef].value = PID[mode][coef];
});

controllers.tab_rot.addEventListener("click", (e) => {
	if (mode == "rot")
		return ;

	mode = "rot";
	controllers.tab_dist.className = "nav-link tab_dist";
	controllers.tab_rot.className = "nav-link tab_rot active";
	controllers.dist.style.display = "none";
	controllers.rot.style.display = "flex";

	for (let coef of ["Kp", "Ki", "Kd"])
		PID.html[coef].value = PID[mode][coef];
});
