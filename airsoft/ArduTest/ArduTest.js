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

function write(txt)
{
	console.log(txt);
	let nl = txt.find("\n");
	if (nl == -1)
		return ;
	let cmd = txt.substring(0, nl);

	let map = {
		"dist" : (args) => {
			cmd.push('consigne distance');
			output.push('LOG:1,2,3,4');
			output.push('LOG:12,9,3,6');
			output.push('DEBUG:la fin');
		}
	};

	if (map[cmd])
		map[cmd]();

}

module.exports = {
	connect: () => true,
	disconnect: () => true,
	read_output,
	read_cmd,
	write
};
