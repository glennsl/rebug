
# rebug

Originally a port of [debug](https://github.com/visionmedia/debug) to BuckleScript, but has since expanded a bit.

## Status

* Browser back-end has been implemented, Node.js back-end has not
* Formatting isn't properly supported yet
* Colors are always on
* Isn't quite as customizable (yet)

## Example

```reason
/* Reason */

/* creates a logger with the given namespace */
let debug = Debug.make("my:namespace");

/* debug.log* logs the message only if the namespace of the logger is enabled */
debug.log1("The red cow jumps over the lazy giraffe");

/* debug.error* will log the message regardless of whether the namespace is
   enabled, format it as an error and include a stack trace */
try (...) {
| error => a.error2("Error occurred while doing stuff: ", error)
};

/* debug.fn* groups any log output during its execution, and logs the arguments
   passed to and the value returned from the function */
let loggedParse = debug.fn1("JSON.parse", Js.Json.parseExn);
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
