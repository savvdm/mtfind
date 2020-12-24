# mtfind
Simple multi-threaded grep-like utility.

Patterns may contain '?' wildcards, matching any character. Several wildcard characters in a pattern are supported.

## Usage

`mtfind <input_file> <pattern>`

## Limitations
  * Only ascii charset is supported. 
  * Matches cannot cross line boundary.
  * Matches cannot overlap.

## How to build
  1. Clone the repository
  2. Change dir to it
  3. Run the following commands:
```
    cmake .
    make
    make test
```
