
TOOLS_PATH=`dirname $0`
cd ${TOOLS_PATH}

# echo "开始时间："
num1=`date +%s`

# cocos compile -p android --android-studio -j 4 --ap android-22 --app-abi armeabi-v7a
cocos compile -p android --android-studio -j 4 --ap android-22 --app-abi armeabi-v7a -m release

# echo "结束时间："
num2=`date +%s`

num=`expr "$num2" "-" "$num1"`

echo "总共耗时 $num 秒"

