## Setup

### Required
* Install python 2.7 and add it to path (you may have to uninstall python3 :/)
* Install Node.js with at least v10

### Installation
Download and install node packages:
```
npm install

npm install -g node-gyp
npm install -g --production windows-build-tools
```

Configure node to compile c++ module
```
npm config set python python2.7
npm config set msvs_version 2017 --target_platform=win32
```

## Build
```
cd serial
node-gyp configure
node-gyp rebuild --target=3.0.12 --arch=x64 --target_platform=win32 --dist-url=https://atom.io/download/atom-shell
```

## Run
```
npm start
```
