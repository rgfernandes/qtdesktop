# QTagConvert #

Qt-based mp3 tag converter

## Features ##

  * Just convert tags - and nothing else;
  * convert encodings of ID3v1 and ID3v2 tags;
  * convert tags between from ID3v1 to ID3v2 and vice versa - and IDv2 from one version to another: [2.2](http://www.id3.org/id3v2-00), [2.3](http://www.id3.org/id3v2.3.0), [2.4](.md);
  * delete unused tags;
  * workflow-like processing
  * encodings - for all of the fields

## Options ##

  * Autosave last path
  * Autorefresh Filelist on load
  * Defaults:
    * IDv2 ver
    * Fill char on xcoding

## [Main Screen](http://qtdesktop.googlecode.com/files/qtagconvert.ui) ##

  * Dir tree
> > Tree of directories - w/ "recurse" checkbox (find files in selected dir only - or below too)
  * Filelist
> > List of processing files - w/ Refresh/Stop button and "processed" flag lefter filename (to prepend double conversion);
  * Source
> > id3v1 tags (if exists, encoding), id3v2 tags (if exists, ver, encoding)
  * Workflow
> > What to do w/ tags
  * Dest
> > Preview of converted tags
  * Buttons
> > Next, Convert (w/ warning if we try convert yet converted files)
  * Statusbar
> > Current file etc

## Encodings ##

  * Ascii
  * Transcoding
  * koi7
  * koi8-r/u/ru
  * cp866
  * cp1251
  * utf8
  * utf16

## Workflow ##

### ID3v1 ###

  1. What to do:
    1. Don't tought
    1. Delete
    1. Convert encoding
    1. Fill from IDv2
  1. Fields:
    * ...
  1. From encoding:
    1. Std/defined in tag
    1. Force:
      * ...
    1. Auto (try std => enca)
  1. To encoding:
    * ...

### ID3v2 ###

  1. What to do:
    1. Don't tought
    1. Delete
    1. Convert version (???):
      1. From:
        * ...
      1. To:
        * ...
    1. Convert encoding:
    1. Fill from IDv1:
  1. Fields:
    * ...
  1. From encoding:
    * Std/defined in tag
    * Force:
      * ...
    * Auto (try std => enca)
  1. To encoding:
    * Std:
      * ...
    * Force:
      * ...
  1. From version ? ...

## Notes ##

  * Inexisted/deleted tags/fields - disabled
  * All fields - readonly, but selectable