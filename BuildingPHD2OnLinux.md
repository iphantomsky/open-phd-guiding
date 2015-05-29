# Instructions for building PHD2 on Linux #

## Build Dependencies ##
### Fedora-like systems ###

  * install dependencies (commands for a Fedora-like system; your system may vary)
```
sudo yum install subversion cmake libwxgtk3.0-devel wx-common wx3.0-i18n libindi-devel libnova-devel
```

### Ubuntu ###

  * install dependencies (tested on 14.04 and 14.10)
```
sudo apt-get install build-essential subversion cmake libwxgtk3.0-dev wx-common wx3.0-i18n libindi-dev libnova-dev
```

## phd2 ##

  * get the phd2 sources

> https://code.google.com/p/open-phd-guiding/source/checkout

We'll refer to the source tree location as $PHD2\_SRC

  * generate the Makefiles

```
  mkdir -p $PHD2_SRC/tmp
  cd $PHD2_SRC/tmp
  cmake ..
```

  * build phd2

```
  cd $PHD2_SRC/tmp
  make
```

  * run phd2

```
  cd $PHD2_SRC/tmp
  ./phd2
```



---


# Build Debian packages #

## Prepare the system with the required packages ##

If you plan to build the packages locally on your computer you need to install all the prerequisite described above.

You also need to install the specific Debian build tools, this is done by installing the following meta package:
```
apt-get install packaging-dev
```

If you want to sign the package (required for uploading) you need to setup a gpg and a ssh key. See http://packaging.ubuntu.com/html/getting-set-up.html

## Prepare the source tar ##

We need to start with a copy of the source code located in a directory named with the full version name, here 2.5.0 is used as an example, you must replace it by the current version in all the following commands. This directory must not include any subversion information. For that we use the "svn export" command:
```
svn export http://open-phd-guiding.googlecode.com/svn/trunk phd2_2.5.0
```
Then we remove some files not used by Linux:
```
cd phd2_2.5.0
rm -rf extra_frameworks WinLibs *.a
```
Ensure the translations are up to date and compiled:
```
cd build
./locales_update.sh
./locales_compile.sh
```
Ensure the help file is updated for all the languages:
```
cd build
./build_help.sh
```
Update the files in the phd2\_2.5.0/debian directory. Be careful that every character in this files is important for the build process. Refer to the Debian documentation if your are not familiar with this files.
  * The file "changelog" must be updated to include the new version you package and the target system you want to build.
  * The file "control"  must be checked to add every new build dependency.
  * The file "copyright" must be updated to match the content of the PHD2 About dialog.

Create the versioned source tar:
```
tar cvzf phd2_2.5.0.orig.tar.gz phd2_2.5.0
```

## Build the package locally ##

Even if you plan to upload the source package it is a good idea to test if it builds correctly on your system.
```
cd phd2_2.5.0
debuild -us -uc
```
You can now test the resulting .deb on your system.

## Build the package on a Launchpad PPA ##

We need to build a source package that is later uploaded to the PPA and compiled there. If we want to target many Ubuntu version we need to repeat the process for every version after updating the file debian/changelog.

Only the first package build for a new version must include the source tar `*`.orig.tar.gz. Use the following command to build it:
```
cd phd2_2.5.0
debuild -S -sa
```

Change the Ubuntu version in debian/changelog:
```
sed -i '/2.5.0/s/trusty/utopic/g' debian/changelog
```
Make the source package without `*`.orig.tar.gz :
```
debuild -S -sd
```

Upload the source packages to the PPA, first the one with the source tar:
```
dput ppa:username/ppaname phd2_2.5.0-0ppa1~trusty1_source.changes
dput ppa:username/ppaname phd2_2.5.0-0ppa1~utopic1_source.changes
```

Connect to the PPA and check the build result for every version and architecture.