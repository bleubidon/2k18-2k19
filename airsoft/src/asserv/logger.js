let logger = {
	map: {},

	update: function() {
		let out;
		while ((out = arduino.get_output())) {
			let colon = out.indexOf(":");
			if (colon == -1)
				console.log(out);
			else
			{
				let prefix = out.substr(0, colon);
				console.log(prefix);
				if (logger.map[prefix])
					logger.map[prefix](out.substr(colon + 1));
				else
					console.log(out);
			}
		}
	},

	add: function(prefix, callback) {
		logger.map[prefix] = callback;
	}
};
