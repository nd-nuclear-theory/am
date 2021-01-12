For standalone compilation, use ndmakefile module, e.g.:

```
% ln -s ../ndmakefile/makefile
% ln -s ../ndconfig/config-gnu.mk config.mk
```

For use as a Python module, install using distutils/pip:

```
% pip3 install --user .
--- OR ---
% pip install --user .
```

Note, you may need to set `CFLAGS` and/or `LDFLAGS` so that
distutils/pip can find GSL, e.g.:
```
% env CFLAGS="-I$GSL_DIR/include -L$GSL_DIR/lib" pip install -v --user .
```
