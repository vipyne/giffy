# giffy

![new alt](https://github.com/vipyne/giffy/blob/master/readme.gif)
![new alt](https://github.com/vipyne/giffy/blob/master/reallyreadme.gif)

Can you tell the difference between these two animated gifs? They *are* different. Open each in a hex editor if you don't believe me or... use this program to decode the secret message in the second one!

Compile
-
```
make giffy
```

Usage
-
  Encode a message:
```
$ ./giffy.exe e [input-image-file] [output-filename] [message-string]
```
  Decode a message:
```
$ ./giffy.exe d [input-image-file]
```

Example
-

  Encode:
```
$ ./giffy.exe e /input/path/to/file /desired/output/my_filename 'hey friend, what is new with you?'
```
  Decode:
```
$ ./giffy.exe d ~/my_filename

hey friend, what is new with you?

```

It's just like passing notes in class.

lots of thanks to [this](http://giflib.sourceforge.net/whatsinagif/bits_and_bytes.html) and [this](http://www.matthewflickinger.com/lab/whatsinagif/bits_and_bytes.asp#global_color_table_block).
