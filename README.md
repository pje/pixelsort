## pixelsort

a command-line tool for pixel-sorting png images.

![](https://user-images.githubusercontent.com/319655/43423202-bdb21664-9419-11e8-82a0-72988c71dc92.png)

### build

```sh
$ make
```

### use

```sh
$ ./pixelsort [options] <in.png> <out.png>

options:
  -o, --orientation     [row|column|both] (row)
  -m, --method          [light|dark|all] (light)
  -c, --comparator      [avg|mul|max|min|xor] (min)
  -t, --threshold       [0..255] (210)
```
