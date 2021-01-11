<script>
  import { default as Emscripten } from 'svelte-emscripten';
  import { default as module } from '../cpp/wasm-module/wasm-test.js';

  let instanceList = [
    {module: module, autorun: true},
    {module: module, autorun: true}
  ];

  const maxGlContexts = 16;

  function addInstance() {
    if (instanceList.length < maxGlContexts)
      instanceList = [
        ...instanceList, 
        {module: module, autorun: true}
      ];
    else
      console.error(`Not creating more than ${maxGlContexts} components.`);
  }

  function resetInstanceList() {
    instanceList = instanceList.slice(0, 2);
  }
</script>

<main>
  {#each instanceList as instance}
    <Emscripten
      module={instance.module}
      options={{ 
        autorun: instance.autorun,
        wasmPath: 'module/wasm-test.wasm'
      }}
    />
  {/each}

  <div class="buttonContainer">
    <button on:click={addInstance}>Add</button>
    <button on:click={resetInstanceList}>Reset</button>
  </div>
</main>

<style>
  main {
    display: grid;
    grid-template-columns: 1fr 1fr;
    grid-auto-rows: 25%;
    gap: 0;
    width: 100%;
    height: 100%;
    max-width: 240px;
  }

  @media (min-width: 640px) {
    main {
      max-width: none;
    }
  }

  .buttonContainer {
    position: fixed;
    width: 4em;
    height: 4em;
    bottom: 2em;
    left: 50%;
    transform: translate(-50%, -50%);
    margin: 0 auto;
  }

  .buttonContainer > button {
    width: 100%;
    height: 50%;
    margin: 0;
  }
</style>