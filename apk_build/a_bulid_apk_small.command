#! /bin/bash

CURRENT_DIR=`dirname $0`

# input paths
RF_DIR_OLD=$CURRENT_DIR/../ReadingMate

# path that game proj use
RF_DIR_NEW=$CURRENT_DIR/RF_build

RF_CHANNELS=(1003)

create_dir(){
    if [ -d $1 ]
    then
       :
    else
       mkdir $1
    fi
}

copy_dir() {
    cp -r -f $RF_DIR_OLD $RF_DIR_NEW
}

copy_channel() {

    cp -r -f $RF_DIR_OLD/Resources/others  $RF_DIR_NEW/ReadingMate/Resources
    cp -f $RF_DIR_OLD/proj.android-studio/app/AndroidManifest.xml  $RF_DIR_NEW/ReadingMate/proj.android-studio/app/AndroidManifest.xml
    cp -r -f $RF_DIR_OLD/proj.android-studio/app/src/org/cocos2dx/share $RF_DIR_NEW/ReadingMate/proj.android-studio/app/src/org/cocos2dx

}

replace_channel() {

    sed -i "" "s/app_channel_ID/$1/g" $CURRENT_DIR/RF_build/ReadingMate/Resources/config/others_res.xml;
    sed -i "" "s/app_channel_ID/$1/g" $CURRENT_DIR/RF_build/ReadingMate/proj.android-studio/app/AndroidManifest.xml;
    sed -i "" "s/app_channel_ID/$1/g" $CURRENT_DIR/RF_build/ReadingMate/proj.android-studio/app/src/org/cocos2dx/share/MainApplication.java;

}

replace_pngScale() {

    sed -i "" "s/cur_res_png_scale/2/g" $CURRENT_DIR/RF_build/ReadingMate/Resources/config/others_res.xml;

}

build_old() {
    cocos compile -s $RF_DIR_OLD  -p android --android-studio -m release -o   $CURRENT_DIR/RF_out_apk/old_apk
}

build_apk() {
    replace=`basename $1`"_small"
    cocos compile -s $CURRENT_DIR/RF_build/ReadingMate -p android --android-studio -m release   -o   $CURRENT_DIR/RF_out_apk/$replace
}

#do the job
echo  "build_old"
build_old
echo  "ergodic_file"
rm -rf $RF_DIR_NEW
create_dir $RF_DIR_NEW
echo  "copy_dir"
copy_dir $RF_DIR_OLD  $RF_DIR_NEW
echo  "ReSizePng"
$CURRENT_DIR/ReSizePng -path=$CURRENT_DIR/RF_build/ReadingMate/Resources  -pngquant=$CURRENT_DIR/pngquant
for i in ${RF_CHANNELS[*]}
do
   echo "copy_channel"
   copy_channel
   echo  "replace_channel"
   replace_channel $i
   echo  "replace_pngScale"
   replace_pngScale
   echo  "build_apk"
   build_apk  $i
done

