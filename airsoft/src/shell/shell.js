const arduino = require('electron').remote.getGlobal('arduino');

let history = [];
let histPos = 0;

let shell, prompt;
let arduInfos = {
	name: 'GrosRobot',
	year: '2k19'
};

function initShell() {
	shell = document.querySelector("#shell");

	if (!arduino.connected)
	{
		shell.innerHTML = "<h1>Arduino not connected</h1>";
		return;
	}

	addPrompt();

	shell.addEventListener('click', function() {
		prompt.querySelector('.input').focus();
	});
}

function addPrompt()
{
	prompt = document.createElement("div");

	prompt.innerHTML = `
		<span id='prompt'>> ${arduInfos.name}:${arduInfos.year} </span>
		<input
			class='input'
			type='text'
			autocomplete='off'
			maxlength=85
			onKeyDown='getCmd(event);'>`;

	shell.appendChild(prompt);

	prompt.querySelector('.input').focus();
}

function addOutput(output)
{
	if (output && output.length) {
		let out = document.createElement("div");

		out.innerHTML = output;
		shell.appendChild(out);
	}
	addPrompt();
}

function getCmd(e) {
	let input = prompt.querySelector('.input');
	code = e.keyCode ? e.keyCode : e.charCode;

	switch (code)
	{
	case 13: // Enter
		cmd = htmlEntities(input.value); // XSS
		exec_cmd(cmd);
		history.unshift(cmd);
		histPos = -1;
		break;
	case 38: // UP
		if (histPos < history.length - 1) {
			input.value = history[++histPos];
		}
		break;
	case 40: // DOWN
		if (histPos > 0) {
			input.value = history[--histPos];
		}
		else {
			input.value = '';
			histPos = -1;
		}
	}
}

function exec_cmd(cmd)
{
	let tokens = cmd.split(' ');
	if (tokens.length == 0)
		return ;
	if (builtins[tokens[0]])
		addOutput(builtins[tokens[0]](tokens));
	else
		addOutput(arduino.exec(cmd));
}


function htmlEntities(str) {
	str = str.replace(/</g, '&lt;');
	return str.replace(/\>/g, '&gt;');
}

let builtins = {
	'help': () => {
		let out  = 'List of available commands:';
		for (let cmd in builtins)
			out += "<br>> " + cmd;
		return out;
	},

	'read': () => arduino.get_output(),
	'flush': () => {
		let out = "", temp;
		while ((temp = arduino.get_output()))
			out += temp + "<br>";
			return out;
	},

	'clear': () => {
		while (shell.firstChild)
			shell.removeChild(shell.firstChild);
	}
}
