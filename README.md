# ruckig-wasm

WebAssembly port of [Ruckig](https://github.com/pantor/ruckig) for time-optimal jerk-constrained trajectory generation in the browser and Node.js.

Created by AATB (and Claude) for experimenting with browser/node based robot control.

## Demo

**[Live demo](https://aatb-ch.github.io/ruckig-wasm/)** — position targets, constraint profiles, live jog, and Perlin noise target tracking.

## Node.js Usage

```bash
node example.js
```

```js
var createRuckigModule = require('./dist/ruckig.js');

createRuckigModule().then(function(Module) {
    var joint = new Module.RuckigJoint(0.001); // 1ms timestep
    joint.setConstraints(180, 360, 1000);      // deg/s, deg/s², deg/s³
    joint.setTargetPosition(90);

    var result;
    do { result = joint.update(); } while (result === 0);

    console.log('Position: ' + joint.getPosition().toFixed(2) + '°'); // 90.00°
    joint.delete();
});
```

## Building

Requires [Emscripten](https://emscripten.org/).

```bash
git clone --recursive https://github.com/aatb-ch/ruckig-wasm.git
cd ruckig-wasm
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk && ./emsdk install latest && ./emsdk activate latest && cd ..
source emsdk/emsdk_env.sh
mkdir -p build && cd build
emcmake cmake ..
emmake make -j4
cp ruckig.js ruckig.wasm ../dist/
```

Pre-built WASM binaries are included in `dist/`.

## License

Ruckig is licensed under MIT. See [ruckig/LICENSE](ruckig/LICENSE).
