# Preparation
* Install python 2.7 and add it to path
* Install Node.js with at least v10

```
npm install -g electron
```

# Serial Module
### Setup
```
cd serial
npm install -g node-gyp
npm install --global --production windows-build-tools
npm config set python python2.7
npm config set msvs_version 2017 --target_platform=win32
```

### Build
```
node-gyp configure
node-gyp rebuild --target=3.0.12 --arch=x64
```

# Application
### Install
```
npm i
```

### Run
```
npm start
```
