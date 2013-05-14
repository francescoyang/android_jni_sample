#!/bin/sh
git init
git add .
git commit -m 'android jni and some sample'
#git remote add origin git@github.com:acanoe/android_jni_sample.git
git push -f git@github.com:acanoe/android_jni_sample.git master 
