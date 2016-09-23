# svg-point-mapper
Takes vertices coordinates as pixel 2d map

## Build
Required cmake >= 3.5.0
```bash
cd /path/to/project/build
cmake ../
make
make install
```

## Usage
```bash
usage: svgmapper --svg=/path/to/file.svg [options] ...
options:
  -p, --svg        Path to svg (string)
  -s, --scale      Image scale (int [=4])
  -t, --stride     Point stride (int [=4])
  -d, --density    Document density (pixels per inch) (float [=96])
  -v, --debug      Prints some debug info (bool [=0])
  -j, --js         Prints js context drawer (bool [=0])
  -?, --help       print this message

```

## Example result
```json
[
	[135.647,123.553],
	[104.833,123.264],
	[95.5902,123.552],
	[86.8691,124.416],
	[78.6701,125.856],
	[70.9931,127.872],
	[63.8381,130.464],
	[57.2051,133.632],
    ...
]
```

In this example you see array of arrays where first element is X coordinate at left top cornet and second element is Y coordinates at left top corner