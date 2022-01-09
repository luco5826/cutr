# cutr

This software is just a reimplementation of the popular Linux `cut` command provided by the [core-utils](https://www.gnu.org/software/coreutils/) package, with support for **regular expressions**.

## Problem 

By default, `cut` allows the user to specify a single character as a delimiter; this limitation could lead to write more complex bash scripts when dealing with formatted output or non-single-character-delimited output.

Here's an example of a formatted output processed with `cut`, when processing input like
```
field1:  value
field10: value 
```

`cut` alone cannot extract just the value column

```bash
echo -n "field1:  value1\nfield10: value2\n" | cut -d' ' -f 2
```
Which results in output
```

value2
```

While the correct output would be retrieved with something like

```bash
echo -n "field1:  value1\nfield10: value2\n" | rev | cut -d' ' -f1 | rev
echo -n "field1:  value1\nfield10: value2\n" | tr -s ' ' | cut -d' ' -f2
# ...and so on
```

But when the separator is not a single space, and with each tool following its own pattern syntax, these solution become harder to read and to mantain

## Solution

`cutr` on the other hand, allows the user to provide a regular expression as a delimiter, leveraging the C++ built-in regex engine and the well known and documented [ECMAScript syntax](https://www.cplusplus.com/reference/regex/ECMAScript/)

Hence the same script could become

```bash
echo -n "field1:  value1\nfield10: value2\n" | cutr -d"\\s+" -f2
```

And the output would be the expected one

## Installation

The only dependency is the `g++` compiler which come preinstalled in many Linux distros.

The following instructions will install this tool in your `/usr/bin/` folder, feel free to edit the destination path accordingly if this is not what you want

```
git clone https://github.com/luco5826/cutr.git
cd cutr
g++ cutr.cpp -O2 -o cutr
```

And, if you want it installed in your default executable folder, simply copy it inside `/usr/bin`

```
sudo cp cutr /usr/bin
```

## Disclaimer

Some notes on the usage of `cutr` instead of `cut`:
- `cutr` is not a drop-in replacement for the original `cut` command, since it lacks some other features that will definitely be implemented in the future also in `cutr`, but for basic scripting it may be a more flexible choice indeed.
- when dealing with ranges (e.g., fields 2-4), the original `cut` includes in the output also the delimiter. `cutr` outputs instead just the selected fields (in this case 2, 3, 4) without the original delimiter
