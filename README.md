# bitmanip
A small toy for bit manipulation

## usage
It might come helpful for people who are learning binary number for the first time.

## build
`$ make` at project home directory.

## how to use
Start program by
```
$ bitmanip 112 8
```

then you have `x=112`to manipulate, with `bitwidth=8`.

```
starting with
  preset: 112
  bitwidth: 8
    x: 70
  man: 00
```

Two values `x` and `man` are shown on screen with hexadecimal representation.
You can switch to binary representation by pressing `p`.

```
--------------------------------
  putmode changed to Binary
--------------------------------

    x: 01110000
  man: 00000000
```

`man` is scalpel of yours. You can do various things on `x` using `man`.

`h`,`H` performs bitwise shift to left on `man` by 1 bit. Or if it were `0`, it gets to `1`. It is same as `man =<< 1`.
Pressing `h` three times results in,
```
    x: 01110000
  man: 00000001

    x: 01110000
  man: 00000010

    x: 01110000
  man: 00000100
```

Similarly `l`, `L` shifts `man` right byt 1 bit. (`man =>> 1`)

When you press `<Space>`, `man` is OR-ed to `x`. (`x |= man`)
When you press `<CR>`, `man` is AND-ed to `x` (`x &= man`)

Pressing `<Space>` then `<CR>` results in,
```
    x: 01110100
  man: 00000100

    x: 00000100
  man: 00000100
```

Set the value of `man` manually using `N`, `F`, `B`.
They lets you set values in decimal, floating point, binary representation, respectively. Lower case versions `n`, `f`, `b`, sets the value to `x` instead.

Desmonstraion of `n` and `B`,
```
    x: 00000100
  man: 00000100

insert man: 10
    x: 00000100
  man: 00001010

insert x: 1100
    x: 00001100
  man: 00001010
```

When you are done, press `q` to quit.

## Currently not supporting
- Retrieve terminal state when interrupped.
- Show number as floating point number
- Add or substract between numbers
- More than one variable
- Convert float to IEEE 754 format
