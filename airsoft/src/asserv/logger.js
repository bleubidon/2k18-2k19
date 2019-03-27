let logger = {
	map: {},

	update: function() {
		let out, timeout = 10;
		while (timeout-- > 0 && (out = arduino.get_output()))
		{
			let colon = out.indexOf(":");
			if (colon == -1)
				console.log("cannot parse:", out);
			else
			{
				let prefix = out.substr(0, colon);
				let content = out.substr(colon + 1);
				if (logger.map[prefix])
					logger.map[prefix](content);
				else
					console.log(`[${prefix}]`, content);
			}
		}
	},

	add: function(prefix, callback) {
		logger.map[prefix] = callback;
	}
};
