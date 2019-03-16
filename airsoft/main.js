const { app, BrowserWindow, Menu, dialog  } = require('electron')
//let serial = require("./ArduTest/ArduTest.js");
let serial = require("./serial/build/Release/serial.node");

global.arduino = {
	connected: false,
	port: "COM7",
	exec: function(cmd) {
		serial.write(cmd + "\n");
	},
	get_output: () => strip(serial.read_output()),
	get_cmd: () => strip(serial.read_cmd())
};

function strip(str)
{
	if (typeof str == "string")
		return str.replace(/\r/g, "")
			.replace(/\n$/g, "")
			.replace(/\n/g, "<br>");
}

let mainWindow = null;

app.setName('AIR Soft');
app.on('ready', createWindow)

function createWindow() {
	const windowOptions = {
		width: 920,
		height: 700,
		title: app.getName(),
		icon: 'assets/icon.png',
	};

	process.env['ELECTRON_DISABLE_SECURITY_WARNINGS'] = true;
	mainWindow = new BrowserWindow(windowOptions);

	mainWindow.loadFile('src/shell/index.html')

	mainWindow.on('closed', () => {
		mainWindow = null;
	});

	setMenu();
}


function setMenu()
{
	let template = [{
		label: 'File',
		submenu: [{
			label: 'Open',
			accelerator: 'CmdOrCtrl+O',
			click: (item, focusedWindow) => {
				const options = {
					title: 'Open',
					defaultPath: 'D:/Documents/AIR/',
					filters: [
						{ name: 'Deplacement', extensions: ['json'] }
					]
				};

				dialog.showOpenDialog(options, (filename) => {
					if (!filename || !global.callbacks.onOpen) return;
					
					global.callbacks.onOpen(filename[0]);
				});
			}
		}, {
			label: 'Save',
			accelerator: 'CmdOrCtrl+S',
			click: (item, focusedWindow) => {
				const options = {
					title: 'Save as',
					defaultPath: 'D:/Documents/AIR/',
					filters: [
						{ name: 'Deplacement', extensions: ['json'] }
					]
				};

				dialog.showSaveDialog(options, (filename) => {
					if (!filename || !global.callbacks.onSave) return;
					
					global.callbacks.onSave(filename);
				});
			}
		}, {
			label: 'Export',
			accelerator: 'Shift+CmdOrCtrl+S',
			click: (item, focusedWindow) => {
				const options = {
					title: 'Export as',
					defaultPath: 'D:/Documents/AIR/',
					buttonLabel: 'Export',
					filters: [
						{ name: 'C', extensions: ['c'] }
					]
				};

				dialog.showSaveDialog(options, (filename) => {
					if (!filename || !global.callbacks.onExport) return;
					
					global.callbacks.onExport(filename);
				});
			}
		}, {
			type: 'separator'
		}, {
			label: 'Preferences',
			click: (item, focusedWindow) => {
				console.log("Ouverture modale")
			}
		}, {
			type: 'separator'
		}, {
			label: 'Exit',
			accelerator: 'CmdOrCtrl+W',
			role: 'close'
		}]
	}, {
		label: 'View',
		submenu: [{
			label: 'Reload',
			accelerator: 'CmdOrCtrl+R',
			click: (item, focusedWindow) => {
				if (focusedWindow) {
					// on reload, start fresh and close any old
					// open secondary windows
					if (focusedWindow.id === 1) {
						BrowserWindow.getAllWindows().forEach(win => {
							if (win.id > 1) win.close()
						})
					}
					focusedWindow.reload()
				}
			}
		}, {
			label: 'Toggle Developer Tools',
			accelerator: (() => {
				if (process.platform === 'darwin') {
					return 'Alt+Command+I'
				} else {
					return 'Ctrl+Shift+I'
				}
			})(),
			click: (item, focusedWindow) => {
				if (focusedWindow) {
					focusedWindow.toggleDevTools()
				}
			}
		}]
	}, {
		label: 'Arduino',
		submenu: [{
			label: 'Port',
			submenu:[]
		}, {
			label: 'Connect', enabled: true,
			click: (item, focusedWindow) => {
				if (global.arduino.connected)
					return;
				try {
					serial.connect(global.arduino.port);
					console.log("Connected !");
					global.arduino.connected = true;
					item.enabled = false;
					Menu.getApplicationMenu().items[2].submenu.items[2].enabled = true;

					focusedWindow.setTitle("AIR Soft - Connected on " + global.arduino.port);
					focusedWindow.reload();
				}
				catch (e) {
					console.error(e)
				}
			}
		}, {
			label: 'Disconnect', enabled: false,
			click: (item, focusedWindow) => {
				if (!global.arduino.connected)
					return;
				try {
					serial.disconnect();
					console.log("Disconnected !");
					global.arduino.connected = false;
					item.enabled = false;
					Menu.getApplicationMenu().items[2].submenu.items[1].enabled = true;

					focusedWindow.setTitle("AIR Soft");
					focusedWindow.reload();
				}
				catch (e) {
					console.error(e)
				}
			}
		}]
	}];

	let portMenu = template[2].submenu[0].submenu;
	for (let i = 1; i <= 12; i++)
	{
		portMenu.push({
			label: "COM" + i,
			type: 'radio',
			id: i,
			checked: ("COM" + i == global.arduino.port),
			click: select_COM
		});
	}


	const menu = Menu.buildFromTemplate(template);
	Menu.setApplicationMenu(menu);
}

function select_COM(item, win, e)
{
	global.arduino.port = item.label;
}
