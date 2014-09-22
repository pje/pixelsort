## pixelsort

a command-line tool for pixel-sorting png images.

![](https://www.dropbox.com/s/23lidkimvjh4ele/sorted%402x.png?dl=1)

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
