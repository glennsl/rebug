
# rebug

A port of [debug](https://github.com/visionmedia/debug) to BuckleScript.

## Status

* Browser back-end has been implemented, Node.js back-end has not
* Formatting isn't properly supported yet
* Colors are always on
* Isn't quite as customizable (yet)

## Example

```reason
/* Reason */
let debug = Debug.make "my:namespace";

a.log "The red cow jumps over the lazy giraffe";

try (...) {
| error => a.log2 "Error occurred while doing stuff: " error;
};
```

See [examples](https://github.com/glennsl/rebug/blob/master/examples/) for more.

## Installation

```sh
npm install --save bs-rebug
```

Then add `bs-rebug` to `bs-dependencies` in your `bsconfig.json`:
```js
{
  ...
  "bs-dependencies": ["bs-rebug"]
}
```
