# anzupop-mock-filesystem

[![Travis CI Badge](https://travis-ci.org/xiongnemo/anzupop-mock-filesystem.svg)](https://travis-ci.org/github/xiongnemo/anzupop-mock-filesystem)
[![CodeFactor](https://www.codefactor.io/repository/github/xiongnemo/anzupop-mock-filesystem/badge)](https://www.codefactor.io/repository/github/xiongnemo/anzupop-mock-filesystem)

Course project #3 of Operating System, Jul 2020

For best visual effects, make sure your terminal fully supports ```ANSI Escape Sequences```.

## Project Info

The project work with a custom designed disk image with size of ```0x800000``` bytes, while block size is ```0x1000```.

### Block Type

| Type   | ID |
| ------ | ---------- |
| not_a_node | 0 |
| normal_directory | 1 |
| general_file | 2 |
| general_file_data_block | 3 |
| root_directory | 4 |
| superblock | 5 |

### Disk layout

```GenericBlock```s are blocks that don't belong to ```Superblock```.

| Name   | 4k Block |
| ------ | ---------- |
| Superblock | 0 |
| GenericBlock | 1 ~ 2046 |

## Build

```bash
make
```

## Run

```bash
./anzupop_mock_fs
```

## Test script

```bash
chmod +x ./test_script.sh
./test_script.sh
```
