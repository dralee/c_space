### build
1.  setup builddir
```bash
chmod +x build.sh
./build.sh
```
2. build code
```bash
cd builddir
ninja
```
meson version newer than 0.55.0,can use the command to build instead
```bash
cd builddir
meson compile
```
3. run the app
```
./helloworld
``` 

