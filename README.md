# ruckig-wasm

WebAssembly port of [Ruckig](https://github.com/pantor/ruckig) for time-optimal jerk-constrained trajectory generation in the browser and Node.js.

Created by AATB (and Claude) for experimenting with browser/node based robot control.

## Demo

Open `index.html` in a browser (requires a local HTTP server for WASM loading):

```bash
python3 -m http.server 8080
```

Features: position targets, constraint profiles, live jog, and Perlin noise target tracking.

## Building

Requires [Emscripten](https://emscripten.org/).

```bash
git clone --recursive https://github.com/aatb/ruckig-wasm.git
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
