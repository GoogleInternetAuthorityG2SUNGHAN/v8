// Copyright 2017 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

load("test/mjsunit/wasm/wasm-constants.js");
load("test/mjsunit/wasm/wasm-module-builder.js");

(function exportImmutableGlobal() {
  var builder = new WasmModuleBuilder();
  let globals = [
    [kWasmI32, 'i32_noinit'],              // -
    [kWasmI32, 'i32', 4711],               // -
    [kWasmF32, 'f32_noinit'],              // -
    [kWasmF32, 'f32', Math.fround(3.14)],  // -
    [kWasmF64, 'f64_noinit'],              // -
    [kWasmF64, 'f64', 1 / 7]               // -
  ];
  for (let g of globals) {
    let global_builder = builder.addGlobal(g[0], false).exportAs(g[1]);
    if (g[2]) global_builder.init = g[2];
  }
  var module = builder.instantiate();

  for (let g of globals) {
    assertEquals("number", typeof module.exports[g[1]], g[1]);
    assertEquals(g[2] || 0, module.exports[g[1]], g[1]);
  }
})();

(function cannotExportMutableGlobal() {
  var builder = new WasmModuleBuilder();
  builder.addGlobal(kWasmI32, true).exportAs('g');
  assertThrows(() => builder.instantiate(), WebAssembly.CompileError);
})();

(function cannotExportI64Global() {
  var builder = new WasmModuleBuilder();
  builder.addGlobal(kWasmI64, false).exportAs('g');
  assertThrows(() => builder.instantiate(), WebAssembly.LinkError);
})();
