#this file is a powershell script which is run, then commented steps below are run manually

mkdir zephyr-workspace
mkdir .\zephyr-workspace\application
cp .\west.yml .\zephyr-workspace\application\
cp -r .\01_blink\ .\zephyr-workspace\application\
cd zephyr-workspace
docker run --rm -ti -v ${PWD}:/workdir ghcr.io/zephyrproject-rtos/zephyr-build:main


<#
now do this within container:
user@cb92164ad9af:/workdir$ west init -l application/
user@cb92164ad9af:/workdir$ west update

user@c535f42eec4b:/workdir$ cp -r zephyr/samples/basic/blinky application/
user@c535f42eec4b:/workdir$ cd application/
user@c535f42eec4b:/workdir/application$ cd blinky/
user@c535f42eec4b:/workdir/application/blinky$ west build -b nucleo_f767zi
user@c535f42eec4b:/workdir/application/blinky$ cd ..
user@c535f42eec4b:/workdir/application$ cd ..

user@b5dc1d292931:/workdir$ cd application/
user@b5dc1d292931:/workdir/application$ cd 01_blink/
user@b5dc1d292931:/workdir/application/01_blink$ 
west build -p always -b esp32s3_devkitc/esp32s3/procpu -- -DDTC_OVERLAY_FILE=boards/esp32s3_devkitc.overlay 
#>
