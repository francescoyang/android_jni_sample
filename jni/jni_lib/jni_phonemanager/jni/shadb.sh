#./adb shell am broadcast -a NotifyServiceStop
./adb shell am broadcast -a ServiceStop
./adb forward tcp:12580 tcp:10086
./adb shell am broadcast -a ServiceStart
#./adb shell am broadcast -a NotifyServiceStart
