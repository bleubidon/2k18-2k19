let output = [];
let cmd = [];

function read_output()
{
	let out = output[0];
	output.shift();
	return out;
}

function read_cmd()
{
	let out = cmd[0];
	cmd.shift();
	return out;
}

let time = 0;
let r, d;

function write(txt)
{
	let map = {
		"dist" : (args) => {
			r = 0;
			d = args[1];
			do_log();
		},
		"rot" : (args) => {
			d = 0;
			r = args[1];
			do_log();
		}
	};

	let tokens = txt.slice(0, -1).split(' ');
	if (tokens.length == 0)
		return ;

	if (map[tokens[0]])
		map[tokens[0]](tokens);
	if (cmd.length == 0)
		cmd.push("");
}

function do_log()
{
	output.push(`PID:${time++*500},${d},${Math.random()*d},${r},${Math.random()*r}`);
	if (time % 10 != 0)
		setTimeout(do_log, 500);
}

module.exports = {
	connect: () => true,
	disconnect: () => true,
	read_output,
	read_cmd,
	write
};
